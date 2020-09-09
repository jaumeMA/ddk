
#define PACK_STORAGE_ADDRESS(_STORAGE) reinterpret_cast<char*>(const_cast<decltype(_STORAGE)*>(&_STORAGE))

namespace ddk
{
namespace detail
{

template<typename Type>
template<size_t IIndex, typename Arg>
tuple_impl<mpl::sequence<0>,Type>::tuple_impl(const mpl::sequence<IIndex>&, Arg&& i_arg)
: m_val(std::forward<Arg>(i_arg))
{
	static_assert(IIndex==0, "Index out of bounds");
}
template<typename Type>
template<typename Arg>
tuple_impl<mpl::sequence<0>,Type>::tuple_impl(Arg&& i_val)
: m_val(std::forward<Arg>(i_val))
{
}
template<typename Type>
template<typename TType>
tuple_impl<mpl::sequence<0>,Type>::tuple_impl(const tuple_impl<mpl::sequence<0>,TType>& other)
: m_val(other.m_val)
{
}
template<typename Type>
template<typename TType>
tuple_impl<mpl::sequence<0>,Type>::tuple_impl(tuple_impl<mpl::sequence<0>,TType>&& other)
: m_val(std::move(other.m_val))
{
}
template<typename Type>
template<size_t IIndex>
typename embedded_type<Type>::cref_type tuple_impl<mpl::sequence<0>,Type>::get() const
{
	return m_val;
}
template<typename Type>
template<size_t IIndex>
typename embedded_type<Type>::ref_type tuple_impl<mpl::sequence<0>,Type>::get()
{
	return m_val;
}
template<typename Type>
template<size_t Index, typename Arg>
void tuple_impl<mpl::sequence<0>,Type>::set(Arg&& i_arg)
{
	m_val = std::forward<Arg>(i_arg);
}
template<typename Type>
constexpr size_t tuple_impl<mpl::sequence<0>,Type>::size()
{
	return 1;
}

template<size_t Index, size_t ... Indexs,typename Type, typename ... Types>
template<size_t IIndex, size_t ... IIndexs, typename Arg, typename ... Args>
tuple_impl<mpl::sequence<Index,Indexs...>,Type,Types...>::tuple_impl(const mpl::sequence<IIndex,IIndexs...>&, Arg&& i_arg, Args&& ... i_args)
{
    construct<typename mpl::nth_type_of<IIndex,Type,Types...>::type>(PACK_STORAGE_ADDRESS(m_storage) + data_offset::at(Index), std::forward<Arg>(i_arg)) && ( construct<typename mpl::nth_type_of<IIndexs,Type,Types...>::type>(PACK_STORAGE_ADDRESS(m_storage) + data_offset::at(Indexs), std::forward<Args>(i_args)) && ... );
}
template<size_t Index, size_t ... Indexs,typename Type, typename ... Types>
template<typename Arg, typename ... Args>
tuple_impl<mpl::sequence<Index,Indexs...>,Type,Types...>::tuple_impl(Arg&& i_arg, Args&& ... i_args)
{
    construct<Type>(PACK_STORAGE_ADDRESS(m_storage) + data_offset::at(Index), std::forward<Arg>(i_arg)) && ( construct<Types>(PACK_STORAGE_ADDRESS(m_storage) + data_offset::at(Indexs), std::forward<Args>(i_args)) && ... );
}
template<size_t Index, size_t ... Indexs,typename Type, typename ... Types>
tuple_impl<mpl::sequence<Index,Indexs...>,Type,Types...>::tuple_impl(const tuple_impl& other)
{
    construct<Type>(PACK_STORAGE_ADDRESS(m_storage) + data_offset::at(Index), std::forward<Type>(get<Type>(PACK_STORAGE_ADDRESS(other.m_storage) + data_offset::at(Index)))) && ( construct<Types>(PACK_STORAGE_ADDRESS(m_storage) + data_offset::at(Indexs), std::forward<Types>(get<Types>(PACK_STORAGE_ADDRESS(other.m_storage) + data_offset::at(Indexs)))) && ... );
}
template<size_t Index, size_t ... Indexs,typename Type, typename ... Types>
tuple_impl<mpl::sequence<Index,Indexs...>,Type,Types...>::tuple_impl(tuple_impl&& other)
{
    construct<Type>(PACK_STORAGE_ADDRESS(m_storage) + data_offset::at(Index), std::move(extract<Type>(PACK_STORAGE_ADDRESS(other.m_storage) + data_offset::at(Index)))) && ( construct<Types>(PACK_STORAGE_ADDRESS(m_storage) + data_offset::at(Indexs), std::move(extract<Types>(PACK_STORAGE_ADDRESS(other.m_storage) + data_offset::at(Indexs)))) && ... );
}
template<size_t Index, size_t ... Indexs,typename Type, typename ... Types>
template<size_t IIndex, size_t ... IIndexs, typename TType, typename ... TTypes>
tuple_impl<mpl::sequence<Index,Indexs...>,Type,Types...>::tuple_impl(const tuple_impl<mpl::sequence<IIndex,IIndexs...>,TType,TTypes...>& other)
{
    construct<Type>(PACK_STORAGE_ADDRESS(m_storage) + data_offset::at(Index), std::forward<Type>(get<Type>(PACK_STORAGE_ADDRESS(other.m_storage) + data_offset::at(Index)))) && ( construct<Types>(PACK_STORAGE_ADDRESS(m_storage) + data_offset::at(Indexs), std::forward<Types>(get<Types>(PACK_STORAGE_ADDRESS(other.m_storage) + data_offset::at(Indexs)))) && ... );
}
template<size_t Index, size_t ... Indexs,typename Type, typename ... Types>
template<size_t IIndex, size_t ... IIndexs, typename TType, typename ... TTypes>
tuple_impl<mpl::sequence<Index,Indexs...>,Type,Types...>::tuple_impl(tuple_impl<mpl::sequence<IIndex,IIndexs...>,TType,TTypes...>&& other)
{
    construct<Type>(PACK_STORAGE_ADDRESS(m_storage) + data_offset::at(Index), std::move(extract<Type>(PACK_STORAGE_ADDRESS(other.m_storage) + data_offset::at(Index)))) && ( construct<Types>(PACK_STORAGE_ADDRESS(m_storage) + data_offset::at(Indexs), std::move(extract<Types>(PACK_STORAGE_ADDRESS(other.m_storage) + data_offset::at(Indexs)))) && ... );
}
template<size_t Index, size_t ... Indexs,typename Type, typename ... Types>
tuple_impl<mpl::sequence<Index,Indexs...>,Type,Types...>::~tuple_impl()
{
    ( destruct<Type>(PACK_STORAGE_ADDRESS(m_storage) + data_offset::at(Index)) && ( destruct<Types>(PACK_STORAGE_ADDRESS(m_storage) + data_offset::at(Indexs)) && ... ) );
}
template<size_t Index, size_t ... Indexs,typename Type, typename ... Types>
tuple_impl<mpl::sequence<Index,Indexs...>,Type,Types...>& tuple_impl<mpl::sequence<Index,Indexs...>,Type,Types...>::operator=(const tuple_impl& other)
{
    assign<Type>(PACK_STORAGE_ADDRESS(m_storage) + data_offset::at(Index),std::forward<Type>(get<Type>(PACK_STORAGE_ADDRESS(other.m_storage) + data_offset::at(Index)))) && ( assign<Types>(PACK_STORAGE_ADDRESS(m_storage) + data_offset::at(Indexs), std::forward<Types>(get<Types>(PACK_STORAGE_ADDRESS(other.m_storage) + data_offset::at(Indexs)))) && ... );

    return *this;
}
template<size_t Index, size_t ... Indexs,typename Type, typename ... Types>
tuple_impl<mpl::sequence<Index,Indexs...>,Type,Types...>& tuple_impl<mpl::sequence<Index,Indexs...>,Type,Types...>::operator=(tuple_impl&& other)
{
    assign<Type>(PACK_STORAGE_ADDRESS(m_storage) + data_offset::at(Index),std::move(extract<Type>(PACK_STORAGE_ADDRESS(other.m_storage) + data_offset::at(Index)))) && ( assign<Types>(PACK_STORAGE_ADDRESS(m_storage) + data_offset::at(Indexs), std::move(extract<Types>(PACK_STORAGE_ADDRESS(other.m_storage) + data_offset::at(Indexs)))) && ... );

    return *this;
}
template<size_t Index, size_t ... Indexs,typename Type, typename ... Types>
template<size_t IIndex>
typename embedded_type<typename mpl::nth_type_of<IIndex,Type,Types...>::type>::cref_type tuple_impl<mpl::sequence<Index,Indexs...>,Type,Types...>::get() const
{
    typedef typename mpl::nth_type_of<IIndex,Type,Types...>::type nth_type;

    return get<nth_type>(PACK_STORAGE_ADDRESS(m_storage) + data_offset::at(IIndex));
}
template<size_t Index, size_t ... Indexs,typename Type, typename ... Types>
template<size_t IIndex>
typename embedded_type<typename mpl::nth_type_of<IIndex,Type,Types...>::type>::ref_type tuple_impl<mpl::sequence<Index,Indexs...>,Type,Types...>::get()
{
    typedef typename mpl::nth_type_of<IIndex,Type,Types...>::type nth_type;

    return get<nth_type>(PACK_STORAGE_ADDRESS(m_storage) + data_offset::at(IIndex));
}
template<size_t Index, size_t ... Indexs,typename Type, typename ... Types>
template<size_t ... IIndexs, typename ... Args>
void tuple_impl<mpl::sequence<Index,Indexs...>,Type,Types...>::set(const mpl::sequence<IIndexs...>&, Args&& ... i_args)
{
	static_assert(mpl::get_num_ranks<Indexs...>::value == mpl::get_num_types<Args...>::value, "Unconsistent provided arguments and sequence");
	static_assert(mpl::get_num_types<Type,Types...>::value == mpl::get_num_types<Args...>::value, "Wrong number of arguments");

    ( assign<typename mpl::nth_type_of<Indexs,Type,Types...>::type>(PACK_STORAGE_ADDRESS(m_storage) + data_offset::at(Indexs), std::forward<Args>(i_args)) && ... );
}
template<size_t Index, size_t ... Indexs,typename Type, typename ... Types>
template<size_t IIndex, typename Arg>
bool tuple_impl<mpl::sequence<Index,Indexs...>,Type,Types...>::set(Arg&& i_arg)
{
    typedef typename mpl::nth_type_of<Index,Type,Types...>::type nth_type;

    return assign<nth_type>(PACK_STORAGE_ADDRESS(m_storage) + data_offset::at(Index), std::forward<Arg>(i_arg));
}
template<size_t Index, size_t ... Indexs,typename Type, typename ... Types>
constexpr size_t tuple_impl<mpl::sequence<Index,Indexs...>,Type,Types...>::size()
{
	return mpl::get_num_types<Type,Types...>::value;
}

}
}
