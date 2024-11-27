#include "log.h"
#include <thread>
#include <sstream>
#include <iostream>
#include <queue>
#include <condition_variable>

amber::log::Level s_currentLevel = amber::log::Level::trace;
std::condition_variable cv;
std::mutex mutex;
std::ostream* globalSink = nullptr;
std::queue<std::ostringstream> logQueue;

void amber::log::setLevel(Level level)
{
        s_currentLevel = level;
}

void amber::log::setGlobalSink(std::ostream &sink)
{
        globalSink = &sink;
}

void logWriter_threaded()
{
        if (!globalSink)
                amber::log::setGlobalSink(std::cout);
        while (true)
        {
                if (logQueue.empty())
                        continue;
                std::lock_guard lock(mutex);
                std::ostringstream buf(std::move(logQueue.front()));
                logQueue.pop();
                *globalSink << buf.str() << '\n';
        }
}

void amber::log::write(Level level, std::ostringstream&& buffer)
{
        static std::thread logWriter(logWriter_threaded);
        if (level < s_currentLevel)
                return;
        std::lock_guard guard(mutex);
        logQueue.push(std::move(buffer));
}
