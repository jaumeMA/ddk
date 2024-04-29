//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_iterable_action.h"
#include "ddk_function.h"

namespace ddk
{
namespace detail
{

template<typename Generator>
struct __numbers_iterable
{
public:
    typedef typename mpl::aqcuire_callable_return_type<Generator>::type value_type;

    static_assert(IS_NUMERIC_COND(value_type), "You shall provide a numeric type.");

    __numbers_iterable(const Generator& i_generator);

    template<typename GGenerator>
    inline __numbers_iterable<GGenerator> operator()(const GGenerator& i_generator) const;
    auto operator()(const value_type& i_value) const;

private:
    const Generator m_generator;
};
template<typename Generator>
__numbers_iterable(const Generator&) -> __numbers_iterable<Generator>;

}

template<typename Generator>
class iterable_adaptor<const detail::__numbers_iterable<Generator>>
{
    typedef typename detail::__numbers_iterable<Generator>::value_type underlying_type;

public:
    typedef mpl::empty_type_pack tags_t;
    typedef mpl::type_pack<agnostic_sink_action_tag<const underlying_type&>,begin_action_tag,forward_action_tag> const_tags_t;
    typedef detail::iterable_by_type_adaptor<const underlying_type,tags_t,const_tags_t> traits;
    typedef detail::const_iterable_traits<traits> const_traits;

    iterable_adaptor(const detail::__numbers_iterable<Generator>& i_generator);
    template<typename Adaptor, typename Sink>
    static inline iterable_action_tag_result<traits,sink_action_tag<Sink>> perform_action(Adaptor&& i_adaptor, const sink_action_tag<Sink>& i_sink);
    template<typename Adaptor>
    static inline iterable_action_tag_result<traits,begin_action_tag> perform_action(Adaptor&& i_adaptor, const begin_action_tag&);
    template<typename Adaptor>
    static inline iterable_action_tag_result<traits,forward_action_tag> perform_action(Adaptor&& i_adaptor, const forward_action_tag&);

private:
    mutable underlying_type m_currValue = 0;
    const detail::__numbers_iterable<Generator> m_generator;
};

template<typename Generator>
class iterable_adaptor<detail::__numbers_iterable<Generator>> : public iterable_adaptor<const detail::__numbers_iterable<Generator>>
{
public:
    using iterable_adaptor<const detail::__numbers_iterable<Generator>>::traits;
    using iterable_adaptor<const detail::__numbers_iterable<Generator>>::const_traits;
    using iterable_adaptor<const detail::__numbers_iterable<Generator>>::tags_t;
    using iterable_adaptor<const detail::__numbers_iterable<Generator>>::const_tags_t;

    using iterable_adaptor<const detail::__numbers_iterable<Generator>>::iterable_adaptor;
};

extern const const_forward_iterable<const int> integers;
extern const const_forward_iterable<const int> odd_integers;
extern const const_forward_iterable<const int> even_integers;

extern const const_forward_iterable<const int> naturals;
extern const const_forward_iterable<const int> odd_naturals;
extern const const_forward_iterable<const int> even_naturals;

extern const const_forward_iterable<const float> reals;
extern const const_forward_iterable<const float> odd_reals;
extern const const_forward_iterable<const float> even_reals;

}
