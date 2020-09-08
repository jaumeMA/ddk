
#define PACK_STORAGE_ADDRESS(_STORAGE) reinterpret_cast<char*>(const_cast<decltype(_STORAGE)*>(&_STORAGE))

namespace ddk
{

template<typename Type, typename ... Types>
template<size_t Index, size_t ... Indexs, typename Arg, typename ... Args>
tuple<Type,Types...>::tuple(const mpl::sequence<Index,Indexs...>&, Arg&& i_arg, Args&& ... i_args)
{
    size_t thisTypeIndex = 0;

    construct<typename mpl::nth_type_of<Index,Type,Types...>::type>(PACK_STORAGE_ADDRESS(m_storage), std::forward<Arg>(i_arg)) && ( construct<typename mpl::nth_type_of<Indexs,Type,Types...>::type>(PACK_STORAGE_ADDRESS(m_storage) + data_offset::at(thisTypeIndex++), std::forward<Args>(i_args)) && ... );
}
template<typename Type, typename ... Types>
template<typename Arg, typename ... Args>
tuple<Type,Types...>::tuple(Arg&& i_arg, Args&& ... i_args)
{
    size_t thisTypeIndex = 1;

    construct<Type>(PACK_STORAGE_ADDRESS(m_storage), std::forward<Arg>(i_arg)) && ( construct<Types>(PACK_STORAGE_ADDRESS(m_storage) + data_offset::at(thisTypeIndex++), std::forward<Args>(i_args)) && ... );
}
template<typename Type, typename ... Types>
tuple<Type,Types...>::tuple(const tuple<Type,Types...>& other)
{
    size_t thisTypeIndex = 1;
    size_t otherTypeIndex = 1;

    construct<Type>(PACK_STORAGE_ADDRESS(m_storage), std::forward<Type>(get<Type>(PACK_STORAGE_ADDRESS(other.m_storage) ))) && ( construct<Types>(PACK_STORAGE_ADDRESS(m_storage) + data_offset::at(thisTypeIndex++), std::forward<Types>(get<Types>(PACK_STORAGE_ADDRESS(other.m_storage) + data_offset::at(otherTypeIndex++)))) && ... );
}
template<typename Type, typename ... Types>
tuple<Type,Types...>::tuple(tuple<Type,Types...>&& other)
{
    size_t thisTypeIndex = 1;
    size_t otherTypeIndex = 1;

    construct<Type>(PACK_STORAGE_ADDRESS(m_storage), std::move(extract<Type>(PACK_STORAGE_ADDRESS(other.m_storage) ))) && ( construct<Types>(PACK_STORAGE_ADDRESS(m_storage) + data_offset::at(thisTypeIndex++), std::move(extract<Types>(PACK_STORAGE_ADDRESS(other.m_storage) + data_offset::at(otherTypeIndex++)))) && ... );
}
template<typename Type, typename ... Types>
template<typename Arg, typename ... Args>
tuple<Type,Types...>::tuple(const tuple<Arg,Args...>& other)
{
    size_t thisTypeIndex = 1;
    size_t otherTypeIndex = 1;

    construct<Type>(PACK_STORAGE_ADDRESS(m_storage),std::forward<Arg>(get<Arg>(PACK_STORAGE_ADDRESS(other.m_storage)))) && ( construct<Types>(PACK_STORAGE_ADDRESS(m_storage) + data_offset::at(thisTypeIndex++), std::forward<Args>(get<Args>(PACK_STORAGE_ADDRESS(other.m_storage) + data_offset::at(otherTypeIndex++)))) && ... );
}
template<typename Type, typename ... Types>
template<typename Arg, typename ... Args>
tuple<Type,Types...>::tuple(tuple<Arg,Args...>&& other)
{
    size_t thisTypeIndex = 1;
    size_t otherTypeIndex = 1;

    construct<Type>(PACK_STORAGE_ADDRESS(m_storage),std::move(extract<Arg>(PACK_STORAGE_ADDRESS(other.m_storage)))) && ( construct<Types>(PACK_STORAGE_ADDRESS(m_storage) + data_offset::at(thisTypeIndex++), std::move(extract<Args>(PACK_STORAGE_ADDRESS(other.m_storage) + data_offset::at(otherTypeIndex++)))) && ... );
}
template<typename Type, typename ... Types>
tuple<Type,Types...>::~tuple()
{
    size_t thisTypeIndex = 1;

    ( destruct<Type>(PACK_STORAGE_ADDRESS(m_storage)) && ( destruct<Types>(PACK_STORAGE_ADDRESS(m_storage) + data_offset::at(thisTypeIndex++)) && ... ) );
}
template<typename Type, typename ... Types>
tuple<Type,Types...>& tuple<Type,Types...>::operator=(const tuple<Type,Types...>& other)
{
    size_t thisTypeIndex = 1;
    size_t otherTypeIndex = 1;

    assign<Type>(PACK_STORAGE_ADDRESS(m_storage),std::forward<Type>(get<Type>(PACK_STORAGE_ADDRESS(other.m_storage)))) && ( assign<Types>(PACK_STORAGE_ADDRESS(m_storage) + data_offset::at(thisTypeIndex++), std::forward<Types>(get<Types>(PACK_STORAGE_ADDRESS(other.m_storage) + data_offset::at(otherTypeIndex++)))) && ... );

    return *this;
}
template<typename Type, typename ... Types>
tuple<Type,Types...>& tuple<Type,Types...>::operator=(tuple<Type,Types...>&& other)
{
    size_t thisTypeIndex = 1;
    size_t otherTypeIndex = 1;

    assign<Type>(PACK_STORAGE_ADDRESS(m_storage),std::move(extract<Type>(PACK_STORAGE_ADDRESS(other.m_storage)))) && ( assign<Types>(PACK_STORAGE_ADDRESS(m_storage) + data_offset::at(thisTypeIndex++), std::move(extract<Types>(PACK_STORAGE_ADDRESS(other.m_storage) + data_offset::at(otherTypeIndex++)))) && ... );

    return *this;
}
template<typename Type, typename ... Types>
template<int Index>
typename embedded_type<typename mpl::nth_type_of<Index,Type,Types...>::type>::cref_type tuple<Type,Types...>::get() const
{
    typedef typename mpl::nth_type_of<Index,Type,Types...>::type nth_type;

    return get<nth_type>(PACK_STORAGE_ADDRESS(m_storage) + data_offset::at(Index));
}
template<typename Type, typename ... Types>
template<int Index>
typename embedded_type<typename mpl::nth_type_of<Index,Type,Types...>::type>::ref_type tuple<Type,Types...>::get()
{
    typedef typename mpl::nth_type_of<Index,Type,Types...>::type nth_type;

    return get<nth_type>(PACK_STORAGE_ADDRESS(m_storage) + data_offset::at(Index));
}
template<typename Type, typename ... Types>
template<int ... Indexs>
void tuple<Type,Types...>::set(typename mpl::nth_type_of<Indexs,Type,Types...>::type ... args)
{
    ( assign<typename mpl::nth_type_of<Indexs,Type,Types...>::type>(PACK_STORAGE_ADDRESS(m_storage) + data_offset::at(Indexs), std::forward<typename mpl::nth_type_of<Indexs,Type,Types...>::type>(args)) && ... );
}
template<typename Type, typename ... Types>
template<int Index>
bool tuple<Type,Types...>::set(typename mpl::nth_type_of<Index,Type,Types...>::type val)
{
    typedef typename mpl::nth_type_of<Index,Type,Types...>::type nth_type;

    return assign<nth_type>(PACK_STORAGE_ADDRESS(m_storage) + data_offset::at(Index), std::forward<nth_type>(val));
}

}
