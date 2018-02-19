/*
 * Logger.cpp
 *
 *  Created on: 30 août 2017
 *      Author: valen
 */

#include <string>
#include <util/Logger.h>
#include <util/TimeManager.h>

void NLog(const std::string& message)
{
	std::cout << message;
}

void Log(LogLevel level, const std::string& message)
{
	static const std::string LOG_LEVEL_NAMES[] = { "INFO", "WARNING", "ERROR", "FATAL", "DEBUG" };

	struct tm* time = TimeManager::getTimeOfDay();
	std::cout << "[" << time->tm_hour << ":" << time->tm_min << ":" << time->tm_sec << "] " << LOG_LEVEL_NAMES[level] << ": " << message << std::endl;
}

void Info(const std::string& message)
{
	Log(LogLevel::LL_INFO, message);
}

void Warning(const std::string& message)
{
	Log(LogLevel::LL_WARNING, message);
}

void Error(const std::string& message)
{
	Log(LogLevel::LL_ERROR, message);
}

void Fatal(const std::string& message)
{
	Log(LogLevel::LL_FATAL, message);
}

void Debug(const std::string& message)
{
	Log(LogLevel::LL_DEBUG, message);
}
