#include "ddk_result.h"

namespace ddk
{

#if defined(DDK_DEBUG)

result_checker::result_checker(bool i_check)
: _checked(i_check)
{
}
result_checker::result_checker(result_checker&& other)
: _checked(true)
{
    std::swap(_checked,other._checked);
}
result_checker::~result_checker()
{
    DDK_ASSERT(_checked == true,"Unchecked result!");
}
result_checker& result_checker::operator=(bool i_checked)
{
    _checked = i_checked;

    return *this;
}
result_checker& result_checker::operator=(const result_checker& other)
{
    DDK_ASSERT(_checked == true,"Unchecked result!");

    _checked = other._checked;

    return *this;
}
result_checker& result_checker::operator=(result_checker&& other)
{
    DDK_ASSERT(_checked == true,"Unchecked result!");

    _checked = true;
    std::swap(_checked,other._checked);

    return *this;
}
result_checker::operator bool() const
{
    return _checked == true;
}

#endif

}