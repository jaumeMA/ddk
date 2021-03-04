#include "ddk_function.h"
#include "ddk_function_ops.h"

namespace ddk
{

const function<float(float)> sqrt = std::sqrt;
const function<float(float)> sin = std::sin;
const function<float(float)> cos = std::cos;

}