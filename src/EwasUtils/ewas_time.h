#pragma once

#include <time.h>

#include <string>

namespace ewas
{
struct time_format
{
	static const std::string simple, iso;
};

struct tm gmtime(time_t ts);

struct tm localtime(time_t ts);

time_t utc_time(const struct tm &tm);

std::string strftime(time_t ts, const std::string &format = time_format::simple);
std::string strftime(const struct tm &tm, const std::string &format = time_format::simple);

}
