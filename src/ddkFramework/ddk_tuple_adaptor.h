#pragma once

#include "ddk_tuple.h"
#include "ddk_variant.h"

namespace ddk
{

template<typename ... T>
class tuple_adaptor
{
	static const size_t s_numTypes = tuple<T...>::size();
	typedef typed_arena<variant<typename embedded_type<T>::ref_type ...>> cached_value_t;

public:
	typedef variant<typename embedded_type<T>::ref_type ...> reference;
	typedef variant<typename embedded_type<T>::cref_type ...> const_reference;

	tuple_adaptor(tuple<T...>& i_iterable,const ddk::iter::shift_action& i_initialAction);
	~tuple_adaptor();
	inline reference get_value() noexcept;
	inline const_reference get_value() const noexcept;
	inline ddk::optional<reference> next_value() noexcept;
	inline ddk::optional<const_reference> next_value() const noexcept;
	inline ddk::optional<reference> prev_value() noexcept;
	inline ddk::optional<const_reference> prev_value() const noexcept;
	inline ddk::optional<reference> shift_value(int i_shift) noexcept;
	inline ddk::optional<const_reference> shift_value(int i_shift) const noexcept;
	inline bool valid() const noexcept;

private:
	template<size_t ... Indexs>
	inline reference get(const mpl::sequence<Indexs...>&);

	template<size_t ... Indexs>
	inline const_reference get(const mpl::sequence<Indexs...>&) const;

	template<size_t Index>
	inline static reference _get(tuple<T...>&, cached_value_t&);

	template<size_t Index>
	inline static const_reference _get(const tuple<T...>&, cached_value_t&);

	tuple<T...>& m_iterable;
	mutable size_t m_currIndex = 0;
	mutable cached_value_t m_currValue;
};

}

#include "ddk_tuple_adaptor.inl"