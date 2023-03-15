#include <gtest/gtest.h>
#include "ddk_arena.h"
#include "ddk_scoped_enum.h"
#include "ddk_embedded_buddy_allocator.h"
#include "ddk_buddy_allocator.h"
#include "ddk_formatter.h"
#include "ddk_type_id.h"
#include "ddk_mutex.h"
#include <utility>

using namespace testing;

class DDKArenaTest : public TestWithParam<std::pair<size_t,size_t>>
{
};

TEST(DDKArenaTest, defaultConstruction)
{
	ddk::atomic<size_t> value(10);

	const size_t val = ddk::atomic_add(value,size_t(22));

	value = value;

	char* kk = (char*)malloc(4096);
	ddk::embedded_buddy_allocator<ddk::mpl::get_power_of_two(4096) - ddk::mpl::get_power_of_two(64)> ba(kk,4096);

	struct kk_t;
	typedef ddk::Id<size_t,kk_t> kk_id;
	kk_id kkk = ddk::format_to<kk_id>(std::string("10"));

	void* a0 = ba.allocate(100);
	void* a1 = ba.allocate(100);
	void* b = ba.allocate(200);
	void* c = ba.allocate(500);

	ba.deallocate(a1);
	ba.deallocate(a0);
	ba.deallocate(c);
	ba.deallocate(b);

	free(kk);

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