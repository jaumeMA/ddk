#include <gtest/gtest.h>

#include "ewas_time.h"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/c_local_time_adjustor.hpp>
#include <boost/locale/time_zone.hpp>

using namespace boost::gregorian;
using namespace boost::posix_time;
using namespace testing;

inline bool operator == (const struct tm &tm1, const struct tm &tm2)
{
	return tm1.tm_sec == tm2.tm_sec && tm1.tm_min == tm2.tm_min && tm1.tm_hour == tm2.tm_hour && tm1.tm_mday == tm2.tm_mday && tm1.tm_mon == tm2.tm_mon && tm1.tm_year == tm2.tm_year;
}

class EwasTimeTest : public TestWithParam<ptime>
{
};

TEST_P(EwasTimeTest, gmtime)
{
	const ptime time = GetParam();

	EXPECT_EQ(ewas::gmtime(to_time_t(time)), to_tm(time));
}

TEST_P(EwasTimeTest, localtime)
{
	const ptime time = GetParam();
	const ptime local_time = boost::date_time::c_local_adjustor<ptime>::utc_to_local(time);

	EXPECT_EQ(ewas::localtime(to_time_t(time)), to_tm(local_time));
}

TEST_P(EwasTimeTest, strftime)
{
	const ptime time = GetParam();

	EXPECT_EQ(ewas::strftime(to_time_t(time)), to_iso_extended_string(time.date()) + " " + to_simple_string(time.time_of_day()));
	EXPECT_EQ(ewas::strftime(to_tm(time)), to_iso_extended_string(time.date()) + " " + to_simple_string(time.time_of_day()));
}

TEST_P(EwasTimeTest, strftime_invalid)
{
	const ptime time = GetParam();

	EXPECT_EQ(ewas::strftime(to_time_t(time), ""), "");
	EXPECT_EQ(ewas::strftime(to_tm(time), ""), "");
}

TEST_P(EwasTimeTest, strftime_iso)
{
	time_facet *const facet = new time_facet(ewas::time_format::iso.c_str());

	std::stringstream stream;

	stream.imbue(std::locale(stream.getloc(), facet));

	const ptime time = GetParam();

	stream << time;

	EXPECT_EQ(ewas::strftime(to_time_t(time), ewas::time_format::iso), stream.str());
	EXPECT_EQ(ewas::strftime(to_tm(time), ewas::time_format::iso), stream.str());
}

INSTANTIATE_TEST_SUITE_P(EwasTimeTest, EwasTimeTest, Values(ptime(date(1970, 1, 1)), ptime(date(1980, 5, 7)), ptime(date(1970, 1, 1)) + hours(12) + minutes(30) + seconds(15), ptime(second_clock::universal_time())));
