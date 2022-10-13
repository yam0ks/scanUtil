#pragma once

#include <unordered_map>
#include <string>
#include <vector>

class AnalyzeTask
{
using TipMap = std::unordered_map<std::string, std::vector<std::string>>;

public:
    explicit AnalyzeTask(const std::string& filePath, const TipMap& rules, const TipMap& groups);
    ~AnalyzeTask() = default;

    std::string operator() () const;

private:
    std::string checkForSuspicious() const;
    std::vector<std::string> groupsListBy(const std::string& extension) const;

private:
    std::string m_filePath;
    TipMap m_rules;
    TipMap m_groups;
};
