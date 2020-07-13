#include <gtest/gtest.h>
#include "ddk_arena.h"
#include <utility>

using namespace testing;

class DDKArenaTest : public TestWithParam<std::pair<size_t,size_t>>
{
};

TEST_P(DDKArenaTest, defaultConstruction)
{
	ddk::arena<4096,4096> emptyArena;

	EXPECT_EQ(emptyArena.empty(), true);

	emptyArena.destroy<int>();
}
TEST_P(DDKArenaTest, arenaConstruction)
{
	ddk::arena<4096,4096> playGround;

	playGround.construct<int>(10);

	EXPECT_EQ(playGround.get<int>(), 10);

	playGround.destroy<int>();
}
TEST_P(DDKArenaTest, arenaAssignment)
{
	ddk::arena<4096,4096> playGround;

	playGround.construct<int>(10);

	EXPECT_EQ(playGround.compare<int>(10), true);

	playGround.assign<int>(20);

	EXPECT_EQ(playGround.compare<int>(20),true);

	playGround.destroy<int>();
}
TEST_P(DDKArenaTest, arenaGet)
{
	ddk::arena<4096,4096> playGround;

	playGround.construct<int>(10);

	EXPECT_EQ(playGround.compare<int>(10), true);

	int& nestedValue = playGround.get<int>();

	nestedValue = 20;

	EXPECT_EQ(playGround.compare<int>(20), true);

	playGround.destroy<int>();
}
TEST_P(DDKArenaTest, arenaGetPtr)
{
	ddk::arena<4096,4096> playGround;

	playGround.construct<int>(10);

	int* nestedValuePtr = playGround.get_ptr<int>();

	*nestedValuePtr = 20;

	EXPECT_EQ(playGround.compare<int>(20), true);

	playGround.destroy<int>();
}
TEST_P(DDKArenaTest, arenaDestruction)
{
	ddk::arena<4096,4096> playGround;

	playGround.construct<int>(10);

	EXPECT_EQ(playGround.get<int>(), 10);

	playGround.destroy<int>();

	EXPECT_EQ(playGround.empty(), true);
}

INSTANTIATE_TEST_SUITE_P(DDKArenaTest, DDKArenaTest, Values(std::make_pair(sizeof(int),alignof(int))));