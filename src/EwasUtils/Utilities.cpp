#include "Utilities.h"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <string.h>

#include "ewas_time.h"
#include "eWASConstants.h"

#ifdef __APPLE__
#include <mach/clock.h>
#include <mach/mach.h>

struct timespec orwl_gettime(void) {
    clock_serv_t cclock;
    mach_timespec_t mts;
    host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
    clock_get_time(cclock, &mts);
    mach_port_deallocate(mach_task_self(), cclock);
    struct timespec ts;
    ts.tv_sec = mts.tv_sec;
    ts.tv_nsec = mts.tv_nsec;
    return ts;
}

#endif

time_t Utilities::toTimeT(int year, int month, int day, int hour, int minute, int second)
{
	struct tm tm = ewas::gmtime(time(NULL));

	tm.tm_year = year - 1900;
	tm.tm_mon = month - 1;
	tm.tm_mday = day;
	tm.tm_hour = hour;
	tm.tm_min = minute;
	tm.tm_sec = second;

#ifdef WIN32
	time_t local = mktime(&tm);
	return (local < 0 ? -1 : local - _timezone);
#else
	time_t local = mktime(&tm);
	return (local < 0 ? -1 : local - timezone);
#endif
}

void Utilities::timeToHMS(time_t timestamp, int &hours, int &minutes, int &seconds)
{
	hours = (int) (timestamp/H_TO_S);
	minutes = (int) ((timestamp - hours*H_TO_S)/MIN_TO_S);
	seconds = (int) (timestamp%MIN_TO_S);
}

time_t Utilities::getTimeFromDHM(int day, int hour, int minute, time_t baseTime)
{
	int year = 0, month = 0, mday = 0;

	struct tm tm = ewas::gmtime(baseTime - 24 * H_TO_S);

	year = 1900 + tm.tm_year;
	month = tm.tm_mon + 1;
	mday = tm.tm_mday;

	if (mday > day)
	{
		month++;
		if (month > 12)
		{
			month = 1;
			year++;
		}
	}

	if (hour == 24)
	{
		hour = 0;
		minute = 0;
		day++;
	}

	if (minute == 60)
	{
		minute = 59;
	}

	return toTimeT(year, month, day, hour, minute, 0);
}

time_t Utilities::getTimeFromDHM(int day, int hour, int minute)
{
	return getTimeFromDHM(day, hour, minute, time(NULL));
}

time_t Utilities::startOfDay(time_t tSimulation)
{
	// 01/01/current year
	struct tm tm = ewas::gmtime(tSimulation);

	return Utilities::toTimeT(1900 + tm.tm_year, tm.tm_mon + 1, tm.tm_mday, 0, 0, 0);
}

/////////////////////////////////////////////////////////////////////////////
// Input:  time in double format
// Return: char with hh:mm(:ss)
/////////////////////////////////////////////////////////////////////////////
void Utilities::timeToHoursMinutesSeconds(double time, char* timeInFormat, bool withSeconds)
{
	int hours = 0, minutes = 0, seconds = 0;

	if (timeInFormat == NULL) return;

	hours = ((int)time) / H_TO_S;
	minutes = ((int)time - hours * H_TO_S) / MIN_TO_S;
	seconds = (int)time - hours * H_TO_S - minutes * MIN_TO_S;

	if (withSeconds) sprintf(timeInFormat, "+%02d:%02d:%02d", abs(hours), abs(minutes), abs(seconds));
	else sprintf(timeInFormat, "+%02d:%02d", abs(hours), abs(minutes));
}

std::string Utilities::timet2DateHoursMinutesSeconds(time_t timet)
{
	return ewas::strftime(timet, "%Y-%m-%d %H:%M:%S");
}

std::string Utilities::timet2IsoDate(time_t timet)
{
	return ewas::strftime(timet, "%Y-%m-%dT%H:%M:%S");
}

std::string Utilities::timet2UnformattedDateHoursMinutes(time_t timet)
{
	return ewas::strftime(timet, "%Y%m%d%H%M");
}

std::string Utilities::timet2Unformatted2YDateHoursMinutes(time_t timet)
{
	return ewas::strftime(timet, "%y%m%d%H%M");
}

std::string Utilities::timet2DateHoursMinutes(time_t timet)
{
	return ewas::strftime(timet, "%Y-%m-%d %H:%M");
}

