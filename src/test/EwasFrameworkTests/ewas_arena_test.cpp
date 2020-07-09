#include <gtest/gtest.h>
#include "ewas_arena.h"
#include <utility>

using namespace testing;

class EwasArenaTest : public TestWithParam<std::pair<size_t,size_t>>
{
};

TEST_P(EwasArenaTest, defaultConstruction)
{
	ewas::arena<4096,4096> emptyArena;

	EXPECT_EQ(emptyArena.empty(), true);

	emptyArena.destroy<int>();
}
TEST_P(EwasArenaTest, arenaConstruction)
{
	ewas::arena<4096,4096> playGround;

	playGround.construct<int>(10);

	EXPECT_EQ(playGround.get<int>(), 10);

	playGround.destroy<int>();
}
TEST_P(EwasArenaTest, arenaAssignment)
{
	ewas::arena<4096,4096> playGround;

	playGround.construct<int>(10);

	EXPECT_EQ(playGround.compare<int>(10), true);

	playGround.assign<int>(20);

	EXPECT_EQ(playGround.compare<int>(20),true);

	playGround.destroy<int>();
}
TEST_P(EwasArenaTest, arenaGet)
{
	ewas::arena<4096,4096> playGround;

	playGround.construct<int>(10);

	EXPECT_EQ(playGround.compare<int>(10), true);

	int& nestedValue = playGround.get<int>();

	nestedValue = 20;

	EXPECT_EQ(playGround.compare<int>(20), true);

	playGround.destroy<int>();
}
TEST_P(EwasArenaTest, arenaGetPtr)
{
	ewas::arena<4096,4096> playGround;

	playGround.construct<int>(10);

	int* nestedValuePtr = playGround.get_ptr<int>();

	*nestedValuePtr = 20;

	EXPECT_EQ(playGround.compare<int>(20), true);

	playGround.destroy<int>();
}
TEST_P(EwasArenaTest, arenaDestruction)
{
	ewas::arena<4096,4096> playGround;

	playGround.construct<int>(10);

	EXPECT_EQ(playGround.get<int>(), 10);

	playGround.destroy<int>();

	EXPECT_EQ(playGround.empty(), true);
}

INSTANTIATE_TEST_SUITE_P(EwasArenaTest, EwasArenaTest, Values(std::make_pair(sizeof(int),alignof(int))));