#pragma once

#include "WASLogger.h"
#include "ddk_crash.h"
#include "ddk_flagset.h"

#if defined(WIN32)

    #define __TODO(_FILE,_LINE,_TEXT) __pragma(message(_FILE"("#_LINE") : " #_TEXT))
    #define _TODO(_FILE,_LINE,_TEXT) __TODO(_FILE,_LINE,_TEXT)
    #define TODO(text) _TODO(__FILE__,__LINE__,text)
    #define UNUSED(_x) (void)(_x);

    #ifdef DDK_DEBUG

        #include "ddk_assert.h"

        #define DDK_ASSERT(__cond,__msg) \
            { \
                if((__cond) == false) \
                { \
                    ddk::detail::Assert assertion(#__cond,__FILE__,__LINE__,"Assert",__msg); \
                    if (assertion.raise() == ddk::detail::Assert::Break) \
                    { \
                        __debugbreak(); \
                    } \
                } \
            }

        #define DDK_FAIL(__msg) \
            { \
                ddk::detail::Assert assertion(__FILE__,__LINE__,"Assert",__msg); \
                if (assertion.raise() == ddk::detail::Assert::Break) \
                { \
                    __debugbreak(); \
                } \
            }
        #define DDK_ASSERT_OR_LOG(__cond,__msg) \
            DDK_ASSERT(__cond,__msg)
        #define DDK_FAIL_OR_LOG(__msg) \
            DDK_FAIL(__msg)

    #else

        #define DDK_ASSERT(cond,msg)
        #define DDK_FAIL(msg)
        #define DDK_ASSERT_OR_LOG(__cond,__msg) \
            { \
                if((__cond) == false) \
                { \
                    WAS_LOG_ERROR(__msg); \
                } \
            }
        #define DDK_FAIL_OR_LOG(__msg) \
            WAS_LOG_ERROR(__msg);

    #endif

#elif defined(__LINUX__)

    #define PRAGMA(p)		_Pragma(#p)
    #define TODO(something) PRAGMA(message(something));
    #define UNUSED(_x) (void)_x;

    #ifdef DDK_DEBUG

        #include <signal.h>
        #include "ddk_assert.h"

        #define DDK_ASSERT(__cond,__msg) \
            if((__cond) == false) \
            { \
                ddk::detail::Assert assertion(#__cond,__FILE__,__LINE__,"Assert",__msg); \
                if (assertion.raise() == ddk::detail::Assert::Break) \
                { \
                    raise(SIGTRAP); \
                } \
            }

        #define DDK_FAIL(__msg) \
            { \
                ddk::detail::Assert assertion(__FILE__,__LINE__,"Assert",__msg); \
                if (assertion.raise() == ddk::detail::Assert::Break) \
                { \
                    raise(SIGTRAP); \
                } \
            }

        #define DDK_ASSERT_OR_LOG(__cond,__msg) \
            DDK_ASSERT(__cond,__msg)
        #define DDK_FAIL_OR_LOG(__msg) \
            DDK_FAIL(__msg)

    #else

        #define DDK_ASSERT(__cond,__msg)
        #define DDK_FAIL(__msg)
        #define DDK_ASSERT_OR_LOG(__cond,__msg) \
            { \
                if((__cond) == false) \
                { \
                    WAS_LOG_ERROR(__msg); \
                } \
            }
        #define DDK_FAIL_OR_LOG(__msg) \
            WAS_LOG_ERROR(__msg);

    #endif

#elif defined(__APPLE__)

    #define PRAGMA(p) _Pragma(#p)
    #define TODO(something) PRAGMA(message(something));
    #define UNUSED(_x) (void)_x;

    #ifdef DDK_DEBUG

        #include <signal.h>

        #define DDK_ASSERT(__cond,__msg) \
            { \
                if((__cond) == false) \
                { \
                    raise(SIGTRAP); \
                } \
            }
        #define DDK_FAIL(__msg) \
            { \
                raise(SIGTRAP); \
            }
        #define DDK_ASSERT_OR_LOG(__cond,__msg) \
            DDK_ASSERT(__cond,__msg)
        #define DDK_FAIL_OR_LOG(__msg) \
            DDK_FAIL(__msg)

    #else

        #define DDK_ASSERT(cond,msg)
        #define DDK_FAIL(msg)
        #define DDK_ASSERT_OR_LOG(__cond,__msg) \
            { \
                if((__cond) == false) \
                { \
                    WAS_LOG_ERROR(__msg); \
                } \
            }
        #define DDK_FAIL_OR_LOG(__msg) \
            WAS_LOG_ERROR(__msg);

    #endif

#else

#define PRAGMA(p)		_Pragma(#p)
#define TODO(something) PRAGMA(message(something));
#define UNUSED (void)
#endif

#ifndef NULL
	#define NULL 0
#endif

#define MAKE_IT_CRASH \
		WAS_LOG_ERROR("Crashing at file: " << __FILE__ << ", line: " << __LINE__); \
		*reinterpret_cast<volatile int*>(0xDEAD);

#define CRASH_ON_RETURN(_TYPE) \
		WAS_LOG_ERROR("Crashing at file: " << __FILE__ << ", line: " << __LINE__); \
		return ddk::crash_on_return<_TYPE>::value();
