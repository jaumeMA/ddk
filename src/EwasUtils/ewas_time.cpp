#include "ewas_time.h"

#include <cstring>

const std::string ewas::time_format::simple("%Y-%m-%d %H:%M:%S"), ewas::time_format::iso("%Y-%m-%dT%H:%M:%SZ");

inline struct tm *ewas_gmtime(const time_t *ts, struct tm *tm)
{
#ifdef _WIN32
	return (gmtime_s(tm, ts) == 0)? tm : 0;
#else
	return gmtime_r(ts, tm);
#endif
}

inline struct tm *ewas_localtime(const time_t *ts, struct tm *tm)
{
#ifdef _WIN32
	return (localtime_s(tm, ts) == 0)? tm : 0;
#else
	return localtime_r(ts, tm);
#endif
}

struct tm ewas::gmtime(time_t ts)
{
	struct tm tm;

	if(ewas_gmtime(&ts, &tm) == 0)
	{
		std::memset(&tm, 0, sizeof(tm));
	}

	return tm;
}

struct tm ewas::localtime(time_t ts)
{
	struct tm tm;

	if(ewas_localtime(&ts, &tm) == 0)
	{
		std::memset(&tm, 0, sizeof(tm));
	}

	return tm;
}

time_t ewas::utc_time(const struct tm &tm)
{
	struct tm tmp = tm;

	time_t ts = mktime(&tmp);

	tmp = gmtime(ts);

	ts += ts - mktime(&tmp);

	return ts;
}

std::string ewas::strftime(time_t ts, const std::string &format)
{
	char buffer[256];

	struct tm tm;

	ewas_gmtime(&ts, &tm);

	::strftime(buffer, sizeof(buffer), format.c_str(), &tm);

	return buffer;
}

std::string ewas::strftime(const struct tm &tm, const std::string &format)
{
	char buffer[256];

	::strftime(buffer, sizeof(buffer), format.c_str(), &tm);

	return buffer;
}
