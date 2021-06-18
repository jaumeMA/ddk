#include <gtest/gtest.h>
#include "ddk_avl_map.h"
#include "ddk_multi_map.h"
#include <utility>

using namespace testing;

class DDKMapTest : public Test
{
};

TEST(DDKMapTest,defaultConstruction)
{
	ddk::avl_map<std::string,int> prova;
	ddk::multi_map<std::string,int,ddk::avl_map> provaMultiMap;

	provaMultiMap[std::string("hola")][std::string("adeu")] = 1050;
	provaMultiMap[std::string("hola")][std::string("adeudeu")] = 1060;
	provaMultiMap[std::string("hola")][std::string("adeudeuadeu")] = 1070;
	provaMultiMap[std::string("adeu")][std::string("adeu")] = 1080;
	provaMultiMap[std::string("adeu")][std::string("adeudeuadeu")] = 1090;
	provaMultiMap[std::string("adeu")][std::string("adeudeu")] = 1100;

	const int str1 = provaMultiMap[std::string("hola")][std::string("adeu")];
	const int str2 = provaMultiMap[std::string("hola")][std::string("adeudeu")];
	const int str3 = provaMultiMap[std::string("hola")][std::string("adeudeuadeu")];

	const int str4 = provaMultiMap[std::string("adeu")][std::string("adeu")];
	const int str5 = provaMultiMap[std::string("adeu")][std::string("adeudeu")];
	const int str6 = provaMultiMap[std::string("adeu")][std::string("adeudeuadeu")];

	prova[std::string("hola")] = 10;
	prova[std::string("adeu")] = 20;
	ddk::avl_map<std::string,int>::iterator itBegin = prova.begin();
	ddk::avl_map<std::string,int>::iterator itEnd = prova.end();

	const size_t mapSize = prova.size();
	const bool emptyMap = prova.empty();

	for(;itBegin!=itEnd;)
	{
		itBegin = prova.erase(itBegin);
	}
}
TEST(DDKMapTest,degenerateConstruction)
{
	std::map<int,int> prova;

	for(size_t index=0;index<1000000;++index)
	{
		prova[index] = index;
	}


}