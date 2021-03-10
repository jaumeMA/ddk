#include "ddk_function.h"
#include "ddk_function_ops.h"
#include <cmath>

namespace ddk
{

const function<float(float)> sqrt = std::sqrt;
const function<float(float,float)> pow = std::powf;
const function<float(float)> sin = std::sin;
const function<float(float)> cos = std::cos;

}