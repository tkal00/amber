#ifndef AMBER_LOG
#define AMBER_LOG
#include <sstream>

namespace amber::log
{
        enum Level
        {
                trace,
                debug,
                info,
                warn,
                error,
                fatal
        };
        void setLevel(Level level);

        void setGlobalSink(std::ostream& sink);
        auto getThreadSink() -> std::ostringstream;
        void write(Level level, std::ostringstream&& buffer);
}

#define LOG_TRACE(x)    amber::log::write(amber::log::trace, amber::log::getThreadSink() << x);
#define LOG_DEBUG(x)    amber::log::write(amber::log::debug, amber::log::getThreadSink() << x);
#define LOG_INFO(x)     amber::log::write(amber::log::info, amber::log::getThreadSink() << x);
#define LOG_WARN(x)     amber::log::write(amber::log::warn, amber::log::getThreadSink() << x);
#define LOG_ERROR(x)    amber::log::write(amber::log::error, amber::log::getThreadSink() << x);
#define LOG_FATAL(x)    amber::log::write(amber::log::fatal, amber::log::getThreadSink() << x);

#endif
