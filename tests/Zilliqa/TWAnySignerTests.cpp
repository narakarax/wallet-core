// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "uint256.h"
#include "proto/Zilliqa.pb.h"
#include "../interface/TWTestUtilities.h"
#include <TrustWalletCore/TWAnySigner.h>
#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Zilliqa;

TEST(TWAnySignerZilliqa, Sign) {
    auto input = Proto::SigningInput();
    auto &tx = *input.mutable_transaction();
    auto &transfer = *tx.mutable_transfer();
    auto key = parse_hex("0x68ffa8ec149ce50da647166036555f73d57f662eb420e154621e5f24f6cf9748");
    auto amount = store(uint256_t(1000000000000));
    auto gasPrice = store(uint256_t(1000000000));

    input.set_version(65537);
    input.set_nonce(2);
    input.set_to("zil10lx2eurx5hexaca0lshdr75czr025cevqu83uz");
    input.set_gas_price(gasPrice.data(), gasPrice.size());
    input.set_gas_limit(1);
    input.set_private_key(key.data(), key.size());
    transfer.set_amount(amount.data(), amount.size());

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeZilliqa);

    EXPECT_EQ(hex(output.signature()), "001fa4df08c11a4a79e96e69399ee48eeecc78231a78b0355a8ca783c77c139436e37934fecc2252ed8dac00e235e22d18410461fb896685c4270642738ed268");
    EXPECT_EQ(hex(output.json()), "7b22616d6f756e74223a2231303030303030303030303030222c22636f6465223a22222c2264617461223a22222c226761734c696d6974223a2231222c226761735072696365223a2231303030303030303030222c226e6f6e6365223a322c227075624b6579223a22303366623330623139366365336539373635393365636332646132323064636139636465613863383464323337333737303034326139333062383932616330663563222c227369676e6174757265223a223030316661346466303863313161346137396539366536393339396565343865656563633738323331613738623033353561386361373833633737633133393433366533373933346665636332323532656438646163303065323335653232643138343130343631666238393636383563343237303634323733386564323638222c22746f41646472223a2237464363614366303636613546323645653341466663324544314641393831304465616136333243222c2276657273696f6e223a36353533377d");
}

TEST(TWAnySignerZilliqa, SignJSON) {
    auto json = STRING(R"({"version":65537,"nonce":"2","to":"zil10lx2eurx5hexaca0lshdr75czr025cevqu83uz","gasPrice":"O5rKAA==","gasLimit":"1","transaction":{"transfer":{"amount":"6NSlEAA="}}})");
    auto key = DATA("0x68ffa8ec149ce50da647166036555f73d57f662eb420e154621e5f24f6cf9748");
    auto result = WRAPS(TWAnySignerSignJSON(json.get(), key.get(), TWCoinTypeZilliqa));

    ASSERT_TRUE(TWAnySignerSupportsJSON(TWCoinTypeZilliqa));
    assertStringsEqual(result, "7b22616d6f756e74223a2231303030303030303030303030222c22636f6465223a22222c2264617461223a22222c226761734c696d6974223a2231222c226761735072696365223a2231303030303030303030222c226e6f6e6365223a322c227075624b6579223a22303366623330623139366365336539373635393365636332646132323064636139636465613863383464323337333737303034326139333062383932616330663563222c227369676e6174757265223a223030316661346466303863313161346137396539366536393339396565343865656563633738323331613738623033353561386361373833633737633133393433366533373933346665636332323532656438646163303065323335653232643138343130343631666238393636383563343237303634323733386564323638222c22746f41646472223a2237464363614366303636613546323645653341466663324544314641393831304465616136333243222c2276657273696f6e223a36353533377d");
}
