//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_type_concepts.h"

#define ARE_TRAITS_COMPATIBLE_COND(_TRAITS1,_TRAITS2) \
	(ddk::concepts::are_traits_compatible<_TRAITS1,_TRAITS2>)
#define ARE_TRAITS_COMPATIBLE(_TRAITS1,_TRAITS2) \
	typename std::enable_if<ARE_TRAITS_COMPATIBLE_COND(_TRAITS1,_TRAITS2)>::type

#define ARE_TRAITS_EQUIVALENT_COND(_TRAITS1,_TRAITS2) \
	(ddk::concepts::are_traits_equivalent<_TRAITS1,_TRAITS2>)
#define ARE_TRAITS_EQUIVALENT(_TRAITS1,_TRAITS2) \
	typename std::enable_if<ARE_TRAITS_EQUIVALENT_COND(_TRAITS1,_TRAITS2)>::type

namespace ddk
{
namespace concepts
{

template<typename FromTraits, typename ToTraits>
struct check_traits_compatibility
{
private:
	typedef typename FromTraits::reference from_reference;
	typedef typename ToTraits::reference to_reference;
	typedef typename FromTraits::const_reference from_const_reference;
	typedef typename ToTraits::const_reference to_const_reference;
	typedef typename FromTraits::tags_t from_tags_t;
	typedef typename ToTraits::tags_t to_tags_t;
	typedef typename FromTraits::const_tags_t from_const_tags_t;
	typedef typename ToTraits::const_tags_t to_const_tags_t;

public:
	static const bool value = IS_CONVERTIBLE_COND(from_reference,to_reference) &&
								IS_CONVERTIBLE_COND(from_const_reference,to_const_reference) &&
								from_tags_t::template projects(to_tags_t{}) &&
								from_const_tags_t::template projects(to_const_tags_t{});
};

template<typename Traits1, typename Traits2>
inline constexpr bool are_traits_compatible = check_traits_compatibility<Traits1,Traits2>::value;

template<typename FromTraits,typename ToTraits>
struct check_traits_equivalence
{
private:
	typedef typename FromTraits::reference from_reference;
	typedef typename ToTraits::reference to_reference;
	typedef typename FromTraits::const_reference from_const_reference;
	typedef typename ToTraits::const_reference to_const_reference;
	typedef typename FromTraits::tags_t from_tags_t;
	typedef typename ToTraits::tags_t to_tags_t;
	typedef typename FromTraits::const_tags_t from_const_tags_t;
	typedef typename ToTraits::const_tags_t to_const_tags_t;

public:
	static const bool value = IS_CONVERTIBLE_COND(from_reference,to_reference) &&
								IS_CONVERTIBLE_COND(from_const_reference,to_const_reference) &&
								IS_CONVERTIBLE_COND(to_reference,from_reference) &&
								IS_CONVERTIBLE_COND(to_const_reference,from_const_reference) &&
								from_tags_t::template projects(to_tags_t{}) &&
								from_const_tags_t::template projects(to_const_tags_t{}) &&
								to_tags_t::template projects(from_tags_t{}) &&
								to_const_tags_t::template projects(from_const_tags_t{});
};

template<typename Traits1,typename Traits2>
inline constexpr bool are_traits_equivalent = check_traits_equivalence<Traits1,Traits2>::value;

}
}