/*
 * This file is part of the Firelands Core Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
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

#include "Config.h"
#include "Log.h"
#include "StringFormat.h"
#include "StringConvert.h"
#include "Tokenize.h"
#include "Util.h"
#include <fstream>
#include <mutex>
#include <unordered_map>

namespace
{
    std::string _filename;
    std::vector<std::string> _additonalFiles;
    std::vector<std::string> _args;
    std::unordered_map<std::string /*name*/, std::string /*value*/> _configOptions;
    std::mutex _configLock;
    bool _usingDistConfig = false;

    // Check logging system configs like Appender.* and Logger.*
    bool IsLoggingSystemOptions(std::string_view optionName)
    {
        size_t foundAppender = optionName.find("Appender.");
        size_t foundLogger = optionName.find("Logger.");

        return foundAppender != std::string_view::npos || foundLogger != std::string_view::npos;
    }


    void AddKey(std::string const& optionName, std::string const& optionKey, std::string_view fileName, bool isOptional, [[maybe_unused]] bool isReload)
    {
        auto const& itr = _configOptions.find(optionName);

        // Check old option
        if (isOptional && itr == _configOptions.end())
        {
            if (!IsLoggingSystemOptions(optionName) && !isReload)
            {
                std::string fileNameStr = std::string{ fileName };
                LOG_ERROR(fileNameStr, "> Config::LoadFile: Found incorrect option '%s' in config file '%s'. Skip", optionName, fileNameStr);

#ifdef CONFIG_ABORT_INCORRECT_OPTIONS
                ABORT("> Core can't start if found incorrect options");
#endif

                return;
            }
        }

        // Check exit option
        if (itr != _configOptions.end())
        {
            _configOptions.erase(optionName);
        }

        _configOptions.emplace(optionName, optionKey);
    }

    bool ParseFile(std::string const& file, bool isOptional, bool isReload)
    {
        std::ifstream in(file);

        if (in.fail())
        {
            if (isOptional)
            {
                // No display erorr if file optional
                return false;
            }

            std::string message = "Config::LoadFile: Failed open " + std::string(isOptional ? "optional " : "") + " file '" + file + "'";
            throw ConfigException(message);
        }

        uint32 count = 0;
        uint32 lineNumber = 0;
        std::unordered_map<std::string /*name*/, std::string /*value*/> fileConfigs;

        auto IsDuplicateOption = [&](std::string const& confOption)
        {
            auto const& itr = fileConfigs.find(confOption);
            if (itr != fileConfigs.end())
            {
                LOG_ERROR(file, "> Config::LoadFile: Dublicate key name '%s' in config file '%s'", confOption, file);
                return true;
            }

            return false;
        };

        while (in.good())
        {
            lineNumber++;
            std::string line;
            std::getline(in, line);

            // read line error
            if (!in.good() && !in.eof())
            {
                std::string msg = "> Config::LoadFile: Failure to read line number " + std::to_string(lineNumber) + " in file '" + std::string(file) + "'";
                throw ConfigException(msg);
            }

            // remove whitespace in line
            line = Firelands::String::Trim(line, in.getloc());

            if (line.empty())
            {
                continue;
            }

            // comments
            if (line[0] == '#' || line[0] == '[')
            {
                continue;
            }

            size_t found = line.find_first_of('#');
            if (found != std::string::npos)
            {
                line = line.substr(0, found);
            }

            auto const equal_pos = line.find('=');

            if (equal_pos == std::string::npos || equal_pos == line.length())
            {
                LOG_ERROR(file, "> Config::LoadFile: Failure to read line number %s in file '%s'. Skip this line", lineNumber, file);
                continue;
            }

            auto entry = Firelands::String::Trim(line.substr(0, equal_pos), in.getloc());
            auto value = Firelands::String::Trim(line.substr(equal_pos + 1, std::string::npos), in.getloc());

            value.erase(std::remove(value.begin(), value.end(), '"'), value.end());

            // Skip if 2+ same options in one config file
            if (IsDuplicateOption(entry))
            {
                continue;
            }

            // Add to temp container
            fileConfigs.emplace(entry, value);
            count++;
        }

        // No lines read
        if (!count)
        {
            if (isOptional)
            {
                // No display erorr if file optional
                return false;
            }

            std::string msg = "Config::LoadFile: Empty file '" + file + "'";
            throw ConfigException(msg);
        }

        // Add correct keys if file load without errors
        for (auto const& [entry, key] : fileConfigs)
        {
            AddKey(entry, key, file, isOptional, isReload);
        }

        return true;
    }

    bool LoadFile(std::string const& file, bool isOptional, bool isReload)
    {
        try
        {
            return ParseFile(file, isOptional, isReload);
        }
        catch (const std::exception& e)
        {
            LOG_ERROR(file, "> %s", e.what());
        }

        return false;
    }
}

