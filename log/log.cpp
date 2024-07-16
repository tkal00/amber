#include "log.h"
#include <thread>
#include <sstream>
#include <iostream>

amber::log::Level s_currentLevel = amber::log::Level::trace;
std::ostream* globalSink = nullptr;
std::mutex mutex;

void amber::log::setLevel(Level level)
{
        s_currentLevel = level;
}

void amber::log::setGlobalSink(std::ostream &sink)
{
        globalSink = &sink;
}

auto amber::log::getThreadSink() -> std::ostringstream
{
        std::ostringstream buffer;
        return buffer;
}

void write_threaded(amber::log::Level level, std::ostringstream&& buffer)
{
        if (level < s_currentLevel)
                return;
        std::lock_guard<std::mutex> guard(mutex);
        if (!globalSink)
                amber::log::setGlobalSink(std::cout);
        switch (level) {
        case amber::log::trace: *globalSink << "[TRC] "; break;
        case amber::log::debug: *globalSink << "[DBG] "; break;
        case amber::log::info:  *globalSink << "[INF] "; break;
        case amber::log::warn:  *globalSink << "[WRN] "; break;
        case amber::log::error: *globalSink << "[ERR] "; break;
        case amber::log::fatal: *globalSink << "[FAT] "; break;
        default: LOG_ERROR("invalid log level provided");break;
        }
        *globalSink << buffer.str() << '\n';
} 

void amber::log::write(Level level, std::ostringstream&& buffer)
{
        std::thread(write_threaded, level, std::forward<std::ostringstream&&>(buffer)).detach();
}
