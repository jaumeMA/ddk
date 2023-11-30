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
    typedef const underlying_type value_type;
    typedef value_type& reference;
    typedef value_type& const_reference;
    typedef long long difference_type;
    typedef mpl::type_pack<begin_action_tag,forward_action_tag> tags_t;

    iterable_adaptor(const detail::__numbers_iterable<Generator>& i_generator);
    inline bool valid() const;
    inline reference get_value();
    inline const_reference get_value() const;
    template<typename Sink>
    inline auto forward_value(Sink&& i_sink);
    template<typename Sink>
    inline auto forward_value(Sink&& i_sink) const;
    inline iterable_action_result<begin_action_tag> perform_action(const begin_action_tag&) const;
    inline iterable_action_result<forward_action_tag> perform_action(const forward_action_tag&) const;

private:
    mutable underlying_type m_currValue = 0;
    const detail::__numbers_iterable<Generator> m_generator;
};

template<typename Generator>
class iterable_adaptor<detail::__numbers_iterable<Generator>> : public iterable_adaptor<const detail::__numbers_iterable<Generator>>
{
public:
    using iterable_adaptor<const detail::__numbers_iterable<Generator>>::value_type;
    using iterable_adaptor<const detail::__numbers_iterable<Generator>>::reference;
    using iterable_adaptor<const detail::__numbers_iterable<Generator>>::const_reference;
    using iterable_adaptor<const detail::__numbers_iterable<Generator>>::difference_type;
    using iterable_adaptor<const detail::__numbers_iterable<Generator>>::tags_t;

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
