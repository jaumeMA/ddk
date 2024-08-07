//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#define IS_RTTI(_TYPE) \
	typename std::enable_if<ddk::concepts::is_rtti_available<_TYPE>::value>::type*

namespace ddk
{
namespace concepts
{

template<typename T>
struct is_rtti_available
{
private:
	template<typename TT, typename = typename TT::rtti_tag*>
	static std::true_type _checker(const TT&);
	template<typename ... TT>
	static std::false_type _checker(const TT& ...);

public:
	static const bool value = decltype(_checker(std::declval<T>()))::value;
};

}
}