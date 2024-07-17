//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_template_helper.h"
#include "ddk_type_concepts.h"
#include "ddk_concepts.h"

namespace ddk
{
namespace detail
{

template<typename T, typename TT>
inline constexpr bool inherit_from_first = ((sizeof(T) <= sizeof(TT)) && std::is_class<T>::value) || std::is_class<TT>::value == false;

template<typename First, typename Second>
class compressed_pair : public mpl::which_type<inherit_from_first<First,Second>,First,Second>::type
{
	static constexpr bool isFirstBased = inherit_from_first<First,Second>;
	typedef typename mpl::static_if<isFirstBased,First,Second>::type base_type;
	typedef typename mpl::static_if<isFirstBased,Second,First>::type member_type;
public:
	compressed_pair() = default;
	TEMPLATE(typename FFirst, typename SSecond)
	REQUIRES(IS_CONSTRUCTIBLE(First,FFirst),IS_CONSTRUCTIBLE(Second,SSecond))
	compressed_pair(FFirst&& i_first, SSecond&& i_second);

	template<typename FFirst>
	void set_first(FFirst&& i_first);
	template<typename SSecond>
	void set_second(SSecond&& i_second);

	First& get_first();
	const First& get_first() const;
	Second& get_second();
	const Second get_second() const;

private:
	member_type m_member;
};

}
}

#include "ddk_compressed_pair.inl"