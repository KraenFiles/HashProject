#include <gtest/gtest.h>

#include <iostream>
#include <fstream>

#include "HashTable.h"

class HashTableTest : public testing::Test
{
protected:
    HashTableTest()
        : key("V9832N"), table()
    {
    }

    ~HashTableTest()
    {
    }

    void SetUp() override
    {
    }

    void TearDown() override
    {
    }

    string key;
    HashTable<int> table;
};

TEST_F(HashTableTest, TestAddSuccess)
{
    table.Add(key, 2);

    ASSERT_EQ(table.At(key), 2);
}

TEST_F(HashTableTest, TestWriteInFile)
{
    table.Add(key, 2);
    string text = "Segment num\tCount\n";
    text.append(std::format("{}\t{}\n", Hash::HashСalculation(key), 1));
    string fileText = "./files/test.csv";

    table.LoadInFile(fileText);

    std::ifstream file(fileText);
    std::ostringstream buffer;
    buffer << file.rdbuf();
    ASSERT_EQ(text, buffer.str());
}

TEST_F(HashTableTest, TestResolveCollision)
{
    uint16_t hash = Hash::HashСalculation(key);
    uint16_t hash2 = Hash::HashСalculationForCollision(key);
    uint16_t position = (hash + hash2) % TABLE_SIZE;
    table.Add(key, 2);
    table.Add(key, 4);

    ASSERT_EQ(table.At(position), 4);
}

TEST_F(HashTableTest, TestDeleteSuccess)
{
    table.Add(key, 2);

    table.Delete(key);

    ASSERT_THROW(table.At(key), std::out_of_range::exception);
}

TEST_F(HashTableTest, TestDeleteResolveCollision)
{
    uint16_t hash = Hash::HashСalculation(key);
    uint16_t hash2 = Hash::HashСalculationForCollision(key);
    uint16_t position = (hash + hash2) % TABLE_SIZE;
    table.Add(key, 2);
    table.Add(key, 4);

    table.Delete(key);

    ASSERT_THROW(table.At(key), std::overflow_error::exception);
}

TEST_F(HashTableTest, TestFilledExeption)
{
    for (int i = 0; i < TABLE_SIZE; ++i)
    {
        table.Add(i, 16);
    }

    ASSERT_THROW(table.Add(2, 16), std::out_of_range::exception);
}

TEST_F(HashTableTest, TestClearTable)
{
    table.Add(key, 2);
    table.Add(key, 4);

    table.ClearAll();

    ASSERT_TRUE(table.IsEmpty());
}
