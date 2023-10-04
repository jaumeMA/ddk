#include <gtest/gtest.h>
#include "ddk_arena.h"
#include "ddk_scoped_enum.h"
#include "ddk_embedded_buddy_allocator.h"
#include "ddk_buddy_allocator.h"
#include "ddk_formatter.h"
#include "ddk_type_id.h"
#include "ddk_mutex.h"
#include "ddk_async.h"
#include "test_utils.h"
#include <utility>

using namespace testing;

class DDKArenaTest : public TestWithParam<std::pair<size_t,size_t>>
{
};

TEST(DDKArenaTest, defaultConstruction)
{
	ddk::unique_pointer_wrapper<ConstructionDeletionBalancer> foo = ddk::make_unique_reference<ConstructionDeletionBalancer>(0xFF);
	ddk::arena<sizeof(int),alignof(int)> emptyArena;

	EXPECT_EQ(emptyArena.empty(), true);

	emptyArena.destroy<int>();
}
TEST(DDKArenaTest, arenaConstruction)
{
	ddk::arena<sizeof(int),alignof(int)> playGround;

	playGround.construct<int>(10);

	EXPECT_EQ(playGround.get<int>(), 10);

	playGround.destroy<int>();
}
TEST(DDKArenaTest, arenaAssignment)
{
	ddk::arena<sizeof(int),alignof(int)> playGround;

	playGround.construct<int>(10);

	EXPECT_EQ(playGround.compare<int>(10), true);

	playGround.assign<int>(20);

	EXPECT_EQ(playGround.compare<int>(20),true);

	playGround.destroy<int>();
}
TEST(DDKArenaTest, arenaGet)
{
	ddk::arena<sizeof(int),alignof(int)> playGround;

	playGround.construct<int>(10);

	EXPECT_EQ(playGround.compare<int>(10), true);

	int& nestedValue = playGround.get<int>();

	nestedValue = 20;

	EXPECT_EQ(playGround.compare<int>(20), true);

	playGround.destroy<int>();
}
TEST(DDKArenaTest, arenaGetPtr)
{
	ddk::arena<sizeof(int),alignof(int)> playGround;

	playGround.construct<int>(10);

	int* nestedValuePtr = playGround.get_ptr<int>();

	*nestedValuePtr = 20;

	EXPECT_EQ(playGround.compare<int>(20), true);

	playGround.destroy<int>();
}
TEST(DDKArenaTest, arenaDestruction)
{
	ddk::arena<sizeof(int),alignof(int)> playGround;

	playGround.construct<int>(10);

	EXPECT_EQ(playGround.get<int>(), 10);

	playGround.destroy<int>();

	EXPECT_EQ(playGround.empty(), true);
}