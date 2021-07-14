#pragma once

#include "ddk_iterable_action.h"
#include "ddk_container_concepts.h"

namespace ddk
{
namespace detail
{

class reversable_action_resolver
{
public:
	reversable_action_resolver(bool i_forward);

	template<typename Iterable>
	reversable_action_resolver init(Iterable&& i_iterable) const;
	template<typename ... Types>
	variant<Types...> resolve(const variant<Types...>& i_action) const;
	shift_action resolve(const shift_action& i_action) const;

protected:
	mutable bool m_forward;
};

class alternate_action_resolver : reversable_action_resolver
{
public:
	using reversable_action_resolver::resolve;

	alternate_action_resolver(bool i_forward);

	template<typename Iterable>
	alternate_action_resolver init(Iterable&& i_iterable) const;
	template<typename ... Types>
	variant<Types...> resolve(const variant<Types...>& i_action) const;
	alternate_action_resolver operator()(size_t i_constrain) const;

private:
	alternate_action_resolver(size_t i_constrain, bool i_forward);

	const size_t m_constrain = 0;
	mutable int m_currIndex = 0;
};

class multi_dimensional_action_resolver : public reversable_action_resolver
{
public:
	enum Type
	{
		Forward,
		Transpose
	};

	using reversable_action_resolver::resolve;

	multi_dimensional_action_resolver(bool i_forward);
	multi_dimensional_action_resolver(bool i_forward, Type i_bottomToTop);

	TEMPLATE(typename Iterable)
	REQUIRES(IS_DIMENSIONABLE(Iterable))
	multi_dimensional_action_resolver init(Iterable&& i_iterable) const;
	template<typename ... Types>
	variant<Types...> resolve(const variant<Types...>& i_action) const;

private:
	multi_dimensional_action_resolver(bool i_forward,Type i_bottomToTop, const std::vector<size_t>& i_dimensions);

	size_t get_curr_item() const;
	size_t get_next_item() const;
	size_t get_prev_item() const;

	const Type m_order = Forward;
	const std::vector<size_t> m_constrains;
	mutable std::vector<size_t> m_currIndex;
	mutable size_t m_currDimension = 0;
};

}

extern const detail::reversable_action_resolver forward_order;
extern const detail::reversable_action_resolver reverse_order;
extern const detail::alternate_action_resolver alternate_order;
extern const detail::multi_dimensional_action_resolver dimension_order;
extern const detail::multi_dimensional_action_resolver transponse_dimension_order;

}

#include "ddk_iterable_action_resolver.inl"