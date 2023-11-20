#include <gtest/gtest.h>

#include <iostream>
#include <array>
#include <numeric>
#include "Hash.h"

TEST(HashTest, TestCheckKeySuccess)
{
    string key = "B5392M";

    bool result = Hash::CheckKey(key);

    ASSERT_TRUE(result);
}

TEST(HashTest, TestCheckKeyFail)
{
    string key = "A0A00A1";

    bool results = Hash::CheckKey(key);

    ASSERT_FALSE(results);
}

TEST(HashTest, TestKeyGenerate)
{
    string key;
    bool result = false;

    key = Hash::KeyGenerate();
    result = Hash::CheckKey(key);

    ASSERT_TRUE(result);
}

TEST(HashTest, TestHashСalculation)
{
    string key = "A1000A";

    uint16_t hash = Hash::HashСalculation(key);

    ASSERT_EQ(hash, 29);
}

TEST(HashTest, HashСalculationForCollision)
{
    string key = "A1000A";

    uint16_t hash = Hash::HashСalculationForCollision(key);

    ASSERT_EQ(hash, 2);
}