bool ConfigMgr::LoadInitial(std::string const& file, bool isReload /*= false*/)
{
    std::lock_guard<std::mutex> lock(_configLock);
    _configOptions.clear();
    return LoadFile(file, false, isReload);
}

bool ConfigMgr::LoadAdditionalFile(std::string file, bool isOptional /*= false*/, bool isReload /*= false*/)
{
    std::lock_guard<std::mutex> lock(_configLock);
    return LoadFile(file, isOptional, isReload);
}

ConfigMgr* ConfigMgr::instance()
{
    static ConfigMgr instance;
    return &instance;
}

bool ConfigMgr::Reload()
{
    if (!LoadAppConfigs(true))
    {
        return false;
    }

    return LoadModulesConfigs(true, false);
}

template<class T>
T ConfigMgr::GetValueDefault(std::string const& name, T const& def, bool showLogs /*= true*/) const
{
    auto const& itr = _configOptions.find(name);
    if (itr == _configOptions.end())
    {
        if (showLogs)
        {
            LOG_ERROR("server.loading", "> Config: Missing property %s in all config files, at least the .dist file must contain: \"%s = %s\"",
                name, name, Firelands::ToString(def));
        }

        return def;
    }

    auto value = Firelands::StringTo<T>(itr->second);
    if (!value)
    {
        if (showLogs)
        {
            LOG_ERROR("server.loading", "> Config: Bad value defined for name '%s', going to use '%s' instead",
                name, Firelands::ToString(def));
        }

        return def;
    }

    return *value;
}

template<>
std::string ConfigMgr::GetValueDefault<std::string>(std::string const& name, std::string const& def, bool showLogs /*= true*/) const
{
    auto const& itr = _configOptions.find(name);
    if (itr == _configOptions.end())
    {
        if (showLogs)
        {
            LOG_ERROR("server.loading", "> Config: Missing option %s, add \"%s = %s\"", name, name, def);
        }

        return def;
    }

    return itr->second;
}

template<class T>
T ConfigMgr::GetOption(std::string const& name, T const& def, bool showLogs /*= true*/) const
{
    return GetValueDefault<T>(name, def, showLogs);
}

template<>
bool ConfigMgr::GetOption<bool>(std::string const& name, bool const& def, bool showLogs /*= true*/) const
{
    std::string val = GetValueDefault(name, std::string(def ? "1" : "0"), showLogs);

    auto boolVal = Firelands::StringTo<bool>(val);
    if (!boolVal)
    {
        if (showLogs)
        {
            LOG_ERROR("server.loading", "> Config: Bad value defined for name '%s', going to use '%s' instead",
                name, def ? "true" : "false");
        }

        return def;
    }

    return *boolVal;
}

std::vector<std::string> ConfigMgr::GetKeysByString(std::string const& name)
{
    std::lock_guard<std::mutex> lock(_configLock);

    std::vector<std::string> keys;

    for (auto const& [optionName, key] : _configOptions)
    {
        if (!optionName.compare(0, name.length(), name))
        {
            keys.emplace_back(optionName);
        }
    }

    return keys;
}

std::string const ConfigMgr::GetFilename()
{
    std::lock_guard<std::mutex> lock(_configLock);
    return _usingDistConfig ? _filename + ".dist" : _filename;
}

std::vector<std::string> const& ConfigMgr::GetArguments() const
{
    return _args;
}

std::string const ConfigMgr::GetConfigPath()
{
    std::lock_guard<std::mutex> lock(_configLock);

#if AC_PLATFORM == AC_PLATFORM_WINDOWS
    return "configs/";
#else
    return std::string(_CONF_DIR) + "/";
#endif
}

void ConfigMgr::Configure(std::string const& initFileName, std::vector<std::string> args, std::string_view modulesConfigList /*= {}*/)
{
    _filename = initFileName;
    _args = std::move(args);

    // Add modules config if exist
    if (!modulesConfigList.empty())
    {
        for (auto const& itr : Firelands::Tokenize(modulesConfigList, ',', false))
        {
            _additonalFiles.emplace_back(itr);
        }
    }
}

