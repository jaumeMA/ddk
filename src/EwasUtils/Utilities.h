#pragma once

#include <string>
#include <vector>

class Utilities
{
public:

	/**
	* @brief Transforms a time stamp defined by different ints (year, month, day, hour , minute, second) to time_t and returns it
	* @param year Referenced date year
	* @param month Referenced date month
	* @param day Referenced date day
	* @param hour Referenced hour
	* @param minute Referenced minute
	* @param second Referenced second
	* @return The new time_t value
	*/
	static time_t toTimeT(int year, int month, int day, int hour, int minute, int second);

	/**
	* @brief Transforms a time_t to HMS
	* @param time_t timestamp
	* @param hours hours
	* @param minutes minutes
	* @param seconds seconds
	*/
	static void timeToHMS(time_t timestamp, int &hours, int &minutes, int &seconds);

	/**
	* @brief Get time_t from DHM
	* @param day day of month
	* @param hours hours
	* @param minutes minutes
	* @return The time_t value
	*/
	static time_t getTimeFromDHM(int day, int hour, int minute, time_t baseTime);
	static time_t getTimeFromDHM(int day, int hour, int minute);

	/**
	* @brief Returns a time stamp of the start of current year 01/01/current year
	* @return The time_t value
	*/
	static time_t startOfDay(time_t tSimulation);

	/**
	* @brief Input:  time in double format
	* @param time Time to be converted
	* @param timeInFormat Char containing the converted time
	* @return void, output in char with time in hh:mm:ss
	*/
	static void timeToHoursMinutesSeconds(double time, char* timeInFormat, bool withSeconds = true);

	/**
	* @brief Converts from time t to a string in format yyyy-mm-dd hh:mm:ss
	* @return String with the date in the indicated format
	*/
	static std::string timet2DateHoursMinutesSeconds(time_t timet);

	/**
	* @brief Converts from time t to a string in format yyyy-mm-ddThh:mm:ss
	* @return String with the date in the indicated format
	*/
	static std::string timet2IsoDate(time_t timet);

	/**
	* @brief Converts from time t to a string in format yyyymmddhhmm
	* @return String with the date in the indicated format
	*/
	static std::string timet2UnformattedDateHoursMinutes(time_t timet);

	/**
	* @brief Converts from time t to a string in format yymmddhhmm
	* @return String with the date in the indicated format
	*/
	static std::string timet2Unformatted2YDateHoursMinutes(time_t timet);

	/**
	* @brief Converts from time t to a string in format yyyy-mm-dd hh:mm
	* @return String with the date in the indicated format
	*/
	static std::string timet2DateHoursMinutes(time_t timet);

		/**
	* @brief Converts from time t to a string in format hh:mm
	* @return String with the date in the indicated format
	*/
	static std::string timet2HoursMinute(time_t timet);

		/**
	* @brief Converts from time t to a string in format hh:mm
	* @return String with the date in the indicated format
	*/
	static std::string timet2HoursMinuteSeconds(time_t timet);

	/**
	* @brief Converts from time t to a string in format yyyy-mm-dd
	* @return String with the date in the indicated format
	*/
	static std::string timet2Date(time_t timet);

	/**
	* Apply Antimeridian Correction
	*/
	 static void applyAntimeridianCorrection(double &antimeridian, const double &lon0, double &lon1);
	 static void applyAntimeridianCorrection2(double &lon0, double &lon1);

	/**
	* @brief Returns the current time in milliseconds
	* @return double current time in milliseconds
	*/
    static double getCurrentTimeInMS();

	/**
	* @brief Returns the passed time by a defined string
	* @param time_details String containing the time details
	* @return time_t passed time by a defined string
	*/
    static time_t fromStringToTimeT(const std::string& time_details);

	/**
	* @brief Returns the passed time into a defined string
	* @param i_time passed time
	* @return string passed time converted into a string
	*/
	static std::string fromTimeTToString(const time_t i_time);
	static std::string fromTimeTToDateString(const time_t i_time);
	static std::string fromTimeTToDateHMString(const time_t i_time);

