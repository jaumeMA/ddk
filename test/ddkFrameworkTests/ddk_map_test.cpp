#include <gtest/gtest.h>
#include "ddk_avl_map.h"
#include "ddk_multi_map_adaptor.h"
#include "ddk_iterable.h"
#include "ddk_formatter.h"
#include <utility>

using namespace testing;

class DDKMapTest : public Test
{
};
class DDKMultiMapTest: public Test
{
};

TEST(DDKMapTest,defaultConstruction)
{
	ddk::avl_map<std::string,int> prova;
}

TEST(DDKMultiMapTest,defaultConstruction)
{
	ddk::multi_map<std::string,int,ddk::avl_map> provaMultiMap;
}

TEST(DDKMapTest,constructionByAccess)
{
	ddk::avl_map<std::string,int> prova;

	prova[std::string("hola")] = 10;
	prova[std::string("adeu")] = 20;

	EXPECT_EQ(prova[std::string("hola")],10);
	EXPECT_EQ(prova[std::string("adeu")],20);
}

TEST(DDKMultiMapTest,constructionByAccess)
{
	ddk::multi_map<std::string,int,ddk::avl_map> provaMultiMap;

	provaMultiMap[std::string("hola")][std::string("adeu")] = 1050;
	provaMultiMap[std::string("hola")][std::string("adeudeu")][std::string("ladilla")] = 1060;
	provaMultiMap[std::string("hola")][std::string("adeudeuadeu")] = 1070;
	provaMultiMap[std::string("adeu")][std::string("adeu")] = 1080;
	provaMultiMap[std::string("adeu")][std::string("adeudeuadeu")] = 1090;
	provaMultiMap[std::string("adeu")][std::string("adeudeu")] = 1100;

	EXPECT_EQ(provaMultiMap[std::string("hola")][std::string("adeu")],1050);
	EXPECT_EQ(provaMultiMap[std::string("hola")][std::string("adeudeu")][std::string("ladilla")],1060);
	EXPECT_EQ(provaMultiMap[std::string("hola")][std::string("adeudeuadeu")],1070);
	EXPECT_EQ(provaMultiMap[std::string("adeu")][std::string("adeu")],1080);
	EXPECT_EQ(provaMultiMap[std::string("adeu")][std::string("adeudeu")],1100);
	EXPECT_EQ(provaMultiMap[std::string("adeu")][std::string("adeudeuadeu")],1090);
}

TEST(DDKMapTest,size)
{
	ddk::avl_map<std::string,int> prova;

	const std::vector<std::pair<std::string,int>> contents = { std::make_pair("hola",10),std::make_pair("adeu",20) };

	prova[contents[0].first] = contents[0].second;
	prova[contents[1].first] = contents[1].second;

	EXPECT_EQ(prova.size(),2);
	EXPECT_EQ(prova.empty(),false);
}

TEST(DDKMapTest,visitation)
{
	ddk::avl_map<std::string,int> prova;

	const std::vector<std::pair<std::string,int>> contents = { std::make_pair("adeu",20),std::make_pair("hola",10) };

	prova[contents[0].first] = contents[0].second;
	prova[contents[1].first] = contents[1].second;

	ddk::avl_map<std::string,int>::iterator itBegin = prova.begin();
	ddk::avl_map<std::string,int>::iterator itEnd = prova.end();

	for(size_t index=0; itBegin != itEnd;++index,++itBegin)
	{
		EXPECT_EQ(itBegin->first,contents[index].first);
		EXPECT_EQ(itBegin->second,contents[index].second);
	}
}

TEST(DDKMapTest,erase)
{
	ddk::avl_map<std::string,int> prova;

	const std::vector<std::pair<std::string,int>> contents = { std::make_pair("hola",10),std::make_pair("adeu",20) };

	prova[contents[0].first] = contents[0].second;
	prova[contents[1].first] = contents[1].second;

	ddk::avl_map<std::string,int>::iterator itBegin = prova.begin();
	ddk::avl_map<std::string,int>::iterator itEnd = prova.end();

	for(size_t index = 0; itBegin != itEnd; ++index)
	{
		itBegin = prova.erase(itBegin);
	}
}

TEST(DDKMultiMapTest,visitation)
{
	ddk::multi_map<std::string,int,ddk::avl_map> provaMultiMap;

	provaMultiMap[std::string("hola")][std::string("adeu")] = 1050;
	provaMultiMap[std::string("hola")][std::string("adeudeu")][std::string("ladilla")] = 1060;
	provaMultiMap[std::string("hola")][std::string("adeudeuadeu")] = 1070;
	provaMultiMap[std::string("adeu")][std::string("adeu")] = 1080;
	provaMultiMap[std::string("adeu")][std::string("adeudeuadeu")] = 1090;
	provaMultiMap[std::string("adeu")][std::string("adeudeu")] = 1100;

	ddk::make_function([](const std::pair<const std::vector<std::string>,int&>& i_pair) 
	{ 
		printf("multimap value %s, %d. \n",ddk::formatter<std::string>::format(i_pair.first).c_str(),i_pair.second);
	}) <<= ddk::view::order(ddk::reverse_order) <<= provaMultiMap;
}

#ifndef DDK_DEBUG

TEST(DDKMapTest,degenerateConstruction)
{
	ddk::avl_map<int,int> prova;

	for(size_t index=0;index<1000000;++index)
	{
		prova[index] = index;
	}
}

TEST(DDKMultiMapTest,degenerateConstruction)
{
	ddk::multi_map<int,int,ddk::avl_map> provaMultiMap;
	size_t index = 0;

	for(size_t index1 = 0; index1 < 100; ++index1,++index)
	{
		for(size_t index2 = 0; index2 < 100; ++index2,++index)
		{
			for(size_t index3 = 0; index3 < 100; ++index3,++index)
			{
				provaMultiMap[index1][index2][index3] = index;
			}
		}
	}
}

#endif