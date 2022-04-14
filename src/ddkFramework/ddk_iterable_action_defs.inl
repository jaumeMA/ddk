
#include "ddk_fiber_utils.h"

namespace ddk
{

template<typename T>
void stop_action::operator()(const T& i_code, const std::string& i_reason) const
{
	suspend(static_cast<int>(i_code),i_reason);
}

}
