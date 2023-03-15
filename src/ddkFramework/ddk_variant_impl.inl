#pragma once

#include "ddk_visitor_invoker.h"
#include "ddk_static_builtin_visitors.h"


#define CREATE_INNER_VISITOR(_visitor,_types) \
    typedef typename mpl::make_sequence<0,mpl::get_num_types<_types...>()>::type _range_seq_t; \
	typedef decltype(_visitor) visitor_type; \
	typedef typename visitor_type::return_type return_type; \
    typedef variant_visitor_invoker<return_type,_types...> _variant_visitor_t;

#define CALL_INNER_VISITOR(_visitor,_variant) \
    _variant_visitor_t::template inner_invoker(_range_seq_t{},_visitor,_variant); \

namespace ddk
{
namespace detail
{

template<typename ... Types, typename ... TTypes>
inline constexpr variadic_union<Types...> construct_union(const mpl::type_pack<Types...>&, const variant_impl<TTypes...>& other)
{
	//here we rely on RVO
	if (other.m_currentType < variant_impl<TTypes...>::s_numTypes)
	{
		typedef typename mpl::make_sequence<0, mpl::get_num_types<TTypes...>()>::type _range_seq_t;
		typedef typename constructor_visitor<variadic_union<Types...>, TTypes...>::return_type return_type;
		typedef variant_visitor_invoker<return_type,TTypes...> _variant_visitor_t;

		constructor_visitor<variadic_union<Types...>,TTypes...> ctr;

		return _variant_visitor_t::template inner_invoker(_range_seq_t{},ctr,other);
	}
	else
	{
		return none;
	}
}
template<typename ... Types, typename ... TTypes>
inline constexpr variadic_union<Types...> construct_union(const mpl::type_pack<Types...>&, variant_impl<TTypes...>&& other)
{
	//here we rely on RVO
	if (other.m_currentType < variant_impl<TTypes...>::s_numTypes)
	{
		typedef typename mpl::make_sequence<0, mpl::get_num_types<TTypes...>()>::type _range_seq_t;
		typedef typename constructor_visitor<variadic_union<Types...>, TTypes...>::return_type return_type;
		typedef variant_visitor_invoker<return_type, TTypes...> _variant_visitor_t;

		constructor_visitor<variadic_union<Types...>, TTypes...> ctr;

		return _variant_visitor_t::template inner_invoker(_range_seq_t{},ctr,std::move(other));
	}
	else
	{
		return none;
	}
}
template<typename ... Types, typename ... TTypes>
inline constexpr variadic_union<Types...>& construct_union(variadic_union<Types...>& i_storage, const variant_impl<TTypes...>& other)
{
	if (other.m_currentType < variant_impl<TTypes...>::s_numTypes)
	{
		typedef typename mpl::make_sequence<0, mpl::get_num_types<TTypes...>()>::type _range_seq_t;
		typedef typename constructor_inplace_visitor<variadic_union<Types...>,TTypes...>::return_type return_type;
		typedef variant_visitor_invoker<return_type,TTypes...> _variant_visitor_t;

		constructor_inplace_visitor<variadic_union<Types...>,TTypes...> ctr(i_storage);

		_variant_visitor_t::template inner_invoker(_range_seq_t{},ctr,other);
	}

	return i_storage;
}
template<typename ... Types, typename ... TTypes>
inline constexpr variadic_union<Types...>& construct_union(variadic_union<Types...>& i_storage, variant_impl<TTypes...>&& other)
{
	if (other.m_currentType < variant_impl<TTypes...>::s_numTypes)
	{
		typedef typename mpl::make_sequence<0, mpl::get_num_types<TTypes...>()>::type _range_seq_t;
		typedef typename constructor_inplace_visitor<variadic_union<Types...>, TTypes...>::return_type return_type;
		typedef variant_visitor_invoker<return_type,TTypes...> _variant_visitor_t;

		constructor_inplace_visitor<variadic_union<Types...>, TTypes...> ctr(i_storage);

		_variant_visitor_t::template inner_invoker(_range_seq_t{},ctr,std::move(other));
	}

	return i_storage;
}

template<typename SuperClass>
variant_impl_destructor<SuperClass,false>::~variant_impl_destructor()
{
	static_cast<SuperClass*>(this)->destroy();
}

template<typename ... Types>
const mpl::type_pack<Types...> variant_impl<Types...>::s_types = mpl::type_pack<Types...>{};

template<typename ... Types>
constexpr variant_impl<Types...>::variant_impl()
: m_currentType((std::is_default_constructible<typename mpl::nth_type_of<0,Types...>::type>::value) ? 0 : s_numTypes)
{
}
template<typename ... Types>
template<size_t Index, typename TType>
constexpr variant_impl<Types...>::variant_impl(const mpl::static_number<Index>&, TType&& other)
: m_currentType(Index)
, m_storage(mpl::class_holder<mpl::nth_type_of_t<Index,Types...>>{},std::forward<TType>(other))
{
}
template<typename ... Types>
constexpr variant_impl<Types...>::variant_impl(const variant_impl<Types...>& other)
: m_currentType(other.m_currentType)
, m_storage(construct_union(s_types,other))
{
}
template<typename ... Types>
constexpr variant_impl<Types...>::variant_impl(variant_impl<Types...>&& other)
: m_currentType(other.m_currentType)
, m_storage(construct_union(s_types,std::move(other)))
{
	other.m_currentType = s_numTypes;
}
template<typename ... Types>
template<typename ... TTypes>
constexpr variant_impl<Types...>::variant_impl(const variant_impl<TTypes...>& other)
: m_currentType(other.m_currentType)
, m_storage(construct_union(s_types,other))
{
}
template<typename ... Types>
template<typename ... TTypes>
constexpr variant_impl<Types...>::variant_impl(variant_impl<TTypes...>&& other)
: m_currentType(other.m_currentType)
, m_storage(construct_union(s_types,std::move(other)))
{
	other.m_currentType = s_numTypes;
}
template<typename ... Types>
void variant_impl<Types...>::destroy()
{
	if (m_currentType < s_numTypes)
	{
		destructor_visitor<data_type,Types...> dtr(m_storage);

		CREATE_INNER_VISITOR(dtr,Types);
		CALL_INNER_VISITOR(dtr,*this);

		m_currentType = s_numTypes;
	}
}
template<typename ... Types>
variant_impl<Types...>& variant_impl<Types...>::operator=(const variant_impl<Types...>& other)
{
	if (other.m_currentType < s_numTypes)
	{
		if (m_currentType == other.m_currentType)
		{
			assigner_visitor<data_type,Types...> ctr(m_storage);

			CREATE_INNER_VISITOR(ctr,Types);
			CALL_INNER_VISITOR(ctr,other);
		}
		else
		{
			destroy();

			m_currentType = other.m_currentType;

			construct_union(m_storage,other);
		}
	}
	else
	{
		destroy();
	}

	return *this;
}
template<typename ... Types>
variant_impl<Types...>& variant_impl<Types...>::operator=(variant_impl<Types...>&& other)
{
	if (other.m_currentType < s_numTypes)
	{
		if (m_currentType == other.m_currentType)
		{
			assigner_visitor<data_type,Types...> ctr(m_storage);

			CREATE_INNER_VISITOR(ctr,Types);
			CALL_INNER_VISITOR(ctr,std::move(other));
		}
		else
		{
			destroy();

			m_currentType = other.m_currentType;

			construct_union(m_storage,std::move(other));
		}
	}
	else
	{
		destroy();
	}

	return *this;
}
template<typename ... Types>
template<typename ... TTypes>
variant_impl<Types...>& variant_impl<Types...>::operator=(const variant_impl<TTypes...>& other)
{
	if (other.m_currentType < s_numTypes)
	{
		if (m_currentType == other.m_currentType)
		{
			assigner_visitor<data_type,Types...> ctr(m_storage);

			CREATE_INNER_VISITOR(ctr,TTypes);
			CALL_INNER_VISITOR(ctr,other);
		}
		else
		{
			destroy();

			m_currentType = other.m_currentType;

			construct_union(m_storage,other);
		}
	}
	else
	{
		destroy();
	}

	return *this;
}
template<typename ... Types>
template<typename ... TTypes>
variant_impl<Types...>& variant_impl<Types...>::operator=(variant_impl<TTypes...>&& other)
{
	if (other.m_currentType < s_numTypes)
	{
		if (m_currentType == other.m_currentType)
		{
			assigner_visitor<data_type,Types...> ctr(m_storage);

			CREATE_INNER_VISITOR(ctr,TTypes);
			CALL_INNER_VISITOR(ctr,std::move(other));
		}
		else
		{
			destroy();

			m_currentType = other.m_currentType;

			construct_union(m_storage, std::move(other));
		}
	}
	else
	{
		destroy();
	}

	return *this;
}
template<typename ... Types>
template<size_t Index, typename TType>
variant_impl<Types...>& variant_impl<Types...>::assign(TType&& val)
{
	static_assert(Index >= 0 && Index < s_numTypes, "Type out of bounds!");

	if (Index != m_currentType)
	{
		destroy();

		constructor_inplace_visitor<data_type, Types...> ctr(m_storage);

		ctr.template operator()<Index>(std::forward<TType>(val));

		m_currentType = static_cast<unsigned char>(Index);
	}
	else if (m_currentType < s_numTypes)
	{
		//just an assignment
		assigner_visitor<data_type, Types...> ass(m_storage);

		ass.template operator()<Index>(std::forward<TType>(val));
	}

	return *this;
}
template<typename ... Types>
bool variant_impl<Types...>::operator==(const variant_impl<Types...>& other) const
{
	bool res = false;

	if (m_currentType == other.m_currentType)
	{
		if(m_currentType != s_numTypes)
		{
			comparison_visitor<variant_impl<Types...>,Types...> comparator(*this);

			CREATE_INNER_VISITOR(comparator,Types);
			res = CALL_INNER_VISITOR(comparator,other);
		}
		else
		{
			//empty case, valid case (both were empty)
			res = true;
		}
	}

	return res;
}
template<typename ... Types>
bool variant_impl<Types...>::operator==(variant_impl<Types...>&& other) const
{
	bool res = false;

	if (m_currentType == other.m_currentType)
	{
		comparison_visitor<variant_impl<Types...>,Types...> comparator(*this);

		CREATE_INNER_VISITOR(comparator,Types);
		res = CALL_INNER_VISITOR(comparator,other);
	}

	return res;
}
template<typename ... Types>
bool variant_impl<Types...>::operator!=(const variant_impl<Types...>& other) const
{
	bool res = true;

	if (m_currentType == other.m_currentType)
	{
		comparison_visitor<variant_impl<Types...>,Types...> comparator(*this);

		CREATE_INNER_VISITOR(comparator,Types);
		res = !CALL_INNER_VISITOR(comparator,other);
	}

	return res;
}
template<typename ... Types>
bool variant_impl<Types...>::operator!=(variant_impl<Types...>&& other) const
{
	bool res = true;

	if (m_currentType == other.m_currentType)
	{
		comparison_visitor<variant_impl<Types...>,Types...> comparator(*this);

		CREATE_INNER_VISITOR(comparator,Types);
		res = !CALL_INNER_VISITOR(comparator,other);
	}

	return res;
}
template<typename ... Types>
template<size_t Index, typename TType>
constexpr bool variant_impl<Types...>::compare(TType&& other) const
{
	bool res = false;

	if (m_currentType == static_cast<unsigned char>(Index))
	{
		typedef typename std::add_const<typename mpl::nth_type_of<Index, Types...>::type>::type currType;
		typedef typename embedded_type<currType>::cref_type ref_type;

		const val_retriever_visitor<currType,Types...> getter;

		CREATE_INNER_VISITOR(getter,Types);
		ref_type thisVal = CALL_INNER_VISITOR(getter,*this);

		res = (thisVal == std::forward<TType>(other));
	}

	return res;
}
template<typename ... Types>
template<typename TType>
constexpr typename embedded_type<TType>::cref_type variant_impl<Types...>::get() const
{
	DDK_ASSERT(is<TType>(), "Accessing non current type!");

	return m_storage.template get<TType>();
}
template<typename ... Types>
template<typename TType>
constexpr typename embedded_type<TType>::ref_type variant_impl<Types...>::get()
{
	DDK_ASSERT(is<TType>(), "Accessing non current type!");

	return m_storage.template get<TType>();
}
template<typename ... Types>
template<typename TType>
constexpr typename embedded_type<TType>::cpointer_type variant_impl<Types...>::get_ptr() const
{
	DDK_ASSERT(is<TType>(),"Accessing non current type!");

	return m_storage.template get_ptr<TType>();
}
template<typename ... Types>
template<typename TType>
constexpr typename embedded_type<TType>::pointer_type variant_impl<Types...>::get_ptr()
{
	DDK_ASSERT(is<TType>(),"Accessing non current type!");

	return m_storage.template get_ptr<TType>();
}
template<typename ... Types>
template<typename TType>
TType variant_impl<Types...>::extract() &&
{
	TType res = std::move(m_storage).template extract<TType>();

	destroy();

	return std::forward<TType>(res);
}
template<typename ... Types>
template<typename TType>
constexpr bool variant_impl<Types...>::is() const
{
	if constexpr (mpl::is_among_types<TType, Types...>)
	{
		return m_currentType == pos_of_type<TType>();
	}
	else
	{
		return false;
	}
}
template<typename ... Types>
template<typename TType>
constexpr unsigned char variant_impl<Types...>::pos_of_type()
{
	return static_cast<unsigned char>(mpl::pos_of_type<TType,Types...>);
}
template<typename ... Types>
template<typename TType>
constexpr typename embedded_type<TType>::cref_type variant_impl<Types...>::get_as() const
{
	typedef typename std::add_const<TType>::type ret_type;

	const val_retriever_visitor<ret_type,Types...> getter;

	CREATE_INNER_VISITOR(getter,Types);

	return CALL_INNER_VISITOR(getter,*this);
}
template<typename ... Types>
template<typename TType>
constexpr typename embedded_type<TType>::ref_type variant_impl<Types...>::get_as()
{
	val_retriever_visitor<TType,Types...> getter;

	CREATE_INNER_VISITOR(getter,Types);

	return CALL_INNER_VISITOR(getter,*this);
}
template<typename ... Types>
template<typename TType>
constexpr bool variant_impl<Types...>::is_base_of() const
{
	const is_base_of_visitor<TType,Types...> isBaseOf;

	CREATE_INNER_VISITOR(isBaseOf,Types);

	return CALL_INNER_VISITOR(isBaseOf,*this);
}
template<typename ... Types>
template<size_t Pos>
constexpr typename embedded_type<typename mpl::nth_type_of<Pos,Types...>::type>::cref_type variant_impl<Types...>::get() const
{
	typedef typename mpl::nth_type_of<Pos,Types...>::type embeddedType;

	return m_storage.template get<embeddedType>();
}
template<typename ... Types>
template<size_t Pos>
constexpr typename embedded_type<typename mpl::nth_type_of<Pos,Types...>::type>::ref_type variant_impl<Types...>::get()
{
	typedef typename mpl::nth_type_of<Pos,Types...>::type embeddedType;

	return m_storage.template get<embeddedType>();
}
template<typename ... Types>
template<size_t Pos>
constexpr typename embedded_type<typename mpl::nth_type_of<Pos,Types...>::type>::cpointer_type variant_impl<Types...>::get_ptr() const
{
	typedef typename mpl::nth_type_of<Pos,Types...>::type embeddedType;

	return m_storage.template get_ptr<embeddedType>();
}
template<typename ... Types>
template<size_t Pos>
constexpr typename embedded_type<typename mpl::nth_type_of<Pos,Types...>::type>::pointer_type variant_impl<Types...>::get_ptr()
{
	typedef typename mpl::nth_type_of<Pos,Types...>::type embeddedType;

	return m_storage.template get_ptr<embeddedType>();
}
template<typename ... Types>
template<size_t Pos>
embedded_type<typename mpl::nth_type_of<Pos,Types...>::type> variant_impl<Types...>::extract() &&
{
	typedef typename mpl::nth_type_of<Pos,Types...>::type embeddedType;

	m_currentType = s_numTypes;

	return embedded_type<typename mpl::nth_type_of<Pos,Types...>::type>{ std::move(m_storage).template extract<embeddedType>() };
}
template<typename ... Types>
template<size_t Pos>
constexpr bool variant_impl<Types...>::is() const
{
	return m_currentType == Pos;
}
template<typename ... Types>
constexpr unsigned char variant_impl<Types...>::which() const
{
	return m_currentType;
}
template<typename ... Types>
void variant_impl<Types...>::swap(variant_impl<Types...>& other)
{
	if (m_currentType < s_numTypes && other.m_currentType < s_numTypes)
	{
		//as in copy construction case, just check types are the same or not
		if (m_currentType == other.m_currentType)
		{
			swaper_visitor<variant_impl<Types...>,Types...> letsSwap(other, *this);

			CREATE_INNER_VISITOR(letsSwap,Types);
			CALL_INNER_VISITOR(letsSwap,*this);
		}
		else
		{
			//mmm, we will have to reconstruct types (at least, do it with move semantics in case we may save some allocations)

			//construction of tmp
			variant_impl<Types...> tmp;

			tmp.m_currentType = m_currentType;

			construct_union(tmp.m_storage,*this);

			//reconstruction of us
			*this = other;

			//reconstruction of other
			other = tmp;
		}
	}
	else if (m_currentType < s_numTypes)
	{
		//construct us with other data
		construct_union(other.m_storage,*this);
	}
	else if (other.m_currentType < s_numTypes)
	{
		//construct other with our data
		construct_union(m_storage,other);
	}
}
template<typename ... Types>
TEMPLATE(typename Visitor)
REQUIRED(IS_CALLABLE(Visitor,Types)...)
constexpr auto variant_impl<Types...>::visit(Visitor&& visitor) &
{
	typedef typename mpl::nth_type_of<0,Types...>::type first_type;
	typedef decltype(std::declval<Visitor>()(std::declval<first_type>())) return_type;

	static_assert((std::is_same<decltype(std::declval<Visitor>()(std::declval<Types>())),return_type>::value && ...), "You shall provide a uniform return type callable.");

	typedef typename mpl::make_sequence<0,mpl::get_num_types<Types...>()>::type range_seq;

	if constexpr(std::is_same<void,return_type>::value)
	{
		variant_visitor_invoker<return_type,Types...>::template outer_invoker(range_seq{},const_cast<Visitor&>(visitor),*this);
	}
	else
	{
		return variant_visitor_invoker<return_type,Types...>::template outer_invoker(range_seq{},const_cast<Visitor&>(visitor),*this);
	}
}
template<typename ... Types>
TEMPLATE(typename Visitor)
REQUIRED(IS_CALLABLE(Visitor,Types)...)
constexpr auto variant_impl<Types...>::visit(Visitor&& visitor) const &
{
	typedef typename mpl::nth_type_of<0,Types...>::type first_type;
	typedef decltype(std::declval<Visitor>()(std::declval<first_type>())) return_type;

	static_assert((std::is_same<decltype(std::declval<Visitor>()(std::declval<Types>())),return_type>::value && ...),"You shall provide a uniform return type callable.");

	typedef typename mpl::make_sequence<0,mpl::get_num_types<Types...>()>::type range_seq;

	if constexpr(std::is_same<void,return_type>::value)
	{
		variant_visitor_invoker<return_type,Types...>::template outer_invoker(range_seq{},visitor,*this);
	}
	else
	{
		return variant_visitor_invoker<return_type,Types...>::template outer_invoker(range_seq{},visitor,*this);
	}
}
template<typename ... Types>
TEMPLATE(typename Visitor)
REQUIRED(IS_CALLABLE(Visitor,Types)...)
constexpr auto variant_impl<Types...>::visit(Visitor&& visitor) &&
{
	typedef typename mpl::nth_type_of<0,Types...>::type first_type;
	typedef decltype(std::declval<Visitor>()(std::declval<first_type>())) return_type;

	static_assert((std::is_same<decltype(std::declval<Visitor>()(std::declval<Types>())),return_type>::value && ...),"You shall provide a uniform return type callable.");

	typedef typename mpl::make_sequence<0,mpl::get_num_types<Types...>()>::type range_seq;

	if constexpr(std::is_same<void,return_type>::value)
	{
		variant_visitor_invoker<return_type,Types...>::template outer_invoker(range_seq{},const_cast<Visitor&>(visitor),std::move(*this));
	}
	else
	{
		return variant_visitor_invoker<return_type,Types...>::template outer_invoker(range_seq{},const_cast<Visitor&>(visitor),std::move(*this));
	}
}
template<typename ... Types>
template<typename Visitor,typename ... Args>
constexpr auto variant_impl<Types...>::visit(Args&& ... i_args) const
{
	typedef typename mpl::nth_type_of<0,Types...>::type first_type;
	typedef decltype(std::declval<Visitor>()(std::declval<first_type>())) return_type;

	static_assert((std::is_same<decltype(std::declval<Visitor>()(std::declval<Types>())),return_type>::value && ...),"You shall provide a uniform return type callable.");

	typedef typename mpl::make_sequence<0,mpl::get_num_types<Types...>()>::type range_seq;

	const Visitor visitor(std::forward<Args>(i_args)...);

	if constexpr(std::is_same<void,return_type>::value)
	{
		variant_visitor_invoker<return_type,Types...>::template outer_invoker(range_seq{},visitor,*this);
	}
	else
	{
		return variant_visitor_invoker<return_type,Types...>::template outer_invoker(range_seq{},visitor,*this);
	}
}

}
}