bool ConfigMgr::LoadAppConfigs(bool isReload /*= false*/)
{
    // #1 - Load init config file .conf.dist
    if (!LoadInitial(_filename + ".dist", isReload))
    {
        LOG_ERROR("server.loading", "> Config::LoadAppsConfig: .dist file not found.");
        return false;
    }

    // #2 - Load .conf file
    if (!LoadAdditionalFile(_filename, true, isReload))
    {
        _usingDistConfig = true;
    }

    return true;
}

bool ConfigMgr::LoadModulesConfigs(bool isReload /*= false*/, bool isNeedPrintInfo /*= true*/)
{
    if (_additonalFiles.empty())
    {
        // Send successful load if no found files
        return true;
    }

    if (isNeedPrintInfo)
    {
        LOG_INFO("server.loading", " ");
        LOG_INFO("server.loading", "Loading modules configuration...");
    }

    // Start loading module configs
    std::string const& moduleConfigPath = GetConfigPath() + "modules/";
    bool isExistDefaultConfig = true;
    bool isExistDistConfig = true;

    for (auto const& distFileName : _additonalFiles)
    {
        std::string defaultFileName = distFileName;

        if (!defaultFileName.empty())
        {
            defaultFileName.erase(defaultFileName.end() - 5, defaultFileName.end());
        }

        // Load .conf.dist config
        isExistDistConfig = LoadAdditionalFile(moduleConfigPath + distFileName, false, isReload);

        if (!isReload && !isExistDistConfig)
        {
            LOG_FATAL("server.loading", "> ConfigMgr::LoadModulesConfigs: Not found original config '%s'. Stop loading", distFileName);
            ABORT();
        }

        // Load .conf config
        isExistDefaultConfig = LoadAdditionalFile(moduleConfigPath + defaultFileName, true, isReload);

        if (isExistDefaultConfig && isExistDistConfig)
        {
            _moduleConfigFiles.emplace_back(defaultFileName);
        }
        else if (!isExistDefaultConfig && isExistDistConfig)
        {
            _moduleConfigFiles.emplace_back(distFileName);
        }
    }

    if (isNeedPrintInfo)
    {
        if (!_moduleConfigFiles.empty())
        {
            // Print modules configurations
            LOG_INFO("server.loading", " ");
            LOG_INFO("server.loading", "Using modules configuration:");

            for (auto const& itr : _moduleConfigFiles)
            {
                LOG_INFO("server.loading", "> %s", itr);
            }
        }
        else
        {
            LOG_INFO("server.loading", "> Not found modules config files");
        }
    }

    if (isNeedPrintInfo)
    {
        LOG_INFO("server.loading", " ");
    }

    return true;
}

// @deprecated DO NOT USE - use GetOption<std::string> instead.
std::string ConfigMgr::GetStringDefault(std::string const& name, const std::string& def, bool showLogs /*= true*/)
{
    return GetOption<std::string>(name, def, showLogs);
}

// @deprecated DO NOT USE - use GetOption<bool> instead.
bool ConfigMgr::GetBoolDefault(std::string const& name, bool def, bool showLogs /*= true*/)
{
    return GetOption<bool>(name, def, showLogs);
}

// @deprecated DO NOT USE - use GetOption<int32> instead.
int ConfigMgr::GetIntDefault(std::string const& name, int def, bool showLogs /*= true*/)
{
    return GetOption<int32>(name, def, showLogs);
}

// @deprecated DO NOT USE - use GetOption<float> instead.
float ConfigMgr::GetFloatDefault(std::string const& name, float def, bool showLogs /*= true*/)
{
    return GetOption<float>(name, def, showLogs);
}

#define TEMPLATE_CONFIG_OPTION(__typename) \
    template __typename ConfigMgr::GetOption<__typename>(std::string const& name, __typename const& def, bool showLogs /*= true*/) const;

TEMPLATE_CONFIG_OPTION(std::string)
TEMPLATE_CONFIG_OPTION(uint8)
TEMPLATE_CONFIG_OPTION(int8)
TEMPLATE_CONFIG_OPTION(uint16)
TEMPLATE_CONFIG_OPTION(int16)
TEMPLATE_CONFIG_OPTION(uint32)
TEMPLATE_CONFIG_OPTION(int32)
TEMPLATE_CONFIG_OPTION(uint64)
TEMPLATE_CONFIG_OPTION(int64)
TEMPLATE_CONFIG_OPTION(float)

#undef TEMPLATE_CONFIG_OPTION
