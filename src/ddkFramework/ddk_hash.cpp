#include "ewas_hash.h"

namespace ewas
{

size_t hash(const std::string& i_value)
{
    size_t result = 0;
    size_t b = 378551;
    size_t a = 63689;

    std::string::const_iterator itChar = i_value.begin();
    for (; itChar != i_value.end(); ++itChar)
    {
        result = result*a + (*itChar);
        a = a*b;
    }

    return result;
}
size_t hash(const time_t& i_value)
{
    return static_cast<size_t>(i_value);
}

}
