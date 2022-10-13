#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using TipMap = std::unordered_map<std::string, std::vector<std::string>>;

namespace
{
    static std::unordered_set<std::string> reservedWords = {
        "Processed", "Errors"
    };
}

class TipParser
{

public:
    static TipMap parse(const std::string& input, const std::string& delimiter);

private:
    static std::vector<std::string> split(const std::string& input, const std::string& delimiter);
};

