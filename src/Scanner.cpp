#include "Scanner.h"

using namespace std;

Scanner::Scanner(const std::vector<string>& groups, uint64_t threadCount)
{
    threadCount = threadCount == 0 ? thread::hardware_concurrency() : threadCount;
    m_threads.reserve(threadCount);

    for (uint64_t i = 0; i < threadCount; ++i)
        m_threads.emplace_back(&Scanner::run, this);

    for (const auto& group : groups)
        m_result.insert({group, 0});
}

Scanner::~Scanner()
{
    m_finished = true;

    for (auto& thread : m_threads)
    {
        m_tasksCV.notify_all();
        thread.join();
    }
}

uint64_t Scanner::addTask(const AnalyzeTask& task)
{
    const uint64_t currentTaskId = ++m_taskId;

    std::lock_guard lg{m_tasksMutex};

    m_tasksQueue.emplace(async(launch::deferred, task), currentTaskId);

    m_tasksCV.notify_one();

    return currentTaskId;
}

void Scanner::wait(uint64_t taskId)
{
    auto ul = unique_lock{m_resultMutex};

    m_resultCV.wait(ul, [this, taskId] {
        return m_completedTasks.find(taskId) != m_completedTasks.end();
    });
}

std::unordered_map<string, int> Scanner::waitResult()
{
    auto ul = unique_lock{m_tasksMutex};

    m_resultCV.wait(ul, [this] {
        lock_guard lg{m_resultMutex};
        return m_tasksQueue.empty() && m_taskId == m_completedTasks.size();
    });

    return m_result;
}

void Scanner::run()
{
    while (!m_finished)
    {
        auto ul = unique_lock{m_tasksMutex};

        m_tasksCV.wait(ul, [this] {return !m_tasksQueue.empty() || m_finished;});

        if (!m_tasksQueue.empty())
        {
            auto taskwithId = move(m_tasksQueue.front());
            m_tasksQueue.pop();

            std::string report = taskwithId.first.get();
            if (!report.empty())
                m_result[report]++;

            m_result["Processed"]++;

            ul.unlock();

            lock_guard lg{m_resultMutex};

            m_completedTasks.insert(taskwithId.second);
            m_resultCV.notify_all();
        }
    }
}
