// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../Bech32.h"
#include "../Bitcoin/Bech32Address.h"

#include <TrezorCrypto/ecdsa.h>
#include <TrustWalletCore/TWBech32Address.h>
#include <TrustWalletCore/TWHash.h>
#include <TrustWalletCore/TWHRP.h>
#include <TrustWalletCore/TWPublicKey.h>

using namespace TW;
using namespace TW::Bitcoin;

bool TWBech32AddressEqual(struct TWBech32Address *_Nonnull lhs, struct TWBech32Address *_Nonnull rhs) {
   return lhs->impl == rhs->impl;
}

bool TWBech32AddressIsValidString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);
    return Bech32Address::isValid(*s);
}

struct TWBech32Address *_Nullable TWBech32AddressCreateWithString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);
    auto dec = Bech32Address::decode(*s);
    if (!dec.second) {
        return nullptr;
    }

    return new TWBech32Address{ std::move(dec.first) };
}

struct TWBech32Address *_Nonnull TWBech32AddressCreateWithPublicKey(enum TWHRP hrp, struct TWPublicKey *_Nonnull publicKey) {
    const auto address = Bech32Address(publicKey->impl, 0, stringForHRP(hrp));
    return new TWBech32Address{ std::move(address) };
}

void TWBech32AddressDelete(struct TWBech32Address *_Nonnull address) {
    delete address;
}

TWString *_Nonnull TWBech32AddressDescription(struct TWBech32Address *_Nonnull address) {
    const auto string = address->impl.string();
    return TWStringCreateWithUTF8Bytes(string.c_str());
}

enum TWHRP TWBech32AddressHRP(struct TWBech32Address *_Nonnull address) {
    return hrpForString(address->impl.hrp.c_str());
}

TWData *_Nonnull TWBech32AddressWitnessProgram(struct TWBech32Address *_Nonnull address) {
    return TWDataCreateWithBytes(address->impl.witnessProgram.data(), address->impl.witnessProgram.size());
}
