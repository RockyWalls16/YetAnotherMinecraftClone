/*
 * Logger.hpp
 *
 *  Created on: 30 août 2017
 *      Author: valen
 */

#ifndef UTIL_LOGGER_H_
#define UTIL_LOGGER_H_

#include <iostream>
#include <string>

#define LOG_LINE "-------------------------------------------------------------------------------\n"

enum LogLevel : char
{
	LL_INFO,
	LL_WARNING,
	LL_ERROR,
	LL_FATAL,
	LL_DEBUG
};

void NLog(const std::string& message);

void Log(LogLevel level, const std::string& message);

void Info(const std::string& message);

void Warning(const std::string& message);

void Error(const std::string& message);

void Fatal(const std::string& message);

void Debug(const std::string& message);


#endif /* UTIL_LOGGER_H_ */
