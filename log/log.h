#ifndef AMBER_LOG
#define AMBER_LOG
#include <sstream>
#include <source_location>

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

#define LOG_TRACE(x)    do { const auto loc = std::source_location::current(); amber::log::write(amber::log::trace, std::ostringstream() << "[TRC] " << loc.file_name() << '(' << loc.line() << ',' << loc.column() << ") " << loc.function_name() << ": \t" << x); } while (false)
#define LOG_DEBUG(x)    do { const auto loc = std::source_location::current(); amber::log::write(amber::log::debug, std::ostringstream() << "[DBG] " << loc.file_name() << '(' << loc.line() << ',' << loc.column() << ") " << loc.function_name() << ": \t" << x); } while (false)
#define LOG_INFO(x)     do { const auto loc = std::source_location::current(); amber::log::write(amber::log::info,  std::ostringstream() << "[INF] " << loc.file_name() << '(' << loc.line() << ',' << loc.column() << ") " << loc.function_name() << ": \t" << x); } while (false)
#define LOG_WARN(x)     do { const auto loc = std::source_location::current(); amber::log::write(amber::log::warn,  std::ostringstream() << "[WRN] " << loc.file_name() << '(' << loc.line() << ',' << loc.column() << ") " << loc.function_name() << ": \t" << x); } while (false)
#define LOG_ERROR(x)    do { const auto loc = std::source_location::current(); amber::log::write(amber::log::error, std::ostringstream() << "[ERR] " << loc.file_name() << '(' << loc.line() << ',' << loc.column() << ") " << loc.function_name() << ": \t" << x); } while (false)
#define LOG_FATAL(x)    do { const auto loc = std::source_location::current(); amber::log::write(amber::log::fatal, std::ostringstream() << "[FTL] " << loc.file_name() << '(' << loc.line() << ',' << loc.column() << ") " << loc.function_name() << ": \t" << x); } while (false)
#define LOG_ERRNO()     errno << ' ' << '"' << strerror(errno) << '"' << ' '

#endif
