#pragma once

namespace ddk
{
namespace mpl
{

template<typename T>
struct is_allocator
{
private:
	template<typename TT>
	static char func(const TT&, typename TT::allocator* = nullptr);
	template<typename ... TT>
	static int func(const TT& ...);

public:
	static const bool value = sizeof(decltype(func(std::declval<T>()))) == sizeof(char);
};

}
}