std::string Utilities::timet2HoursMinute(time_t timet)
{
	return ewas::strftime(timet, "%H:%M");
}

std::string Utilities::timet2HoursMinuteSeconds(time_t timet)
{
	return ewas::strftime(timet, "%H:%M:%S");
}

std::string Utilities::timet2Date(time_t timet)
{
	return ewas::strftime(timet, "%Y-%m-%d");
}

//////////////////////////////////////////////////////////////////////
// Apply Antimeridian Correction
// Modified for iOS Version
//////////////////////////////////////////////////////////////////////
void Utilities::applyAntimeridianCorrection(double &antimeridian, const double &lon0, double &lon1)
{
	double tantimeridian = 0.0, tlon1 = 0.0, tlon0 = 0.0;

	memcpy(&tantimeridian, &antimeridian, sizeof(double));
	memcpy(&tlon1, &lon1, sizeof(double));
    memcpy(&tlon0, &lon0, sizeof(double));

	// Check Antimeridian
	if(tantimeridian == 0.0 && fabs(tlon1 - tlon0) > 180.0) // Crosses antimeridian first time
	{
		tantimeridian = (lon0 > 0.0 ? 360.0 : -360.0); // Add antimeridian correction +360/-360
	}

	if((tantimeridian > 0.0 && tlon1 < 0.0) || (tantimeridian < 0.0 && tlon1 > 0.0))
	{
		tlon1 += tantimeridian; // Apply antimeridian correction
	}

	memcpy(&antimeridian, &tantimeridian, sizeof(double));
	memcpy(&lon1, &tlon1, sizeof(double));
}

//////////////////////////////////////////////////////////////////////
// Apply Antimeridian Correction 2
//////////////////////////////////////////////////////////////////////
void Utilities::applyAntimeridianCorrection2(double &lon0, double &lon1)
{
	double tlon0 = 0.0, tlon1 = 0.0;

    memcpy(&tlon0, &lon0, sizeof(double));
	memcpy(&tlon1, &lon1, sizeof(double));

	double lon0_360 = ANGLE_TO_360(tlon0);
	double lon1_360 = ANGLE_TO_360(tlon1);

	// Check Antimeridian
	// Crosses antimeridian first time
	if((lon1_360 > lon0_360 &&  (lon0_360 <= 180.0 && lon1_360 > 180.0)) ||
	   (lon1_360 < lon0_360 && !(lon1_360 <= 180.0 && lon0_360 > 180.0)))
	{
		tlon1 += 360.0; // Apply antimeridian correction
	}

	memcpy(&lon1, &tlon1, sizeof(double));
}

//////////////////////////////////////////////////////////////////////
// Returns the current time in milliseconds
//////////////////////////////////////////////////////////////////////
double Utilities::getCurrentTimeInMS()
{
    double currentTime = 0;

    #ifdef WIN32
        currentTime = S_TO_MS * ((double) clock() / (double) CLOCKS_PER_SEC);
    #elif __APPLE__
        struct timespec now;
        now = orwl_gettime();
        currentTime = (double) now.tv_sec*1000.0 + (double) now.tv_nsec/1000000.0;	//Get time in ms
    #else
        struct timespec now;
        clock_gettime(CLOCK_REALTIME, &now);
        currentTime = (double) now.tv_sec*1000.0 + (double) now.tv_nsec/1000000.0;	//Get time in ms
    #endif

    return currentTime;
}

//////////////////////////////////////////////////////////////////////
// Returns the passed time by a defined string
//////////////////////////////////////////////////////////////////////
time_t Utilities::fromStringToTimeT(const std::string& time_details)
{
	//Example string "2015-03-31 19:25"
	int year,month,day,hour,minute,second;
	sscanf(time_details.c_str(),"%d-%d-%d %d:%d",&year,&month,&day,&hour,&minute);
	second = 0;
	time_t timeT = toTimeT(year, month, day, hour, minute, second);
	return timeT;
}

//////////////////////////////////////////////////////////////////////
// Returns the passed time into a defined string
//////////////////////////////////////////////////////////////////////
std::string Utilities::fromTimeTToString(const time_t i_time)
{
	return ewas::strftime(i_time, "%H:%M:%S %d/%m/%Y");
}

std::string Utilities::fromTimeTToDateString(const time_t i_time)
{
	return ewas::strftime(i_time, "%d/%m/%Y");
}

