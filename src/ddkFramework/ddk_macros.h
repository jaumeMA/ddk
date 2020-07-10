#pragma once

#include "WASLogger.h"
#include "ewas_crash.h"
#include "ewas_flagset.h"

#if defined(WIN32)

    #define __TODO(_FILE,_LINE,_TEXT) __pragma(message(_FILE"("#_LINE") : " #_TEXT))
    #define _TODO(_FILE,_LINE,_TEXT) __TODO(_FILE,_LINE,_TEXT)
    #define TODO(text) _TODO(__FILE__,__LINE__,text)
    #define UNUSED(_x) (void)(_x);

    #ifdef EWAS_DEBUG

        #include "ewas_assert.h"

        #define EWAS_ASSERT(__cond,__msg) \
            { \
                if((__cond) == false) \
                { \
                    ewas::detail::Assert assertion(#__cond,__FILE__,__LINE__,"Assert",__msg); \
                    if (assertion.raise() == ewas::detail::Assert::Break) \
                    { \
                        __debugbreak(); \
                    } \
                } \
            }

        #define EWAS_FAIL(__msg) \
            { \
                ewas::detail::Assert assertion(__FILE__,__LINE__,"Assert",__msg); \
                if (assertion.raise() == ewas::detail::Assert::Break) \
                { \
                    __debugbreak(); \
                } \
            }
        #define EWAS_ASSERT_OR_LOG(__cond,__msg) \
            EWAS_ASSERT(__cond,__msg)
        #define EWAS_FAIL_OR_LOG(__msg) \
            EWAS_FAIL(__msg)

    #else

        #define EWAS_ASSERT(cond,msg)
        #define EWAS_FAIL(msg)
        #define EWAS_ASSERT_OR_LOG(__cond,__msg) \
            { \
                if((__cond) == false) \
                { \
                    WAS_LOG_ERROR(__msg); \
                } \
            }
        #define EWAS_FAIL_OR_LOG(__msg) \
            WAS_LOG_ERROR(__msg);

    #endif

#elif defined(__LINUX__)

    #define PRAGMA(p)		_Pragma(#p)
    #define TODO(something) PRAGMA(message(something));
    #define UNUSED(_x) (void)_x;

    #ifdef EWAS_DEBUG

        #include <signal.h>
        #include "ewas_assert.h"

        #define EWAS_ASSERT(__cond,__msg) \
            if((__cond) == false) \
            { \
                ewas::detail::Assert assertion(#__cond,__FILE__,__LINE__,"Assert",__msg); \
                if (assertion.raise() == ewas::detail::Assert::Break) \
                { \
                    raise(SIGTRAP); \
                } \
            }

        #define EWAS_FAIL(__msg) \
            { \
                ewas::detail::Assert assertion(__FILE__,__LINE__,"Assert",__msg); \
                if (assertion.raise() == ewas::detail::Assert::Break) \
                { \
                    raise(SIGTRAP); \
                } \
            }

        #define EWAS_ASSERT_OR_LOG(__cond,__msg) \
            EWAS_ASSERT(__cond,__msg)
        #define EWAS_FAIL_OR_LOG(__msg) \
            EWAS_FAIL(__msg)

    #else

        #define EWAS_ASSERT(__cond,__msg)
        #define EWAS_FAIL(__msg)
        #define EWAS_ASSERT_OR_LOG(__cond,__msg) \
            { \
                if((__cond) == false) \
                { \
                    WAS_LOG_ERROR(__msg); \
                } \
            }
        #define EWAS_FAIL_OR_LOG(__msg) \
            WAS_LOG_ERROR(__msg);

    #endif

#elif defined(__APPLE__)

    #define PRAGMA(p) _Pragma(#p)
    #define TODO(something) PRAGMA(message(something));
    #define UNUSED(_x) (void)_x;

    #ifdef EWAS_DEBUG

        #include <signal.h>

        #define EWAS_ASSERT(__cond,__msg) \
            { \
                if((__cond) == false) \
                { \
                    raise(SIGTRAP); \
                } \
            }
        #define EWAS_FAIL(__msg) \
            { \
                raise(SIGTRAP); \
            }
        #define EWAS_ASSERT_OR_LOG(__cond,__msg) \
            EWAS_ASSERT(__cond,__msg)
        #define EWAS_FAIL_OR_LOG(__msg) \
            EWAS_FAIL(__msg)

    #else

        #define EWAS_ASSERT(cond,msg)
        #define EWAS_FAIL(msg)
        #define EWAS_ASSERT_OR_LOG(__cond,__msg) \
            { \
                if((__cond) == false) \
                { \
                    WAS_LOG_ERROR(__msg); \
                } \
            }
        #define EWAS_FAIL_OR_LOG(__msg) \
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
		return ewas::crash_on_return<_TYPE>::value();
