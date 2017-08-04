//
// Created by root on 11/28/16.
//

#pragma once

#include "spdlog/spdlog.h"
#include "Singleton.h"
#include <iostream>

class Log : public mtl::Singleton<Log>
{
private:
    void init()
    {
        spdlog::set_sync_mode();
        m_pLogConsole = spdlog::stdout_color_mt("console");
//        m_pLogConsole = spdlog::daily_logger_mt("console","log/log_console",9,0);
        m_pLogConsole->set_level(spdlog::level::info);
        m_pLogConsole->set_pattern("[%Y-%m-%d %H:%M:%S.%e] %v");
        spdlog::set_async_mode(4096);
        m_pLogFile = spdlog::daily_logger_mt("testLog","log/log_global");
        m_pLogFile->set_level(spdlog::level::info);
        m_pLogFile->set_pattern("[%Y-%m-%d %H:%M:%S.%e] %v");
    }
public:
    std::shared_ptr<spdlog::logger> getConsole()
    {
        return m_pLogConsole;
    }

    std::shared_ptr<spdlog::logger> getFileLog()
    {
        return m_pLogFile;
    }

    bool setLogLevel(spdlog::level::level_enum level)
    {

        m_pLogConsole->set_level(level);
        m_pLogFile->set_level(level);
        return true;
    }

    void flushLog()
    {
        if(m_pLogFile != nullptr)
        {
            m_pLogFile->flush();
        }
    }

public:
    Log()
    {
        init();
    }

private:
//    static Log *log;
    std::shared_ptr<spdlog::logger> m_pLogConsole;
    std::shared_ptr<spdlog::logger> m_pLogFile;

};

// print console log
#define LOGCONSOLE_TRACE(...) Log::GetInstance().getConsole()->trace(__VA_ARGS__);
#define LOGCONSOLE_DEBUG(...) Log::GetInstance().getConsole()->debug(__VA_ARGS__);
#define LOGCONSOLE_INFO(...) Log::GetInstance().getConsole()->info(__VA_ARGS__);
#define LOGCONSOLE_WARN(...) Log::GetInstance().getConsole()->warn(__VA_ARGS__);
#define LOGCONSOLE_ERROR(...) Log::GetInstance().getConsole()->error(__VA_ARGS__);
// print global file log
#define LOGFILE_TRACE(...) Log::GetInstance().getFileLog()->trace(__VA_ARGS__);
#define LOGFILE_DEBUG(...) Log::GetInstance().getFileLog()->debug(__VA_ARGS__);
#define LOGFILE_INFO(...) Log::GetInstance().getFileLog()->info(__VA_ARGS__);
#define LOGFILE_WARN(...) Log::GetInstance().getFileLog()->warn(__VA_ARGS__);
#define LOGFILE_ERROR(...) Log::GetInstance().getFileLog()->error(__VA_ARGS__);