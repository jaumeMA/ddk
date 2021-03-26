#include "ddk_hash.h"

namespace ddk
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

}
