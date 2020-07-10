#pragma once

#include <functional>
#include <sstream>

template<int level>
class EwasLogger
{
public:
	template<class logger_t>
	static void configure(logger_t logger) { log = logger; }

	static bool enabled() { return bool(log); }

	~EwasLogger() { log(stream.str()); }

	template<class value_t>
	EwasLogger &operator <<(value_t value) { stream << value; return *this; }

private:
	static std::function<void(const std::string &)> log;

	std::ostringstream stream;
};

enum LogLevel { LOG_LEVEL_DEBUG, LOG_LEVEL_INFO, LOG_LEVEL_WARNING, LOG_LEVEL_ERROR, LOG_LEVEL_FATAL };

#define WAS_LOG_DEBUG(message) if(!EwasLogger<LOG_LEVEL_DEBUG>::enabled()) {} else EwasLogger<LOG_LEVEL_DEBUG>() << message
#define WAS_LOG_INFO(message) if(!EwasLogger<LOG_LEVEL_INFO>::enabled()) {} else EwasLogger<LOG_LEVEL_INFO>() << message
#define WAS_LOG_WARN(message) if(!EwasLogger<LOG_LEVEL_WARNING>::enabled()) {} else EwasLogger<LOG_LEVEL_WARNING>() << message
#define WAS_LOG_ERROR(message) if(!EwasLogger<LOG_LEVEL_ERROR>::enabled()) {} else EwasLogger<LOG_LEVEL_ERROR>() << message
#define WAS_LOG_FATAL(message) if(!EwasLogger<LOG_LEVEL_FATAL>::enabled()) {} else EwasLogger<LOG_LEVEL_FATAL>() << message
