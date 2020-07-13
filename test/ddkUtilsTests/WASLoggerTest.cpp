#include <gmock/gmock.h>

#include "WASLogger.h"

using namespace testing;

class MockLogger
{
public:
	MOCK_METHOD(void, log, (const std::string &));
};

TEST(WASLoggerTest, LogStreamOnlyDebug)
{
	const std::string msg1 = "lorem", msg2 = "ipsum";

	MockLogger mock;

	EwasLogger<LOG_LEVEL_DEBUG>::configure([&mock](const std::string &msg) { mock.log(msg); });

	EXPECT_CALL(mock, log(msg1));

	WAS_LOG_DEBUG(msg1);
	WAS_LOG_INFO(msg2);
}

TEST(WASLoggerTest, LogStreamDebugInfo)
{
	const std::string msg1 = "lorem", msg2 = "ipsum";

	MockLogger mock1, mock2;

	EwasLogger<LOG_LEVEL_DEBUG>::configure([&mock1](const std::string &msg) { mock1.log(msg); });
	EwasLogger<LOG_LEVEL_INFO>::configure([&mock2](const std::string &msg) { mock2.log(msg); });

	EXPECT_CALL(mock1, log(msg1));

	WAS_LOG_DEBUG(msg1);

	EXPECT_CALL(mock2, log(msg2));

	WAS_LOG_INFO(msg2);
}

TEST(WASLoggerTest, LogStreamAppend)
{
	const std::string msg1 = "lorem", msg2 = "ipsum";

	MockLogger mock;

	EwasLogger<LOG_LEVEL_DEBUG>::configure([&mock](const std::string &msg) { mock.log(msg); });

	EXPECT_CALL(mock, log(msg1 + msg2));

	WAS_LOG_DEBUG(msg1 << msg2);
}
