
namespace ddk
{

template<typename Type,typename ... Types>
constexpr variadic_union<Type,Types...>::trivial_union_type::trivial_union_type()
{
}
template<typename Type,typename ... Types>
constexpr variadic_union<Type,Types...>::trivial_union_type::trivial_union_type(const detail::none_t&)
{
}
template<typename Type,typename ... Types>
TEMPLATE(typename ... Args)
REQUIRED(IS_CONSTRUCTIBLE(Type,Args...))
constexpr variadic_union<Type,Types...>::trivial_union_type::trivial_union_type(const  mpl::class_holder<Type>& i_holder, Args&& ... i_args)
: _local(std::forward<Args>(i_args)...)
{
}
template<typename Type,typename ... Types>
template<typename T,typename ... Args>
constexpr variadic_union<Type,Types...>::trivial_union_type::trivial_union_type(const mpl::class_holder<T>& i_holder,Args&& ... i_args)
: _nested(i_holder,std::forward<Args>(i_args)...)
{
}

template<typename Type,typename ... Types>
constexpr variadic_union<Type,Types...>::non_trivial_union_type::non_trivial_union_type()
{
}
template<typename Type,typename ... Types>
constexpr variadic_union<Type,Types...>::non_trivial_union_type::non_trivial_union_type(const detail::none_t&)
{
}
template<typename Type,typename ... Types>
TEMPLATE(typename ... Args)
REQUIRED(IS_CONSTRUCTIBLE(Type,Args...))
variadic_union<Type,Types...>::non_trivial_union_type::non_trivial_union_type(const  mpl::class_holder<Type>& i_holder,Args&& ... i_args)
: _local(std::forward<Args>(i_args)...)
{
}
template<typename Type,typename ... Types>
template<typename T,typename ... Args>
variadic_union<Type,Types...>::non_trivial_union_type::non_trivial_union_type(const mpl::class_holder<T>& i_holder,Args&& ... i_args)
: _nested(i_holder,std::forward<Args>(i_args)...)
{
}
template<typename Type,typename ... Types>
variadic_union<Type,Types...>::non_trivial_union_type::~non_trivial_union_type()
{
	//already handled by front-end
}

template<typename Type,typename ... Types>
constexpr variadic_union<Type,Types...>::variadic_union()
: variadic_union(typename mpl::static_if<std::is_default_constructible<Type>::value,std::true_type,std::false_type>::type{})
{
}
template<typename Type,typename ... Types>
constexpr variadic_union<Type,Types...>::variadic_union(const detail::none_t&)
{
}
template<typename Type,typename ... Types>
template<typename T, typename ... Args>
constexpr variadic_union<Type,Types...>::variadic_union(const mpl::class_holder<T>& i_holder, Args&& ... i_args)
: m_data(i_holder,std::forward<Args>(i_args)...)
{
}
template<typename Type,typename ... Types>
template<typename T,typename ... Args>
constexpr T& variadic_union<Type,Types...>::construct(Args&& ... i_args)
{
	if constexpr(std::is_same<Type,T>::value)
	{
		return m_data._local.inplace_construct(std::forward<Args>(i_args)...);
	}
	else
	{
		return m_data._nested.template construct<T>(std::forward<Args>(i_args)...);
	}
}
template<typename Type,typename ... Types>
template<typename T,typename ... Args>
constexpr T& variadic_union<Type,Types...>::assign(Args&& ... i_args)
{
	if constexpr(std::is_same<Type,T>::value)
	{
		return m_data._local.inplace_assign(std::forward<Args>(i_args)...);
	}
	else
	{
		return m_data._nested.template assign<T>(std::forward<Args>(i_args)...);
	}
}
template<typename Type,typename ... Types>
template<typename T>
void variadic_union<Type,Types...>::destroy()
{
	if constexpr(std::is_same<Type,T>::value)
	{
		m_data._local.inplace_destroy();
	}
	else
	{
		m_data._nested.template destroy<T>();
	}
}
template<typename Type,typename ... Types>
template<typename T>
constexpr typename embedded_type<T>::cref_type variadic_union<Type,Types...>::get() const
{
	if constexpr(std::is_same<Type,T>::value)
	{
		return m_data._local.get();
	}
	else
	{
		return m_data._nested.template get<T>();
	}
}
template<typename Type,typename ... Types>
template<typename T>
constexpr typename embedded_type<T>::ref_type variadic_union<Type,Types...>::get()
{
	if constexpr(std::is_same<Type,T>::value)
	{
		return m_data._local.get();
	}
	else
	{
		return m_data._nested.template get<T>();
	}
}
template<typename Type,typename ... Types>
template<typename T>
constexpr typename embedded_type<T>::cpointer_type variadic_union<Type,Types...>::get_ptr() const
{
	if constexpr (std::is_same<Type,T>::value)
	{
		return m_data._local.get_ptr();
	}
	else
	{
		return m_data._nested.template get_ptr<T>();
	}
}
template<typename Type,typename ... Types>
template<typename T>
constexpr typename embedded_type<T>::pointer_type variadic_union<Type,Types...>::get_ptr()
{
	if constexpr (std::is_same<Type,T>::value)
	{
		return m_data._local.get_ptr();
	}
	else
	{
		return m_data._nested.template get_ptr<T>();
	}
}
template<typename Type,typename ... Types>
template<typename T>
typename embedded_type<T>::rref_type variadic_union<Type,Types...>::extract() &&
{
	if constexpr(std::is_same<Type,T>::value)
	{
		return std::move(m_data._local).extract();
	}
	else
	{
		return std::move(m_data._nested).template extract<T>();
	}
}
template<typename Type,typename ... Types>
constexpr variadic_union<Type,Types...>::variadic_union(const std::true_type&)
: m_data(mpl::class_holder<Type>{},Type{})
{
}
template<typename Type,typename ... Types>
constexpr variadic_union<Type,Types...>::variadic_union(const std::false_type&)
{
}

}