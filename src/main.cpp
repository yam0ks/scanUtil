#include "Scanner.h"
#include "TipParser.h"

#include <iostream>
#include <filesystem>
#include <fstream>
#include <algorithm>

using namespace std;

using Entry = std::filesystem::directory_entry;
using ReportsMap = unordered_map<string, int>;

void executeScanning(const string& dirPath);

TipMap initTipFile(const string& path);
vector<string> groupsNames(const TipMap& groups);

bool fillTasks(const string& dirPath, Scanner& scanner, const TipMap& rules, const TipMap& groups);

void printReport(const ReportsMap& reports, const vector<string>& groupsNames, uint64_t msTime);

int main(int argc, char** argv)
{
    if (argc == 1)
        cout << "\nExpected dir path\n";
    else if (argc > 2)
        cout << "\nExpected only dir path\n";
    else
        executeScanning(argv[1]);

    return 0;
}

void executeScanning(const string& dirPath)
{
    if (!filesystem::is_directory(filesystem::path(dirPath)))
    {
        cout << "\nWrong dir path\n";
        return;
    }

    const auto benchmarkStart = chrono::high_resolution_clock::now();

    TipMap rules = initTipFile("rules.tip");
    TipMap groups = initTipFile("groups.tip");

    vector<string> names = groupsNames(groups);

    Scanner scanner(names);

    if (!fillTasks(dirPath, scanner, rules, groups))
        return;

    const ReportsMap reports = scanner.waitResult();

    const auto benchmarkEnd = chrono::high_resolution_clock::now();
    const auto duration = chrono::duration_cast<chrono::milliseconds>(benchmarkEnd - benchmarkStart);

    printReport(reports, names, duration.count());
}

TipMap initTipFile(const string& path)
{
    ifstream file(path, ios_base::in);

    if (!file.is_open())
        return {};

    stringstream fileStream;
    fileStream << file.rdbuf();

    return TipParser::parse(fileStream.str(), "###");
}

vector<string> groupsNames(const TipMap& groups)
{
    vector<string> result;
    result.reserve(groups.size() + 2);

    result.push_back("Processed");

    transform(begin(groups), end(groups), back_inserter(result), [](const auto& group){
       return group.first;
    });

    result.push_back("Errors");

    return result;
}

bool fillTasks(const string& dirPath, Scanner& scanner, const TipMap& rules, const TipMap& groups)
{
    try
    {
        for (Entry entry : filesystem::directory_iterator(dirPath))
            if (entry.status().type() == filesystem::file_type::regular)
                scanner.addTask(AnalyzeTask{entry.path().string(), rules, groups});
    }

    catch (exception&)
    {
        cout << "\nUnable to open directory\n";
        return false;
    }

    return true;
}

void printReport(const ReportsMap& reports, const vector<string>& groupsNames, uint64_t msTime)
{
    cout << "\n\n====== Scan result ======\n\n";

    for (const auto& group : groupsNames)
    {
        if (reports.find(group) == reports.end())
            continue;

        int groupReportsCount = reports.at(group);

        cout << group;

        if (group == "Processed")
            cout << " files";

        else if (group != "Errors")
            cout << " detects";

        cout << ": " << groupReportsCount << "\n\n";
    }

    cout << "Exection time: "   << msTime << " ms\n\n";
    cout << "=========================\n\n";
}
