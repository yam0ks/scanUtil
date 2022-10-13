#pragma once

#include "AnalyzeTask.h"

#include <unordered_set>
#include <future>
#include <queue>
#include <mutex>

class Scanner
{
using TaskWithId = std::pair<std::future<std::string>, uint64_t>;

public:
    Scanner(const std::vector<std::string>& groups, uint64_t threadCount = 0);
    ~Scanner();

    uint64_t addTask(const AnalyzeTask& task);

    void wait(uint64_t taskId);
    std::unordered_map<std::string, int> waitResult();

private:
    void run();

private:
    std::unordered_map<std::string, int> m_result;

    std::mutex m_tasksMutex;
    std::condition_variable m_tasksCV;

    std::queue<TaskWithId> m_tasksQueue;
    std::vector<std::thread> m_threads;

    std::mutex m_resultMutex;
    std::condition_variable m_resultCV;

    std::unordered_set<uint64_t> m_completedTasks;

    bool m_finished = false;
    uint64_t m_taskId = 0;
};
