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
        void write(Level level, std::ostringstream&& buffer);
}

#define LOG_TRACE(x)    amber::log::write(amber::log::trace,    std::ostringstream() << "[TRC] " << x);
#define LOG_DEBUG(x)    amber::log::write(amber::log::debug,    std::ostringstream() << "[DBG] " << x);
#define LOG_INFO(x)     amber::log::write(amber::log::info,     std::ostringstream() << "[INF] " << x);
#define LOG_WARN(x)     amber::log::write(amber::log::warn,     std::ostringstream() << "[WRN] " << x);
#define LOG_ERROR(x)    amber::log::write(amber::log::error,    std::ostringstream() << "[ERR] " << x);
#define LOG_FATAL(x)    amber::log::write(amber::log::fatal,    std::ostringstream() << "[FTL] " << x);

#endif
