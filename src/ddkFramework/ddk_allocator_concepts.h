#pragma once

#include "ddk_allocator_template_helper.h"
#include "ddk_type_concepts.h"

#define IS_ALLOCATOR(_TYPE) \
	typename std::enable_if<ddk::concepts::is_allocator<_TYPE>::value>::type*

#define IS_NOT_ALLOCATOR(_TYPE) \
	typename std::enable_if<ddk::concepts::is_allocator<_TYPE>::value==false>::type*

namespace ddk
{
namespace concepts
{

template<typename T>
struct is_allocator
{
private:
	template<typename TT>
	static char func(const TT&,typename TT::allocator* = nullptr);
	template<typename ... TT>
	static int func(const TT& ...);

public:
	static const bool value = sizeof(decltype(func(std::declval<T>()))) == sizeof(char);
};

}
}