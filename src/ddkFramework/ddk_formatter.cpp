#include "ewas_formatter.h"

namespace ewas
{

bool formatter<bool>::format(const std::string& i_str)
{
	return atoi(i_str.c_str()) != 0;
}
char formatter<char>::format(const std::string& i_str)
{
	return atoi(i_str.c_str());
}
unsigned char formatter<unsigned char>::format(const std::string& i_str)
{
	return atoi(i_str.c_str());
}
short formatter<short>::format(const std::string& i_str)
{
	return atoi(i_str.c_str());
}
unsigned short formatter<unsigned short>::format(const std::string& i_str)
{
	return atoi(i_str.c_str());
}
int formatter<int>::format(const std::string& i_str)
{
	return atoi(i_str.c_str());
}
unsigned int formatter<unsigned int>::format(const std::string& i_str)
{
	return static_cast<unsigned int>(std::stoul(i_str.c_str()));
}
long formatter<long>::format(const std::string& i_str)
{
	return stol(i_str);
}
unsigned long formatter<unsigned long>::format(const std::string& i_str)
{
	return stoul(i_str);
}
long long formatter<long long>::format(const std::string& i_str)
{
	return stoll(i_str);
}
unsigned long long formatter<unsigned long long>::format(const std::string& i_str)
{
	return stoull(i_str);
}
float formatter<float>::format(const std::string& i_str)
{
	return static_cast<float>(atof(i_str.c_str()));
}
double formatter<double>::format(const std::string& i_str)
{
	return atof(i_str.c_str());
}
std::string formatter<std::string>::format(const std::string& i_str)
{
	return i_str;
}

}