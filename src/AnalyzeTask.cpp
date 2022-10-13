#ifdef _WIN32
    #include "WMappedFile.h"
#elif __unix
    #include "LMappedFile.h"
#else
    #error Only Windows/Unix OS supported
#endif

#include "AnalyzeTask.h"
#include <filesystem>
#include <string_view>
#include <algorithm>

using namespace std;

AnalyzeTask::AnalyzeTask(const string& filePath, const TipMap& rules, const TipMap& groups)
    : m_filePath(filePath), m_rules(rules), m_groups(groups)
{}

string AnalyzeTask::operator() () const
{
    return checkForSuspicious();
}

string AnalyzeTask::checkForSuspicious() const
{
    const MappedFile file(m_filePath);

    if (!file.isOpen())
        return "Errors";

    const string_view fileText(file.data(), file.size());

    const vector<string>groupsList = groupsListBy(filesystem::path(m_filePath).extension().string());

    for (const auto& groupName : groupsList)
    {
        if (m_groups.find(groupName) == m_groups.end())
            continue;

        const auto& groupPatterns = m_groups.at(groupName);

        auto patternIt = find_if(begin(groupPatterns), end(groupPatterns), [fileText](const string& pattern){
           return  fileText.find(pattern) != string::npos;
        });

        if (patternIt != end(groupPatterns))
            return groupName;
    }

    return string();
}

std::vector<string> AnalyzeTask::groupsListBy(const string& extension) const
{
    vector<string> groupsNames;

    if (m_rules.find(extension) != m_rules.end())
        groupsNames = m_rules.at(extension);

    else if (m_rules.find("") != m_rules.end())
        groupsNames = m_rules.at("");

    return groupsNames;
}
