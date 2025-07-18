#pragma once
#ifndef CONSOLE_LOG_MSG_HPP
#define CONSOLE_LOG_MSG_HPP

#include <string>
#include <chrono>
#include <mutex>
#include <cstdarg>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <utility>

namespace console_log_msg {

    class Program final {
    public: enum class LogLevel { TRACE, DEBUG, INFO, WARNING, ERROR, FATAL, UNKNOWN };
    private:
        
        class Trace {
        protected:
            virtual ~Trace() = default;
            virtual void trace__(const std::string& msg = "...") const = 0;
            virtual void tracef__(const char* fmt = "...", ...) const = 0;
            template<typename T, typename... Args>
                void ttt(T&& first, Args&&... rest) const {
                    std::ostringstream oss;
                    oss << std::forward<T>(first);
                    ((oss << ' ' << std::forward<Args>(rest)), ...);
                    trace__(oss.str());
                }
        };

        class Debug {
        protected:
            virtual ~Debug() = default;
            virtual void debug__(const std::string& msg = "...") const = 0;
            virtual void debugf__(const char* fmt = "...", ...) const = 0;
            template<typename T, typename... Args>
                void ddd(T&& first, Args&&... rest) const {
                    std::ostringstream oss;
                    oss << std::forward<T>(first);
                    ((oss << ' ' << std::forward<Args>(rest)), ...);
                    debug__(oss.str());
                }
        };

        class Info {
        protected:
            virtual ~Info() = default;
            virtual void info__(const std::string& msg = "...") const = 0;
            virtual void infof__(const char* fmt = "...", ...) const = 0;
            template<typename T, typename... Args>
                void iii(T&& first, Args&&... rest) const {
                    std::ostringstream oss;
                    oss << std::forward<T>(first);
                    ((oss << ' ' << std::forward<Args>(rest)), ...);
                    info__(oss.str());
                }
        };

        class Warning {
        protected:
            virtual ~Warning() = default;
            virtual void warning__(const std::string& msg = "...") const = 0;
            virtual void warningf__(const char* fmt = "...", ...) const = 0;
            template<typename T, typename... Args>
                void www(T&& first, Args&&... rest) const {
                    std::ostringstream oss;
                    oss << std::forward<T>(first);
                    ((oss << ' ' << std::forward<Args>(rest)), ...);
                    warning__(oss.str());
                }
        };

        class Error {
        protected:
            virtual ~Error() = default;
            virtual void error__(const std::string& msg = "...") const = 0;
            virtual void errorf__(const char* fmt = "...", ...) const = 0;
            template<typename T, typename... Args>
                void eee(T&& first, Args&&... rest) const {
                    std::ostringstream oss;
                    oss << std::forward<T>(first);
                    ((oss << ' ' << std::forward<Args>(rest)), ...);
                    error__(oss.str());
                }
        };

        class Fatal {
        protected:
            virtual ~Fatal() = default;
            virtual void fatal__(const std::string& msg = "...") const = 0;
            virtual void fatalf__(const char* fmt = "...", ...) const = 0;
            template<typename T, typename... Args>
                void fff(T&& first, Args&&... rest) const {
                    std::ostringstream oss;
                    oss << std::forward<T>(first);
                    ((oss << ' ' << std::forward<Args>(rest)), ...);
                    fatal__(oss.str());
                }
        };

    public:
        class Logger final :
            private Trace, private Debug, private Info, private Warning, private Error, private Fatal {
        private:
            LogLevel currentLevel;
            std::string logFormat;
            mutable std::mutex logMutex;
            
            inline std::string getTimestamp() const;
            inline std::string levelToString(LogLevel level = LogLevel::UNKNOWN) const;
            inline std::string getColorCode(LogLevel level = LogLevel::UNKNOWN) const;

            void replaceAll(std::string& str, const std::string& from, const std::string& to) const;
            void log(LogLevel level, const std::string& message) const;
            std::string vformat(const char* fmt, va_list args) const;

            Logger();
            ~Logger() override;

        public:
            
            static Logger& getInstance();
            void setLogLevel(LogLevel level = LogLevel::UNKNOWN);
            LogLevel getLogLevel() const;
            void setFormat(const std::string& fmt);
            void logException(const std::exception& e, LogLevel level = LogLevel::ERROR) const;
            void logWithDuration(const std::string& message, 
                                std::chrono::milliseconds duration,
                                LogLevel level = LogLevel::INFO) const;

            // Implementações das interfaces
            void trace__(const std::string& msg = "...") const override;
            void debug__(const std::string& msg = "...") const override;
            void info__(const std::string& msg = "...") const override;
            void warning__(const std::string& msg = "...") const override;
            void error__(const std::string& msg = "...") const override;
            void fatal__(const std::string& msg = "...") const override;
            
            void tracef__(const char* fmt = "...", ...) const override;
            void debugf__(const char* fmt = "...", ...) const override;
            void infof__(const char* fmt = "...", ...) const override;
            void warningf__(const char* fmt = "...", ...) const override;
            void errorf__(const char* fmt = "...", ...) const override;
            void fatalf__(const char* fmt = "...", ...) const override;

            template<typename T, typename... Args> void tracet__(T&& t, Args&&... args) 
            const {
                ttt(std::forward<T>(t), std::forward<Args>(args)...);
            };
            template<typename T, typename... Args> void debugt__(T&& t, Args&&... args) 
            const {
                ddd(std::forward<T>(t), std::forward<Args>(args)...);
            };
            template<typename T, typename... Args> void infot__(T&& t, Args&&... args) 
            const {
                iii(std::forward<T>(t), std::forward<Args>(args)...);
            };
            template<typename T, typename... Args> void warningt__(T&& t, Args&&... args) 
            const {
                www(std::forward<T>(t), std::forward<Args>(args)...);
            };
            template<typename T, typename... Args> void errort__(T&& t, Args&&... args) 
            const {
                eee(std::forward<T>(t), std::forward<Args>(args)...);
            };
            template<typename T, typename... Args> void fatalt__(T&& t, Args&&... args) 
            const {
                fff(std::forward<T>(t), std::forward<Args>(args)...);
            };
        };
    };
}; // namespace console_log_msg

#endif // CONSOLE_LOG_MSG_HPP
