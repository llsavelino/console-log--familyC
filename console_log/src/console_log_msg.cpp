
#include "console_log_msg.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstdlib>
#include <algorithm>

namespace console_log_msg {

    inline std::string Program::Logger::getTimestamp() const {
        try {
            auto now = std::chrono::system_clock::now();
            auto in_time = std::chrono::system_clock::to_time_t(now);
            std::tm tmStruct;
        #ifdef _WIN32
            localtime_s(&tmStruct, &in_time);
        #else
            localtime_r(&in_time, &tmStruct);
        #endif
            std::ostringstream oss;
            oss << std::put_time(&tmStruct, "%Y-%m-%d %H:%M:%S");
            return oss.str();
        } catch (...) {
            return "[timestamp error]";
        }
    }

    inline std::string Program::Logger::getColorCode(LogLevel level) const {
        switch (level) {
            case LogLevel::TRACE:   return "\033[90m";
            case LogLevel::DEBUG:   return "\033[36m";
            case LogLevel::INFO:    return "\033[32m";
            case LogLevel::WARNING: return "\033[33m";
            case LogLevel::ERROR:   return "\033[31m";
            case LogLevel::FATAL:   return "\033[41m\033[37m";
            default:                return "\033[0m";
        }
    }

    Program::Logger::Logger() : currentLevel(LogLevel::INFO), 
                                logFormat("[{timestamp}] [{level}] {message}") {}
    Program::Logger::~Logger() = default;

    inline std::string Program::Logger::levelToString(LogLevel level = LogLevel::UNKNOWN) const {
        switch (level) {
            case LogLevel::TRACE:   return "TRACE";
            case LogLevel::DEBUG:   return "DEBUG";
            case LogLevel::INFO:    return "INFO";
            case LogLevel::WARNING: return "WARNING";
            case LogLevel::ERROR:   return "ERROR";
            case LogLevel::FATAL:   return "FATAL";
            default:                return "UNKNOWN";
        }
    }

    void Program::Logger::replaceAll(std::string& str, const std::string& from, const std::string& to) const {
        if (from.empty()) return;
        size_t start_pos = 0;
        while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length();
        }
    }

    void Program::Logger::log(LogLevel level, const std::string& message) const {
        try {
            if(level < currentLevel) return;

            std::lock_guard<std::mutex> lock(logMutex);
            
            std::string formatted = logFormat;
            replaceAll(formatted, "{timestamp}", getTimestamp());
            replaceAll(formatted, "{level}", levelToString(level));
            replaceAll(formatted, "{message}", message);

            std::cout << getColorCode(level) << formatted << "\033[0m" << std::endl;

            if (level == LogLevel::FATAL) std::exit(EXIT_FAILURE);
        } catch (...) {
            std::cerr << "LOG ERROR: " << message << std::endl;
        }
    }

    std::string Program::Logger::vformat(const char* fmt, va_list args) const {
        try {
            std::vector<char> buffer(1024);
            va_list args_copy; // Create a copy of va_list
            va_copy(args_copy, args); // Copy the va_list
            int len = vsnprintf(buffer.data(), buffer.size(), fmt, args_copy);
            va_end(args_copy); // End the copy

            if (len < 0) return "format error";
            if (len >= static_cast<int>(buffer.size())) {
                buffer.resize(len + 1);
                va_copy(args_copy, args); // Copy again for the second vsnprintf
                vsnprintf(buffer.data(), buffer.size(), fmt, args_copy);
                va_end(args_copy); // End the copy
            }
            return std::string(buffer.data());
        } catch (...) {
            return "formatting error";
        }
    }

    Program::Logger& Program::Logger::getInstance() { 
        static Logger instance; 
        return instance; 
    }

    void Program::Logger::setLogLevel(LogLevel level = LogLevel::UNKNOWN) { 
        std::lock_guard<std::mutex> lock(logMutex);
        currentLevel = level; 
    }
    
    Program::LogLevel Program::Logger::getLogLevel() const { 
        std::lock_guard<std::mutex> lock(logMutex);
        return currentLevel; 
    }

    void Program::Logger::setFormat(const std::string& fmt) { 
        std::lock_guard<std::mutex> lock(logMutex);
        logFormat = fmt; 
    }

    void Program::Logger::logException(const std::exception& e, LogLevel level = LogLevel::ERROR) const {
        try {
            log(level, std::string("Exception: ") + e.what());
        } catch (...) {
            std::cerr << "Failed to log exception: " << e.what() << std::endl;
        }
    }

    void Program::Logger::logWithDuration(const std::string& message, 
                                            std::chrono::milliseconds duration,
                                            LogLevel level = LogLevel::INFO) const 
    {
        try {
            std::ostringstream oss;
            oss << message << " (took " << duration.count() << "ms)";
            log(level, oss.str());
        } catch (...) {
            std::cerr << "Failed to log duration" << std::endl;
        }
    }

    void Program::Logger::trace__(const std::string& msg = "...") const 
    { 
        if (currentLevel <= LogLevel::TRACE) log(LogLevel::TRACE, msg); }
    void Program::Logger::debug__(const std::string& msg = "...") const 
    { 
        if (currentLevel <= LogLevel::DEBUG) log(LogLevel::DEBUG, msg); }
    void Program::Logger::info__(const std::string& msg = "...") const 
    {  
        if (currentLevel <= LogLevel::INFO) log(LogLevel::INFO, msg); }
    void Program::Logger::warning__(const std::string& msg = "...") const 
    { 
        if (currentLevel <= LogLevel::WARNING) log(LogLevel::WARNING, msg); }
    void Program::Logger::error__(const std::string& msg = "...") const 
    { 
        if (currentLevel <= LogLevel::ERROR) log(LogLevel::ERROR, msg); }
    void Program::Logger::fatal__(const std::string& msg = "...") const 
    { 
        if (currentLevel <= LogLevel::FATAL) log(LogLevel::FATAL, msg); }
    
    void Program::Logger::tracef__(const char* fmt = "...", ...) const {
        va_list args; va_start(args, fmt); 
        if (currentLevel <= LogLevel::TRACE) try { trace__(vformat(fmt, args)); } catch (...) {}
        va_end(args);
    }
    void Program::Logger::debugf__(const char* fmt = "...", ...) const {
        va_list args; va_start(args, fmt); 
        if (currentLevel <= LogLevel::DEBUG) try { debug__(vformat(fmt, args)); } catch (...) {}
        va_end(args);
    }
    void Program::Logger::infof__(const char* fmt = "...", ...) const {
        va_list args; va_start(args, fmt); 
        if (currentLevel <= LogLevel::INFO) try { info__(vformat(fmt, args)); } catch (...) {}
        va_end(args);
    }
    void Program::Logger::warningf__(const char* fmt = "...", ...) const {
        va_list args; va_start(args, fmt); 
        if (currentLevel <= LogLevel::WARNING) try { warning__(vformat(fmt, args)); } catch (...) {}
        va_end(args);
    }
    void Program::Logger::errorf__(const char* fmt = "...", ...) const {
        va_list args; va_start(args, fmt); 
        if (currentLevel <= LogLevel::ERROR) try { error__(vformat(fmt, args)); } catch (...) {}
        va_end(args);
    }
    void Program::Logger::fatalf__(const char* fmt = "...", ...) const {
        va_list args; va_start(args, fmt); 
        if (currentLevel <= LogLevel::FATAL) try { fatal__(vformat(fmt, args)); } catch (...) {}
        va_end(args);
    }
} // namespace console_log_msg


