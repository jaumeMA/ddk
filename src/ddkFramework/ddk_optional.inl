
namespace ddk
{
namespace detail
{

template<typename SuperClass>
optional_destructor<SuperClass,false>::~optional_destructor()
{
    static_cast<SuperClass*>(this)->destroy();
}

template<typename T>
constexpr variadic_union<none_t,T> construct_union(const optional<T>& i_opt)
{
    //here we rely on RVO
    if(i_opt.empty())
    {
        return {};
    }
    else
    {
        return { mpl::class_holder<T>{},i_opt.get() };
    }
}
template<typename T>
constexpr variadic_union<none_t, T> construct_union(optional<T>&& i_opt)
{
    //here we rely on RVO
    if (i_opt.empty())
    {
        return {};
    }
    else
    {
        return { mpl::class_holder<T>{},std::move(i_opt).extract() };
    }
}

}

template<typename T>
constexpr optional<T>::optional()
{
}
template<typename T>
constexpr optional<T>::optional(detail::none_t)
{
}
template<typename T>
constexpr optional<T>::optional(const optional<T>& other)
: m_storage(detail::construct_union(other))
, m_set(other.m_set)
{
}
template<typename T>
constexpr optional<T>::optional(optional<T>&& other)
: m_set(other.m_set)
{
    if (other.m_set)
    {
        other.m_set = false;
        m_storage.template construct<T>(std::move(other.m_storage).template extract<T>());
    }
}
template<typename T>
constexpr optional<T>::optional(reference_const_type i_val)
: m_storage(mpl::class_holder<T>{},i_val)
, m_set(true)
{
}
template<typename T>
constexpr optional<T>::optional(reference_type i_val)
: m_storage(mpl::class_holder<T>{},i_val)
, m_set(true)
{
}
template<typename T>
constexpr optional<T>::optional(rreference_type i_val)
: m_storage(mpl::class_holder<T>{}, std::move(i_val))
, m_set(true)
{
}
template<typename T>
TEMPLATE(typename TT)
REQUIRED(IS_CONSTRUCTIBLE(T,TT))
constexpr optional<T>::optional(const optional<TT>& other)
: m_set(true)
{
    if (other.m_set)
    {
		m_storage.template construct<T>(other.m_storage.template get<TT>());
    }
}
template<typename T>
TEMPLATE(typename TT)
REQUIRED(IS_CONSTRUCTIBLE(T,TT))
constexpr optional<T>::optional(optional<TT>&& other)
: m_set(true)
{
    if (other.m_set)
    {
		m_storage.template construct<T>(std::move(other.m_storage).template extract<TT>());
    }
}
template<typename T>
TEMPLATE(typename Arg)
REQUIRED(IS_CONSTRUCTIBLE(T,Arg))
constexpr optional<T>::optional(Arg&& i_arg)
: m_storage(mpl::class_holder<T>{},std::forward<Arg>(i_arg))
, m_set(true)
{
}
template<typename T>
constexpr void optional<T>::destroy()
{
    if (m_set)
    {
        m_set = false;
        m_storage.template destroy<T>();
    }
}
template<typename T>
constexpr optional<T>& optional<T>::operator=(const optional<T>& other)
{
    if (other.m_set)
    {
        (m_set) ? m_storage.template assign<T>(other.m_storage.template get<T>()) : m_storage.template construct<T>(other.m_storage.template get<T>());

        m_set = true;
    }
    else
    {
        destroy();
    }

    return *this;
}
template<typename T>
constexpr optional<T>& optional<T>::operator=(optional<T>&& other)
{
    if (other.m_set)
    {
        (m_set) ? m_storage.template assign<T>(std::move(other.m_storage).template extract<T>()) : m_storage.template construct<T>(std::move(other.m_storage).template extract<T>());

        m_set = true;

        other.m_set = false;
    }
    else
    {
        destroy();
    }

    return *this;
}
template<typename T>
TEMPLATE(typename TT)
REQUIRED(IS_ASSIGNABLE(T,TT),IS_CONSTRUCTIBLE(T,TT))
constexpr optional<T>& optional<T>::operator=(const optional<TT>& other)
{
    if (other.m_set)
    {
        (m_set) ? m_storage.template assign<T>(other.m_storage.template get<TT>()) : m_storage.template construct<T>(other.m_storage.template get<TT>());

        m_set = true;
    }
    else
    {
        destroy();
    }

    return *this;
}
template<typename T>
TEMPLATE(typename TT)
REQUIRED(IS_ASSIGNABLE(T,TT),IS_CONSTRUCTIBLE(T,TT))
constexpr optional<T>& optional<T>::operator=(optional<TT>&& other)
{
    if (other.m_set)
    {
        (m_set) ? m_storage.template assign<T>(std::move(other.m_storage).template extract<TT>()) : m_storage.template construct<T>(std::move(other.m_storage).template extract<TT>());

        m_set = true;

        other.m_set = false;
    }
    else
    {
        destroy();
    }

    return *this;
}
template<typename T>
constexpr optional<T>& optional<T>::operator=(detail::none_t)
{
    destroy();

    return *this;
}
template<typename T>
constexpr optional<T>& optional<T>::operator=(reference_type val)
{
    (m_set) ? m_storage.template assign<T>(val) : m_storage.template construct<T>(val);

    m_set = true;

    return *this;
}
template<typename T>
constexpr optional<T>& optional<T>::operator=(reference_const_type val)
{
    (m_set) ? m_storage.template assign<T>(val) : m_storage.template construct<T>(val);

    m_set = true;

    return *this;
}
template<typename T>
constexpr optional<T>& optional<T>::operator=(rreference_type val)
{
    (m_set) ? m_storage.template assign<T>(std::move(val)) : m_storage.template construct<T>(std::move(val));

    m_set = true;

    return *this;
}
template<typename T>
constexpr bool optional<T>::empty() const
{
    return m_set == false;
}
template<typename T>
constexpr optional<T>::operator bool() const
{
    return m_set;
}
template<typename T>
constexpr bool optional<T>::operator!() const
{
    return m_set == false;
}
template<typename T>
constexpr typename embedded_type<T>::cref_type optional<T>::get() const
{
	return m_storage.template get<T>();
}
template<typename T>
constexpr typename embedded_type<T>::ref_type optional<T>::get()
{
	return m_storage.template get<T>();
}
template<typename T>
constexpr typename embedded_type<T>::cref_type optional<T>::get_value_or(typename embedded_type<T>::cref_type default_value) const
{
    if (m_set)
    {
		return m_storage.template get<T>();
    }

    return default_value;
}
template<typename T>
constexpr typename embedded_type<T>::ref_type optional<T>::get_value_or(typename embedded_type<T>::ref_type default_value)
{
    if (m_set)
    {
		return m_storage.template get<T>();
    }

    return default_value;
}
template<typename T>
constexpr typename embedded_type<T>::cpointer_type optional<T>::get_ptr() const
{
    if (m_set)
    {
		return m_storage.template get<T>();
    }

    return nullptr;
}
template<typename T>
constexpr typename embedded_type<T>::pointer_type optional<T>::get_ptr()
{
    if (m_set)
    {
		return m_storage.template get<T>();
    }

    return nullptr;
}
template<typename T>
constexpr T optional<T>::extract() &&
{
	T res = std::move(m_storage).template extract<T>();

    m_set = false;

    return res;
}
template<typename T>
constexpr typename embedded_type<T>::cref_type optional<T>::operator*() const
{
	return m_storage.template get<T>();
}
template<typename T>
constexpr typename embedded_type<T>::ref_type optional<T>::operator*()
{
	return m_storage.template get<T>();
}
template<typename T>
constexpr typename embedded_type<T>::cpointer_type optional<T>::operator->() const
{
	return &(m_storage.template get<T>());
}
template<typename T>
constexpr typename embedded_type<T>::pointer_type optional<T>::operator->()
{
    return &(m_storage.template get<T>());
}
template<typename T>
constexpr void optional<T>::swap(const optional<T>& other)
{
    optional<T>& ncOther = const_cast<optional<T>&>(other);

    if (m_set && ncOther.m_set)
    {
		std::swap(m_storage.template get<T>(),other.m_storage.template get<T>());
    }
    else if (m_set)
    {
		other.m_storage.template construct<T>(m_storage.template extract<T>());
        ncOther.m_set = true;
    }
    else if (ncOther.m_set)
    {
		m_storage.template construct<T>(other.m_storage.template extract<T>());
        m_set = true;
    }
}

}
