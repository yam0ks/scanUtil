#include "TipParser.h"

using namespace std;

TipMap TipParser::parse(const string& input, const string& delimiter)
{
    TipMap result;

    const vector<string> rows = split(input, "\n");

    for (const auto& row : rows)
    {
        vector<string> splitedRow = split(row, delimiter);

        if (splitedRow.empty())
            continue;

        const string key = move(splitedRow.back());

        if (reservedWords.find(key) != reservedWords.end())
            continue;

        splitedRow.pop_back();

        result.insert({key, splitedRow});
    }

    return result;
}

vector<string> TipParser::split(const string& input, const string& delimiter)
{
    vector<string> result;

    if (input.empty())
        return result;

    size_t beginWordIdx = 0;
    size_t endWordIdx = input.find(delimiter);

    while (endWordIdx != string::npos)
    {
        result.push_back(input.substr(beginWordIdx, endWordIdx - beginWordIdx));
        beginWordIdx = endWordIdx + delimiter.size();
        endWordIdx = input.find(delimiter, beginWordIdx);
    }

    if (beginWordIdx <= input.size() && beginWordIdx != 0)
        result.push_back(input.substr(beginWordIdx, input.size() - beginWordIdx));

    return result;
}