	/**
	* @brief Returns the passed time by a defined string
	* Example string "20150331"
	*/
    static time_t fromStringToTimeTV2(const std::string& time_details);

	static void StdString2ValueArray(std::string arrayDesc, std::vector<int> &valueArray);
	static std::string	ValueArray2StdString(const std::vector<int> &valueArray);

	/**
	* @brief Split the given text in words
	*/
	static void split(const std::string &line, std::vector<std::string> &words);

	/**
	* @brief Split the given text according to the given delimiter
	*/
	static void split(const std::string &text, std::vector<std::string> &items, char delim);

	/**
	* @brief Returns the value of the interpolation of 4 points
	* @return The value of the point at [x y]
	* @param q11 Value of the point at [x1 y1]
	* @param q12 Value of the point at [x1 y2]
	* @param q21 Value of the point at [x2 y1]
	* @param q22 Value of the point at [x2 y2]
	* @param x1 position
	* @param x2 position
	* @param y1 position
	* @param y2 position
	* @param x	position
	* @param y	position
	*/
	static double bilinearInterpolation(double q11, double q12, double q21, double q22, double x1, double x2, double y1, double y2, double x, double y);
	static double angleBilinearInterpolation(double q11, double q12, double q21, double q22, double x1, double x2, double y1, double y2, double x, double y);

	/**
	* @brief Returns the value of the interpolation of two sets of 4 points in same lat and long but different heights at a given height
	* @return The value of the point at [x y h]
	* @param q111 Value of the point at [x1 y1 h1]
	* @param q112 Value of the point at [x1 y2 h2]
	* @param q121 Value of the point at [x1 y2 h1]
	* @param q122 Value of the point at [x1 y2 h2]
	* @param q211 Value of the point at [x1 y2 h1]
	* @param q212 Value of the point at [x1 y2 h2]
	* @param q221 Value of the point at [x2 y1 h1]
	* @param q222 Value of the point at [x2 y2 h2]
	* @param x1 position
	* @param x2 position
	* @param y1 position
	* @param y2 position
	* @param h1 height
	* @param h2 height
	* @param x	position
	* @param y	position
	* @param h	height
	*/
	static double angularTrilinearInterpolation(double q111, double q112, double q121, double q122,
												double q211, double q212, double q221, double q222,
												double x1, double y1, double h1,
												double x2, double y2, double h2,
												double x, double y, double h);

	/**
	* @brief Returns the value of the interpolation of 2 points
	* @return The value of the point at [x]
	* @param q1 Value of the point at [x1]
	* @param q2 Value of the point at [x2]
	* @param x1 position
	* @param x2 position
	* @param x position
	*/
	static double linearInterpolation(double q1, double q2, double x1, double x2, double x);
	static double angleLinearInterpolation(double q1, double q2, double x1, double x2, double x);

	/**
	* @brief Gets the Wind Speed in string format
	*/
	static std::string getWindSpeedString(double speed);

	/**
	* @brief Gets the Latitude in string format (Dregrees and minutes)
	*/
	static std::string getLatitudeDegreesWithDecimalString(double lat);
	
	/**
	* @brief Gets the Latitude in string format (Dregrees, minutes and seconds)
	*/
	static std::string getLatitudeDMSString(double lat, bool addSeconds = true);

	/**
	* @brief Gets the Longitude in string format (Dregrees and minutes)
	*/
	static std::string getLongitudeDegreesWithDecimalString(double lon);
	
	/**
	* @brief Gets the Longitude in string format (Dregrees, minutes and seconds)
	*/
	static std::string getLongitudeDMSString(double lon, bool addSeconds = true);

	/**
	* @brief Replace all occurrences
	*/
	static std::string replaceAll(std::string text, const std::string& from, const std::string& to);

	/**
	* @brief Wrap Text in lines of lineSize characters
	*/
	static void wrapText(std::string& text, unsigned int lineSize);

	static int round(double value);

	/* Compare String */
	static int compare(const std::string &str0, const std::string &str1);

	static double normalizeAngle180(double angle);
	static double normalizeAngle360(double angle);

	static double getHeading(std::string& content);

	static unsigned short getVersionNumber(const std::string &version);
};
