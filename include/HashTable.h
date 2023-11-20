#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <array>
#include <list>
#include <format>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <exception>
#include <algorithm>

#include "Config.h"
#include "Hash.h"

template <typename T>
class HashTable
{
private:
    std::array<T, TABLE_SIZE> values;
    std::array<bool, TABLE_SIZE> emptyField;
    std::array<int, TABLE_SIZE> hitCount;
    std::list<string> badKeys;
    uint16_t elementsCount;

    uint16_t ResolveHashCollision(const string &key, bool added);

public:
    HashTable();
    ~HashTable();

    inline T &At(uint16_t position)
    {
        if (emptyField.at(position))
            std::__throw_out_of_range(
                std::format("Empty field for id: {}", position).c_str());
        return values.at(position);
    }
    inline T &At(const string &key)
    {
        const uint16_t hash = Hash::HashСalculation(key);
        if (emptyField.at(hash))
            std::__throw_out_of_range(
                std::format("Empty field for key: {}", key).c_str());
        return values.at(hash);
    }

    void Add(const string &key, const T &value);
    void Add(uint16_t position, const T &value);
    void Delete(uint16_t position);
    void Delete(const string &key);
    void ClearAll();

    inline bool IsEmpty() { return elementsCount == 0 ? true : false; }
    inline bool FieldIsEmpty(uint16_t position)
    {
        if (position >= TABLE_SIZE)
            std::__throw_out_of_range(
                std::format("Out of range for check field for id: {}", position).c_str());
        return emptyField.at(position);
    }

    void ShowTable();
    void LoadInFile(const std::filesystem::path &filePath);
};

template <typename T>
uint16_t HashTable<T>::ResolveHashCollision(const string &key, bool added)
{
    uint16_t result = 0;
    int i = 0;
    uint16_t hashCode = Hash::HashСalculation(key);
    uint16_t secondHash = Hash::HashСalculationForCollision(key);
    bool checkEmpty = false;
    do
    {
        result = (hashCode + i * secondHash) % TABLE_SIZE;

        if (added)
            checkEmpty = !emptyField.at(result);
        else
            checkEmpty = emptyField.at(result);
        ++hitCount.at(result);
        ++i;
    } while (checkEmpty && i < TABLE_SIZE);

    if (i >= TABLE_SIZE)
    {
        std::__throw_out_of_range("Key out of range");
    }

    return result;
}

template <typename T>
HashTable<T>::HashTable()
    : values(), emptyField(), hitCount(), elementsCount(0)
{
    std::fill(emptyField.begin(), emptyField.end(), true);
    std::fill(hitCount.begin(), hitCount.end(), 0);
}

template <typename T>
HashTable<T>::~HashTable()
{
}

template <typename T>
void HashTable<T>::Add(const string &key, const T &value)
{
    if (elementsCount == TABLE_SIZE)
    {
        std::__throw_overflow_error("Table is overflow");
        return;
    }
    if (std::find(badKeys.begin(), badKeys.end(), key) != std::end(badKeys))
    {
        std::cout << std::format("По этому ключу: {} все ячейки заполнены\n", key);
        return;
    }
    uint16_t position = 0;
    try
    {
        position = ResolveHashCollision(key, true);
    }
    catch (const std::out_of_range::exception &)
    {
        std::cout << std::format("По этому ключу: {} все ячейки заполнены\n", key);
        badKeys.push_back(key);
        return;
    }
    Add(position, value);
}

template <typename T>
void HashTable<T>::Add(uint16_t position, const T &value)
{
    if (elementsCount == TABLE_SIZE)
    {
        std::__throw_overflow_error("Table is overflow");
        return;
    }
    values.at(position) = value;
    emptyField.at(position) = false;
    ++elementsCount;
}

template <typename T>
void HashTable<T>::Delete(uint16_t position)
{
    if (emptyField.at(position))
    {
        std::__throw_out_of_range(
            std::format("Empty field in position: {}", position).c_str());
        return;
    }

    values.at(position) = NULL;
    emptyField.at(position) = true;
    --elementsCount;
}

template <typename T>
void HashTable<T>::Delete(const string &key)
{
    uint16_t position = 0;
    try
    {
        position = ResolveHashCollision(key, false);
    }
    catch (const std::out_of_range::exception &)
    {
        std::cout << std::format("По этому ключу: {} все ячейки пустые\n", key);
        return;
    }

    try
    {
        Delete(position);
    }
    catch (const std::out_of_range::exception &e)
    {
        std::__throw_out_of_range(
            std::format("Empty field in key: {}", key).c_str());
        return;
    }
    auto iter = std::find(badKeys.begin(), badKeys.end(), key);
    if (iter != std::end(badKeys))
    {
        badKeys.remove(key);
    }
}

template <typename T>
void HashTable<T>::ClearAll()
{
    for (int i = 0; i < TABLE_SIZE; ++i)
    {
        if (!emptyField.at(i))
        {
            Delete(i);
        }
    }
}

template <typename T>
void HashTable<T>::ShowTable()
{
    // clang-format off
    std::cout << 
        "---------------------------\n"
        "| Segment num |   Value   |\n"
        "---------------------------\n";
    for(int i = 0; i < TABLE_SIZE; ++i)
    {
        if(!emptyField.at(i))
            std::cout << 
            "|" << std::setw(13) << i << "|" << std::setw(11) << values.at(i) << "|\n" << 
            "---------------------------\n";
    }

    std::cout << std::flush;
    // clang-format on
}

template <typename T>
void HashTable<T>::LoadInFile(const std::filesystem::path &filePath)
{
    std::filesystem::directory_entry directory(filePath.parent_path());

    if (!directory.exists())
        std::filesystem::create_directory(directory.path());

    std::ofstream file(filePath);
    file << "Segment num\tCount\n";

    for (int i = 0; i < TABLE_SIZE; ++i)
    {
        if (!emptyField.at(i))
            file << std::format("{}\t{}\n", i, hitCount.at(i));
    }
    file.close();
}

#endif