std::string Utilities::fromTimeTToDateHMString(const time_t i_time)
{
	return ewas::strftime(i_time, "%d/%m/%Y %H:%M");
}

//////////////////////////////////////////////////////////////////////
// Returns the passed time by a defined string
//////////////////////////////////////////////////////////////////////
time_t Utilities::fromStringToTimeTV2(const std::string& time_details)
{
	//Example string "20150331"
	int year,month,day;
	sscanf(time_details.c_str(),"%4d%2d%2d",&year,&month,&day);
	time_t timeT = toTimeT(year, month, day, 0, 0, 0);
	return timeT;
}

void Utilities::StdString2ValueArray(std::string arrayDesc, std::vector<int> &valueArray)
{
	int value[2] = {0, 0};
	std::istringstream is1(arrayDesc);

	// Store data types
	std::string token1;
	while(std::getline(is1, token1, ','))
	{
		int i = 0;
		std::istringstream is2(token1);
		std::string token2;

		while(std::getline(is2, token2, '-'))
		{
			if(i == 0)
			{
				value[i++] = atoi(token2.c_str());
				valueArray.push_back(value[0]);
			}
			else if(i == 1)
			{
				value[i++] = atoi(token2.c_str());
				for(int h = ++value[0] ; h <= value[1] ; h++)
				{
					valueArray.push_back(h);
				}
			}
			else
			{
				// NOT VALID
			}
		}
	}
}

std::string Utilities::ValueArray2StdString(const std::vector<int> &valueArray)
{
	std::ostringstream os;

	for(size_t i = 0; i < valueArray.size(); )
	{
		os << valueArray[i];
		if(i < valueArray.size()-1)
		{
			if(valueArray[i] == valueArray[i+1]-1)
			{
				os << "-";
				while(i < valueArray.size()-1 && valueArray[i] == valueArray[i+1]-1)
				{
					i++;
				}
			}
			else
			{
				os << ",";
				i++;
			}
		}
		else
		{
			i++;
		}
	}

	return os.str();
}

//////////////////////////////////////////////////////////////////////
// Split line in words
//////////////////////////////////////////////////////////////////////
void Utilities::split(const std::string &line, std::vector<std::string> &words)
{
	std::string word;
	std::stringstream ss(line);

	while (ss >> word)
		words.push_back(word);
}

//////////////////////////////////////////////////////////////////////
// Split the given text according to the given delimiter
//////////////////////////////////////////////////////////////////////
void Utilities::split(const std::string &text, std::vector<std::string> &items, char delim)
{
	std::string item;
	std::stringstream ss(text);

	while (std::getline(ss, item, delim))
		items.push_back(item);
}

//////////////////////////////////////////////////////////////////////////////////////
//  Returns the value of a point doing bilinear interpolation (using 4 points)
//////////////////////////////////////////////////////////////////////////////////////
double Utilities::bilinearInterpolation(double q11, double q12, double q21, double q22, double x1, double x2, double y1, double y2, double x, double y)
{
    double x2x1, y2y1, x2x, y2y, yy1, xx1;
    x2x1 = x2 - x1;
    y2y1 = y2 - y1;
    x2x = x2 - x;
    y2y = y2 - y;
    yy1 = y - y1;
    xx1 = x - x1;
    return 1.0 / (x2x1 * y2y1) * (
        q11 * x2x * y2y +
        q21 * xx1 * y2y +
        q12 * x2x * yy1 +
        q22 * xx1 * yy1
    );
}

//////////////////////////////////////////////////////////////////////////////////////
//  Returns the angle of a point doing bilinear interpolation (using 4 points)
//////////////////////////////////////////////////////////////////////////////////////
double Utilities::angleBilinearInterpolation(double q11, double q12, double q21, double q22, double x1, double x2, double y1, double y2, double x, double y)
{
	if((q21 - q11) > 180) q21 -= 360;
	else if((q21 - q11) < -180) q21 += 360;

	if((q12 - q11) > 180) q12 -= 360;
	else if((q12 - q11) < -180) q12 += 360;

	if((q22 - q11) > 180) q22 -= 360;
	else if((q22 - q11) < -180) q22 += 360;

   return ANGLE_TO_360(Utilities::bilinearInterpolation(q11, q12, q21, q22, x1, x2, y1, y2, x, y));
}

