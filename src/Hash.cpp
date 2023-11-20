#include "Hash.h"

#include <random>
#include <iostream>
#include <regex>

string Hash::KeyGenerate()
{
    string result;
    std::random_device rand;
    std::default_random_engine rand_engine(rand());
    std::uniform_int_distribution<int> chars_dist(65, 90);
    std::uniform_int_distribution<int> number_dist(48, 57);

    result.reserve(6);
    result = "XXXXXX";

    result.at(0) = chars_dist(rand_engine);
    result.at(1) = number_dist(rand_engine);
    result.at(2) = number_dist(rand_engine);
    result.at(3) = number_dist(rand_engine);
    result.at(4) = number_dist(rand_engine);
    result.at(5) = chars_dist(rand_engine);

    return result;
}

bool Hash::CheckKey(const string &key)
{
    std::regex reg("^[A-Z]{1}\\d{4}[A-Z]{1}$");
    return std::regex_match(key, reg);
}
