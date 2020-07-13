#include "DDKLogger.h"

template<>
std::function<void(const std::string &)> ddk_logger<LOG_LEVEL_DEBUG>::log = 0;

template<>
std::function<void(const std::string &)> ddk_logger<LOG_LEVEL_INFO>::log = 0;

template<>
std::function<void(const std::string &)> ddk_logger<LOG_LEVEL_WARNING>::log = 0;

template<>
std::function<void(const std::string &)> ddk_logger<LOG_LEVEL_ERROR>::log = 0;

template<>
std::function<void(const std::string &)> ddk_logger<LOG_LEVEL_FATAL>::log = 0;
