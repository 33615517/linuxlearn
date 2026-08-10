#ifndef __LOG_HPP__
#define __LOG_HPP__

#include <iostream>
#include <string>
#include <filesystem>
#include "Mutex.hpp"
#include <fstream>
#include <sstream>
#include <memory>
#include<time.h>
#include <unistd.h>
namespace LogModule
{
    using namespace MutexModule;

    const std::string sep = "\r\n";
    // 策略模式，C++多态特性
    //  2.刷新策略 a:显示器打印 b:文件打印
    //  刷新策略基类
    class LogStrategy
    {
    public:
        ~LogStrategy() = default;
        virtual void SyncLog(const std::string &message) = 0;

    private:
    };

    // 显示器打印策略 ——子类
    class ConsoleLogStrategy : public LogStrategy
    {
    public:
        ConsoleLogStrategy() {};
        ~ConsoleLogStrategy() {};
        void SyncLog(const std::string &message) override
        {
            LockGuard lockguard(_mutex);
            std::cout << message << sep;
        }

    private:
        Mutex _mutex;
    };
    // 文件打印策略 ——子类
    const std::string defaultPath = "./log";
    const std::string defaultFile = "my.log";
    class FileLogStrategy : public LogStrategy
    {
    public:
        FileLogStrategy(const std::string &path = defaultPath, const std::string &file = defaultFile) : _path(path), _file(file)
        {
            LockGuard lockguard(_mutex);
            // 检查路径是否存在，如果不存在则创建
            if (!std::filesystem::exists(_path))
            {
                try
                {
                    std::filesystem::create_directories(_path);
                }
                catch (const std::filesystem::filesystem_error &e)
                {
                    std::cerr << "Error creating directory: " << e.what() << std::endl;
                    throw; // Rethrow the exception to indicate failure
                }
            }
        };
        ~FileLogStrategy() {};
        void SyncLog(const std::string &message) override
        {
            LockGuard lockguard(_mutex);
            // 实现文件打印逻辑
            std::string fullPath = _path + (_path.back() == '/' ? "" : "/") + _file;
            std::ofstream out(fullPath, std::ios::app);
            if (!out.is_open())
            {
                std::cerr << "Error opening log file: " << fullPath << std::endl;
                return;
            }
            out << message << sep;
            out.close();
        }

    private:
        std::string _path; // 文件路径
        std::string _file; // 文件名
        Mutex _mutex;
    };

    // 形成一条完整日志&&根据上面的策略模式，选择不同的刷新策略

    // 1.形成日志等级
    enum class Loglevel
    {
        DEBUG,
        INFO,
        WARNING,
        ERROR,
        FATAL
    };
    std::string Level2Str(Loglevel level)
    {
        switch (level)
        {
        case Loglevel::DEBUG:
            return "DEBUG";
        case Loglevel::INFO:
            return "INFO";
        case Loglevel::WARNING:
            return "WARNING";
        case Loglevel::ERROR:
            return "ERROR";
        case Loglevel::FATAL:
            return "FATAL";
        default:
            return "UNKNOWN";
        }
    }

    // 2.形成时间戳
    std::string GetTimeStamp()
    {
        time_t curr = time(nullptr);
        struct tm curr_tm;
        localtime_r(&curr, &curr_tm);
        char buffer[20];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &curr_tm);
        return std::string(buffer);
    }
    // 1.形成日志&& 2.根据不同的策略，完成刷新
    class Logger
    {
    public:
        Logger()
        {
            EnableConsoleLogStrategy();
        }
        void EnableFileLogStrategy()
        {
            _fflush_strategy = std::make_unique<FileLogStrategy>();
        }
        void EnableConsoleLogStrategy()
        {
            _fflush_strategy = std::make_unique<ConsoleLogStrategy>();
        }

        // 表示的是未来的一条日志
        class LogMessage
        {
        public:
            LogMessage(Loglevel &level, std::string &srcname, int line_number, Logger &logger)
                : _curr_time(GetTimeStamp()),
                  _level(level),
                  _pid(getpid()),
                  _srcname(srcname),
                  _line_number(line_number),
                  _logger(logger)
            {
                // 日志的左半部分,合并起来
                std::ostringstream ss;
                ss << "[" << _curr_time << "]"
                   << "[" << Level2Str(_level) << "]"
                   << "[pid:" << _pid << "]"
                   << "[" << _srcname << "] [" << _line_number << "]"
                   << "- ";
                _loginfo = ss.str();
            }
            template <typename T>
            LogMessage &operator<<(const T &data)
            {
                // 日志的右半部分，可变的，合并起来
                std::ostringstream ss;
                ss << data;
                _loginfo += ss.str();
                return *this;
            }
            ~LogMessage()
            {
                if (_logger._fflush_strategy)
                {
                    _logger._fflush_strategy->SyncLog(_loginfo);
                }
            }

        private:
            std::string _curr_time;
            Loglevel _level;
            pid_t _pid;
            std::string _srcname;
            int _line_number;
            std::string _loginfo; // 合并之后一条完整的消息
            Logger &_logger;
        };
        LogMessage operator()(Loglevel level, std::string src_name, int line_number)
        {
            return LogMessage(level, src_name, line_number, *this);
        }
        ~Logger()
        {
        }

    private:
        std::unique_ptr<LogStrategy> _fflush_strategy;
    };
    //全局日志对象
    Logger logger;

    //使用宏，简化用户操作，获取日志的源文件名和行号
    #define LOG(level) LogModule::logger(level, __FILE__, __LINE__)
    #define Enable_Console_log_strategy() logger.EnableConsoleLogStrategy()
    #define Enable_File_log_strategy() logger.EnableFileLogStrategy()
}

#endif