#ifndef HASH_H
#define HASH_H

#include <cinttypes>
#include <string>
#include "Config.h"

using std::string;

namespace Hash
{
    string KeyGenerate();
    constexpr uint16_t HashСalculation(const string &key);
    constexpr uint16_t HashСalculationForCollision(const string &key);
    bool CheckKey(const string &key);
};

constexpr uint16_t Hash::HashСalculation(const string &key)
{
    uint16_t result = 0;
    for (auto ch : key)
    {
        result += static_cast<int>(ch);
    }
    int minSum = (2 * static_cast<int>('A')) + (4 * static_cast<int>('0'));
    int maxSum = (2 * static_cast<int>('Z')) + (4 * static_cast<int>('9'));
    double coefficient = TABLE_SIZE / (maxSum-minSum);
    (result -= minSum) *= coefficient;
    return result;
}

constexpr uint16_t Hash::HashСalculationForCollision(const string &key)
{
    uint16_t result = 0;
    for (auto ch : key)
    {
        result += static_cast<int>(ch);
    }
    int minSum = (2 * static_cast<int>('A')) + (4 * static_cast<int>('0'));
    ++(result -= minSum);
    return result;
}

#endif