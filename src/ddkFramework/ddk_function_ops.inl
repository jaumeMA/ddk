
#include <cmath>

namespace ddk
{

template<typename T>
auto sum(const std::initializer_list<T>& i_values)
{
	T res = 0;

	for(const auto& value : i_values)
	{
		res += value;
	}

	return res;
}
template<typename T>
auto prod(const std::initializer_list<T>& i_values)
{
	T res = 1;

	for(const auto& value : i_values)
	{
		res *= value;
	}

	return res;
}

}