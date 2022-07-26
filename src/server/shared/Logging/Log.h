/*
 * Copyright (C) 2008-2014 TrinityCore <http://www.trinitycore.org/>
* Copyright (C) 2022 Firelands Project <https://github.com/FirelandsProject>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef FIRELANDS_LOG_H
#define FIRELANDS_LOG_H

#include "Define.h"
#include "Appender.h"
#include "Logger.h"
#include "LogWorker.h"

#include <unordered_map>
#include <string>
#include <ace/Singleton.h>

#define LOGGER_ROOT "root"

class Log
{
    friend class ACE_Singleton<Log, ACE_Thread_Mutex>;

    typedef std::unordered_map<std::string, Logger> LoggerMap;
    typedef std::unordered_map<std::string, Logger const*> CachedLoggerContainer;

    private:
        Log();
        ~Log();

    public:
        void LoadFromConfig();
        void Close();
        bool ShouldLog(std::string const& type, LogLevel level);
        bool SetLogLevel(std::string const& name, char const* level, bool isLogger = true);

        void outMessage(std::string const& f, LogLevel level, char const* str, ...) ATTR_PRINTF(4, 5);

        void outCommand(uint32 account, const char * str, ...) ATTR_PRINTF(3, 4);
        void outCharDump(char const* str, uint32 account_id, uint32 guid, char const* name);

        void SetRealmId(uint32 id);

    private:
        static std::string GetTimestampStr();
        void vlog(std::string const& f, LogLevel level, char const* str, va_list argptr);
        void write(LogMessage* msg);

        Logger const* GetLoggerByType(std::string const& type);
        Appender* GetAppenderByName(std::string const& name);
        uint8 NextAppenderId();
        void CreateAppenderFromConfig(std::string const& name);
        void CreateLoggerFromConfig(std::string const& name);
        void ReadAppendersFromConfig();
        void ReadLoggersFromConfig();

        AppenderMap appenders;
        LoggerMap loggers;
        CachedLoggerContainer cachedLoggers;
        uint8 AppenderId;

        std::string m_logsDir;
        std::string m_logsTimestamp;

        LogWorker* worker;
};

inline Logger const* Log::GetLoggerByType(std::string const& originalType)
{
    // Check if already cached
    CachedLoggerContainer::const_iterator itCached = cachedLoggers.find(originalType);
    if (itCached != cachedLoggers.end())
        return itCached->second;

    Logger const* logger = NULL;
    std::string type(originalType);

    do
    {
        // Search for the logger "type.subtype"
        LoggerMap::const_iterator it = loggers.find(type);
        if (it == loggers.end())
        {
            // Search for the logger "type", if our logger contains '.', otherwise search for LOGGER_ROOT
            size_t found = type.find_last_of(".");
            type = found != std::string::npos ? type.substr(0, found) : LOGGER_ROOT;
        }
        else
            logger = &(it->second);
    }
    while (!logger);

    cachedLoggers[originalType] = logger;
    return logger;
}

inline bool Log::ShouldLog(std::string const& type, LogLevel level)
{
    Logger const* logger = GetLoggerByType(type);
    if (!logger)
        return false;

    LogLevel logLevel = logger->getLogLevel();
    return logLevel != LOG_LEVEL_DISABLED && logLevel <= level;
}

inline void Log::outMessage(std::string const& filter, LogLevel level, const char * str, ...)
{
    va_list ap;
    va_start(ap, str);

    vlog(filter, level, str, ap);

    va_end(ap);
}

#define sLog ACE_Singleton<Log, ACE_Thread_Mutex>::instance()

#if PLATFORM != PLATFORM_WINDOWS
#define LOG_MESSAGE_BODY(filterType__, level__, ...)                 \
        do {                                                            \
            if (sLog->ShouldLog(filterType__, level__))                 \
                sLog->outMessage(filterType__, level__, __VA_ARGS__);   \
        } while (0)
#else
#define LOG_MESSAGE_BODY(filterType__, level__, ...)                 \
        __pragma(warning(push))                                         \
        __pragma(warning(disable:4127))                                 \
        do {                                                            \
            if (sLog->ShouldLog(filterType__, level__))                 \
                sLog->outMessage(filterType__, level__, __VA_ARGS__);   \
        } while (0)                                                     \
        __pragma(warning(pop))
#endif

#define LOG_TRACE(filterType__, ...) \
    LOG_MESSAGE_BODY(filterType__, LOG_LEVEL_TRACE, __VA_ARGS__)

#define LOG_DEBUG(filterType__, ...) \
    LOG_MESSAGE_BODY(filterType__, LOG_LEVEL_DEBUG, __VA_ARGS__)

#define LOG_INFO(filterType__, ...)  \
    LOG_MESSAGE_BODY(filterType__, LOG_LEVEL_INFO, __VA_ARGS__)

#define LOG_WARN(filterType__, ...)  \
    LOG_MESSAGE_BODY(filterType__, LOG_LEVEL_WARN, __VA_ARGS__)

#define LOG_ERROR(filterType__, ...) \
    LOG_MESSAGE_BODY(filterType__, LOG_LEVEL_ERROR, __VA_ARGS__)

#define LOG_FATAL(filterType__, ...) \
    LOG_MESSAGE_BODY(filterType__, LOG_LEVEL_FATAL, __VA_ARGS__)

#endif