//////////////////////////////////////////////////////////////////////////////////////
//  Returns the value of a point doing trilinear interpolation (using 8 points)
//////////////////////////////////////////////////////////////////////////////////////
double Utilities::angularTrilinearInterpolation(double q111, double q112, double q121, double q122,
									    		double q211, double q212, double q221, double q222,
									    		double x1, double y1, double h1,
									    		double x2, double y2, double h2,
									    		double x, double y, double h)
{
	double h1Value = angleBilinearInterpolation(q111,q121,q211,q221,x1,x2,y1,y2,x,y);
	double h2Value = angleBilinearInterpolation(q112,q122,q212,q222,x1,x2,y1,y2,x,y);

	return linearInterpolation(h1,h2,h1Value,h2Value,h);
}

//////////////////////////////////////////////////////////////////////////////////////
//  Returns the value of a point doing linear interpolation (using 2 points)
//////////////////////////////////////////////////////////////////////////////////////
double Utilities::linearInterpolation(double q1, double q2, double x1, double x2, double x)
{
   return q1 + (q2 - q1)*((x - x1)/(x2 - x1));
}

//////////////////////////////////////////////////////////////////////////////////////
//  Returns the angle of a point doing linear interpolation (using 2 points)
//////////////////////////////////////////////////////////////////////////////////////
double Utilities::angleLinearInterpolation(double q1, double q2, double x1, double x2, double x)
{
	if((q2 - q1) > 180) q2 -= 360;
	else if((q2 - q1) < -180) q2 += 360;

   return ANGLE_TO_360(Utilities::linearInterpolation(q1, q2, x1, x2, x));
}

//////////////////////////////////////////////////////////////////////
// Gets the Wind Speed in string format
//////////////////////////////////////////////////////////////////////
std::string Utilities::getWindSpeedString(double speed)
{
	int speedInt = (int) speed;
	std::string windStr;
	windStr += speedInt >= 0 ? "T" : "H";
	speedInt = abs((int)speedInt);
	if(speedInt/10 == 0)
		windStr += "00";
	else if(speedInt/ 100 == 0)
		windStr += "0";
	windStr += std::to_string((long double)speedInt);
	size_t dotLoc = windStr.find(".");
	if(dotLoc != std::string::npos)
	{
		windStr.resize(dotLoc);
	}
	windStr += "KT";
	return windStr;
}

//////////////////////////////////////////////////////////////////////
// Gets the Latitude in string format (Degrees with decimal)
//////////////////////////////////////////////////////////////////////
std::string Utilities::getLatitudeDegreesWithDecimalString(double lat)
{
	char latitudeC = (lat > 0 ? 'N' : 'S');
	double latitude = fabs(lat);
	int	latitudeD = (int) latitude;
	double latitudeM = (latitude - latitudeD)*60;
	char strText[8];
	sprintf(strText, "%c%02d%04.1lf", latitudeC, latitudeD, latitudeM);
	return std::string(strText);
}

//////////////////////////////////////////////////////////////////////
// Gets the Latitude in string format (Dregrees, minutes and seconds)
//////////////////////////////////////////////////////////////////////
std::string Utilities::getLatitudeDMSString(double lat, bool addSeconds)
{
	char latitudeC = (lat > 0 ? 'N' : 'S');
	const double latitude = fabs(lat);
	const int latitudeD = static_cast<int>(floor(latitude));
	const int latitudeM = static_cast<int>(floor((latitude - latitudeD)*60));
	const int latitudeS = static_cast<int>(floor(((latitude - latitudeD)*60 - latitudeM)*60));

	char strText[16];
	if (addSeconds)
		sprintf(strText, "%iº %i' %i'' %c", latitudeD, latitudeM, latitudeS, latitudeC);
	else
		sprintf(strText, "%iº %i' %c", latitudeD, latitudeM, latitudeC);

	return std::string(strText);
}

//////////////////////////////////////////////////////////////////////
// Gets the Longitude in string format (Degrees with decimal)
//////////////////////////////////////////////////////////////////////
std::string Utilities::getLongitudeDegreesWithDecimalString(double lon)
{
	char longitudeC = (lon > 0 ? 'E' : 'W');
	double longitude = fabs(lon);
	int	longitudeD = (int) longitude;
	double longitudeM = (longitude - longitudeD)*60;
	char strText[9];
	sprintf(strText, "%c%03d%04.1lf", longitudeC, longitudeD, longitudeM);
	return std::string(strText);
}

