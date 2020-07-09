#ifndef WASTypes_H
#define WASTypes_H

#include <string>
#include <cstdint>

typedef enum
{
    WAS_OK = 0,
    WAS_ERROR = 1,
    WAS_NULL  = 2,

    WAS_ERROR_QUERYINSERTION_POLYGONDAYS = 10,
    WAS_ERROR_QUERYINSERTION_GRIBFILE = 11,
    WAS_ERROR_QUERYINSERTION = 12
} WAS_Results;

class WAS_Result
{
public:
    WAS_Result()
    {
        m_result  = WAS_NULL;
        m_message = "";
    }

    WAS_Result(WAS_Results result)
    {
        m_result  = result;
        m_message = "";
    }

    WAS_Result(WAS_Results result, const std::string &message)
    {
        m_result  = result;
        m_message = message;
    }

    virtual ~WAS_Result()
    {

    }

    std::string getMessage()
    {
        return m_message;
    }

    void setMessage(const std::string &message)
    {
        m_message = message;
    }

    void operator=(WAS_Results a)
    {
        m_result  = a;
        m_message = "";
    }

    bool operator ==(const WAS_Results &a) const
    {
        return m_result == a;
    }

    bool operator !=(const WAS_Results &a) const
    {
        return m_result != a;
    }

private:
    WAS_Results m_result;
    std::string m_message;
};

#if defined(WIN32)
typedef __int8  was_int8;
typedef __int16 was_int16;
typedef __int32 was_int32;
typedef __int64 was_int64;

typedef unsigned __int8  was_uint8;
typedef unsigned __int16 was_uint16;
typedef unsigned __int32 was_uint32;
typedef unsigned __int64 was_uint64;

typedef float            was_real32;
typedef double           was_real64;
#else
typedef int8_t  was_int8;
typedef int16_t was_int16;
typedef int32_t was_int32;
typedef int64_t was_int64;

typedef uint8_t  was_uint8;
typedef uint16_t was_uint16;
typedef uint32_t was_uint32;
typedef uint64_t was_uint64;

typedef float            was_real32;
typedef double           was_real64;
#endif

//typedef real

#endif /* WASTypes_H */