//////////////////////////////////////////////////////////////////////
// Gets the Longitude in string format  (Dregrees, minutes and seconds)
//////////////////////////////////////////////////////////////////////
std::string Utilities::getLongitudeDMSString(double lon, bool addSeconds)
{
	char longitudeC = (lon > 0 ? 'E' : 'W');
	const double longitude = fabs(lon);
	const int longitudeD = static_cast<int>(floor(longitude));
	const int longitudeM = static_cast<int>(floor((longitude - longitudeD)*60));
	const int longitudeS = static_cast<int>(floor(((longitude - longitudeD)*60 - longitudeM)*60));
	
	char strText[16];
	if (addSeconds)
		sprintf(strText, "%iº %i' %i'' %c", longitudeD, longitudeM, longitudeS, longitudeC);
	else
		sprintf(strText, "%iº %i' %c", longitudeD, longitudeM, longitudeC);

	return std::string(strText);
}

//////////////////////////////////////////////////////////////////////
// Replace all occurrences
//////////////////////////////////////////////////////////////////////
std::string Utilities::replaceAll(std::string text, const std::string& from, const std::string& to)
{
	size_t start_pos = 0;
	while ((start_pos = text.find(from, start_pos)) != std::string::npos)
	{
		text.replace(start_pos, from.length(), to);
		start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
	}

	return text;
}

//////////////////////////////////////////////////////////////////////
// Wrap Text in lines of lineSize characters
//////////////////////////////////////////////////////////////////////
void Utilities::wrapText(std::string& text, unsigned int lineSize)
{
	std::string retString;
	std::vector<std::string> words;
	std::string::size_type prev_pos = 0, pos = 0, init_pos = 0;
	while((pos = text.find_first_of (" ,.:;", pos)) != std::string::npos)
	{
		if(prev_pos > init_pos && pos - init_pos > lineSize)
		{
			retString += text.substr(init_pos, prev_pos - init_pos -1) + "<br>";
			init_pos = prev_pos;
		}
		prev_pos = ++pos;
	}
	retString += text.substr(init_pos, pos - init_pos);

	// Return String
	text = retString;
}

int Utilities::round(double value)
{
	return int(std::floor(value + 0.5));
}

int Utilities::compare(const std::string &str0, const std::string &str1)
{
#ifdef WIN32
	return _stricmp(str0.c_str(), str1.c_str());
#else 
	return strcasecmp(str0.c_str(), str1.c_str());
#endif
}

double Utilities::normalizeAngle180(double angle)
{
	while(angle < -180)
		angle += 360;

	while(angle > 180)
		angle -= 360;

	return angle;
}

double Utilities::normalizeAngle360(double angle)
{
	while(angle < 0)
		angle += 360;

	while(angle > 360)
		angle -= 360;

	return angle;
}

double Utilities::getHeading(std::string& content)
{
	if (content == "N" || content == "NORTH")
	{
		return 0;
	}
	else if (content == "NNE" || content == "N/NE")
	{
		return 22.5;
	}
	else if (content == "NE")
	{
		return 45;
	}
	else if (content == "ENE" || content == "E/NE")
	{
		return 67.5;
	}
	else if (content == "E" || content == "EAST")
	{
		return 90;
	}
	else if (content == "ESE" || content == "E/SE")
	{
		return 112.5;
	}
	else if (content == "SE")
	{
		return 135;
	}
	else if (content == "SSE" || content == "S/SE")
	{
		return 157.5;
	}
	else if (content == "S" || content == "SOUTH")
	{
		return 180;
	}
	else if (content == "SSW" || content == "S/SW")
	{
		return 202.5;
	}
	else if (content == "SW")
	{
		return 225;
	}
	else if (content == "WSW" || content == "W/SW")
	{
		return 247.5;
	}
	else if (content == "W" || content == "WEST")
	{
		return 270;
	}
	else if (content == "WNW" || content == "W/NW")
	{
		return 292.5;
	}
	else if (content == "NW")
	{
		return 315;
	}
	else if (content == "NNW" || content == "N/NW")
	{
		return 337.5;
	}

	return 0;
}

unsigned short Utilities::getVersionNumber(const std::string &version)
{
	int major, minor, revision;
	sscanf(version.c_str(), "%d.%d.%d", &major, &minor, &revision);
	return major * 100 + minor;
}