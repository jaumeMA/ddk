
#include <memory>

namespace ddk
{
namespace detail
{

template<typename ... Types>
const std::array<size_t,mpl::num_types<Types...>> aligned_tuple_storage<Types...>::m_offset = resolve_type_offset(s_total_size);

template<typename ... Types>
std::array<size_t,mpl::num_types<Types...>> aligned_tuple_storage<Types...>::resolve_type_offset(size_t i_totalSize)
{
	typedef typename mpl::acc_sizeof<Types...>::type data_align;
	static const size_t s_numTypes = mpl::get_num_types<Types...>();

	std::array<size_t,mpl::get_num_types<Types...>()> res;
	void* arena = nullptr;
	size_t spaceLeft = i_totalSize;
	static const size_t typeSizes[] = { mpl::size_of_qualified_type<Types>::value ...};
	static const size_t typeAlignments[] = { alignof(Types) ... };

	for(size_t typeIndex=0;typeIndex<s_numTypes;++typeIndex)
	{
		res[typeIndex] = reinterpret_cast<size_t>(std::align(typeAlignments[typeIndex],typeSizes[typeIndex],arena,spaceLeft));
		arena = reinterpret_cast<char*>(arena) + typeSizes[typeIndex];
		spaceLeft -= typeSizes[typeIndex];
	}

	return res;
}

template<typename ... Types>
template<size_t Index>
void* aligned_tuple_storage<Types...>::at()
{
	typedef typename mpl::nth_type_of<Index,Types...>::type curr_type;

	return m_arena.get_arena() + m_offset.at(Index);
}
template<typename ... Types>
template<size_t Index>
const void* aligned_tuple_storage<Types...>::at() const
{
	return m_arena.get_arena() + m_offset.at(Index);
}


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
	return std::forward<typename embedded_type<Type>::cref_type>(m_val);
}
template<typename Type>
template<size_t IIndex>
typename embedded_type<Type>::ref_type tuple_impl<mpl::sequence<0>,Type>::get()
{
	return std::forward<typename embedded_type<Type>::ref_type>(m_val);
}
template<typename Type>
template<size_t IIndex>
typename embedded_type<Type>::rref_type tuple_impl<mpl::sequence<0>,Type>::extract() &&
{
	return std::forward<typename embedded_type<Type>::rref_type>(m_val);
}
template<typename Type>
template<size_t Index, typename Arg>
typename embedded_type<Type>::ref_type tuple_impl<mpl::sequence<0>,Type>::set(Arg&& i_arg)
{
	return (m_val = std::forward<Arg>(i_arg));
}
template<typename Type>
tuple_impl<mpl::sequence<0>,Type>* tuple_impl<mpl::sequence<0>,Type>::operator->()
{
    return this;
}
template<typename Type>
const tuple_impl<mpl::sequence<0>,Type>* tuple_impl<mpl::sequence<0>,Type>::operator->() const
{
    return this;
}
template<typename Type>
constexpr size_t tuple_impl<mpl::sequence<0>,Type>::size()
{
	return 1;
}
template<typename Type>
bool tuple_impl<mpl::sequence<0>,Type>::empty() const
{
	return false;
}

template<size_t Index1, size_t Index2, size_t ... Indexs, typename Type1, typename Type2, typename ... Types>
tuple_impl<mpl::sequence<Index1,Index2,Indexs...>,Type1,Type2,Types...>::tuple_impl()
{
	const bool _[] = { construct<Type1>(m_storage.template at<Index1>(), Type1{}),
                        construct<Type2>(m_storage.template at<Index2>(), Type2{}),
                        construct<Types>(m_storage.template at<Indexs>(), Types{}) ... };
}
template<size_t Index1, size_t Index2, size_t ... Indexs, typename Type1, typename Type2, typename ... Types>
template<size_t IIndex1, size_t IIndex2, size_t ... IIndexs, typename Arg1, typename Arg2, typename ... Args>
tuple_impl<mpl::sequence<Index1,Index2,Indexs...>,Type1,Type2,Types...>::tuple_impl(const mpl::sequence<IIndex1,IIndex2,IIndexs...>&, Arg1&& i_arg1, Arg2&& i_arg2, Args&& ... i_args)
{
	const bool _[] = { construct<typename mpl::nth_type_of<IIndex1,Type1,Type2,Types...>::type>(m_storage.template at<IIndex1>(), std::forward<Arg1>(i_arg1)),
                        construct<typename mpl::nth_type_of<IIndex2,Type1,Type2,Types...>::type>(m_storage.template at<IIndex2>(), std::forward<Arg2>(i_arg2)),
                        construct<typename mpl::nth_type_of<IIndexs,Type1,Type2,Types...>::type>(m_storage.template at<IIndexs>(), std::forward<Args>(i_args)) ... };
}
template<size_t Index1, size_t Index2, size_t ... Indexs, typename Type1, typename Type2, typename ... Types>
template<typename Arg1, typename Arg2, typename ... Args>
tuple_impl<mpl::sequence<Index1,Index2,Indexs...>,Type1,Type2,Types...>::tuple_impl(Arg1&& i_arg1, Arg2&& i_arg2, Args&& ... i_args)
{
    const bool _[] = { construct<Type1>(m_storage.template at<Index1>(), std::forward<Arg1>(i_arg1)),
                        construct<Type2>(m_storage.template at<Index2>(), std::forward<Arg2>(i_arg2)),
                        construct<Types>(m_storage.template at<Indexs>(), std::forward<Args>(i_args)) ... };
}
template<size_t Index1, size_t Index2, size_t ... Indexs, typename Type1, typename Type2, typename ... Types>
tuple_impl<mpl::sequence<Index1,Index2,Indexs...>,Type1,Type2,Types...>::tuple_impl(const tuple_impl& other)
{
	const bool _[] = { construct<Type1>(m_storage.template at<Index1>(), std::forward<Type1>(get<Type1>(other.m_storage.template at<Index1>()))),
                        construct<Type2>(m_storage.template at<Index2>(), std::forward<Type2>(get<Type2>(other.m_storage.template at<Index2>()))),
                        construct<Types>(m_storage.template at<Indexs>(), std::forward<Types>(get<Types>(other.m_storage.template at<Indexs>()))) ... };
}
template<size_t Index1, size_t Index2, size_t ... Indexs, typename Type1, typename Type2, typename ... Types>
tuple_impl<mpl::sequence<Index1,Index2,Indexs...>,Type1,Type2,Types...>::tuple_impl(tuple_impl&& other)
{
    const bool _[] = 	{ construct<Type1>(m_storage.template at<Index1>(), std::forward<typename embedded_type<Type1>::rref_type>(extract<Type1>(other.m_storage.template at<Index1>()))),
                        construct<Type2>(m_storage.template at<Index2>(), std::forward<typename embedded_type<Type2>::rref_type>(extract<Type2>(other.m_storage.template at<Index2>()))),
                        construct<Types>(m_storage.template at<Indexs>(), std::forward<typename embedded_type<Types>::rref_type>(extract<Types>(other.m_storage.template at<Indexs>()))) ... };
}
template<size_t Index1, size_t Index2, size_t ... Indexs, typename Type1, typename Type2, typename ... Types>
template<size_t IIndex1, size_t IIndex2, size_t ... IIndexs, typename TType1, typename TType2, typename ... TTypes>
tuple_impl<mpl::sequence<Index1,Index2,Indexs...>,Type1,Type2,Types...>::tuple_impl(const tuple_impl<mpl::sequence<IIndex1,IIndex2,IIndexs...>,TType1,TType2,TTypes...>& other)
{
	const bool _[] = { construct<Type1>(m_storage.template at<Index1>(), std::forward<TType1>(get<TType1>(other.m_storage.template at<IIndex1>()))),
                        construct<Type2>(m_storage.template at<Index2>(), std::forward<TType2>(get<TType2>(other.m_storage.template at<IIndex2>()))),
                        construct<Types>(m_storage.template at<Indexs>(), std::forward<TTypes>(get<TTypes>(other.m_storage.template at<IIndexs>()))) ... };
}
template<size_t Index1, size_t Index2, size_t ... Indexs, typename Type1, typename Type2, typename ... Types>
template<size_t IIndex1, size_t IIndex2, size_t ... IIndexs, typename TType1, typename TType2, typename ... TTypes>
tuple_impl<mpl::sequence<Index1,Index2,Indexs...>,Type1,Type2,Types...>::tuple_impl(tuple_impl<mpl::sequence<IIndex1,IIndex2,IIndexs...>,TType1,TType2,TTypes...>&& other)
{
	const bool _[] = { construct<Type1>(m_storage.template at<Index1>(), std::move(extract<TType1>(other.m_storage.template at<IIndex1>()))),
                        construct<Type2>(m_storage.template at<Index2>(), std::move(extract<TType2>(other.m_storage.template at<IIndex2>()))),
                        construct<Types>(m_storage.template at<Indexs>(), std::move(extract<TTypes>(other.m_storage.template at<IIndexs>()))) ... };
}
template<size_t Index1, size_t Index2, size_t ... Indexs, typename Type1, typename Type2, typename ... Types>
tuple_impl<mpl::sequence<Index1,Index2,Indexs...>,Type1,Type2,Types...>::~tuple_impl()
{
	const bool _[] = { destruct<Type1>(m_storage.template at<Index1>()),
                        destruct<Type2>(m_storage.template at<Index2>()),
                        destruct<Types>(m_storage.template at<Indexs>()) ... };
}
template<size_t Index1, size_t Index2, size_t ... Indexs, typename Type1, typename Type2, typename ... Types>
tuple_impl<mpl::sequence<Index1,Index2,Indexs...>,Type1,Type2,Types...>& tuple_impl<mpl::sequence<Index1,Index2,Indexs...>,Type1,Type2,Types...>::operator=(const tuple_impl& other)
{
	const bool _[] = { assign<Type1>(m_storage.template at<Index1>(),std::forward<Type1>(get<Type1>(other.m_storage.template at<Index1>()))),
                        assign<Type2>(m_storage.template at<Index2>(),std::forward<Type2>(get<Type2>(other.m_storage.template at<Index2>()))),
                        assign<Types>(m_storage.template at<Indexs>(), std::forward<Types>(get<Types>(other.m_storage.template at<Indexs>()))) ... };

    return *this;
}
template<size_t Index1, size_t Index2, size_t ... Indexs, typename Type1, typename Type2, typename ... Types>
tuple_impl<mpl::sequence<Index1,Index2,Indexs...>,Type1,Type2,Types...>& tuple_impl<mpl::sequence<Index1,Index2,Indexs...>,Type1,Type2,Types...>::operator=(tuple_impl&& other)
{
	const bool _[] = { assign<Type1>(m_storage.template at<Index1>(),std::move(extract<Type1>(other.m_storage.template at<Index1>()))),
                        assign<Type2>(m_storage.template at<Index2>(),std::move(extract<Type2>(other.m_storage.template at<Index2>()))),
                        assign<Types>(m_storage.template at<Indexs>(), std::move(extract<Types>(other.m_storage.template at<Indexs>()))) ... };

    return *this;
}
template<size_t Index1, size_t Index2, size_t ... Indexs, typename Type1, typename Type2, typename ... Types>
template<size_t IIndex>
typename embedded_type<typename mpl::nth_type_of<IIndex,Type1,Type2,Types...>::type>::cref_type tuple_impl<mpl::sequence<Index1,Index2,Indexs...>,Type1,Type2,Types...>::get() const
{
    typedef typename mpl::nth_type_of<IIndex,Type1,Type2,Types...>::type nth_type;

	return get<nth_type>(m_storage.template at<IIndex>());
}
template<size_t Index1, size_t Index2, size_t ... Indexs, typename Type1, typename Type2, typename ... Types>
template<size_t IIndex>
typename embedded_type<typename mpl::nth_type_of<IIndex,Type1,Type2,Types...>::type>::ref_type tuple_impl<mpl::sequence<Index1,Index2,Indexs...>,Type1,Type2,Types...>::get()
{
    typedef typename mpl::nth_type_of<IIndex,Type1,Type2,Types...>::type nth_type;

	return get<nth_type>(m_storage.template at<IIndex>());
}
template<size_t Index1,size_t Index2,size_t ... Indexs,typename Type1,typename Type2,typename ... Types>
template<size_t IIndex>
typename embedded_type<typename mpl::nth_type_of<IIndex,Type1,Type2,Types...>::type>::rref_type tuple_impl<mpl::sequence<Index1,Index2,Indexs...>,Type1,Type2,Types...>::extract() &&
{
	typedef typename mpl::nth_type_of<IIndex,Type1,Type2,Types...>::type nth_type;

	return extract<nth_type>(m_storage.template at<IIndex>());
}
template<size_t Index1, size_t Index2, size_t ... Indexs, typename Type1, typename Type2, typename ... Types>
template<size_t ... IIndexs, typename ... Args>
void tuple_impl<mpl::sequence<Index1,Index2,Indexs...>,Type1,Type2,Types...>::set(const mpl::sequence<IIndexs...>&, Args&& ... i_args)
{
	static_assert(mpl::get_num_ranks<IIndexs...>() == mpl::get_num_types<Args...>(), "Unconsistent provided arguments and sequence");
	static_assert(mpl::get_num_types<Type1,Type2,Types...>() == mpl::get_num_types<Args...>(), "Wrong number of arguments");

	const bool _[] = { assign<typename mpl::nth_type_of<Indexs,Type1,Type2,Types...>::type>(m_storage.template at<IIndexs>(), std::forward<Args>(i_args)) ... };
}
template<size_t Index1, size_t Index2, size_t ... Indexs, typename Type1, typename Type2, typename ... Types>
template<size_t IIndex, typename Arg>
typename embedded_type<typename mpl::nth_type_of<IIndex,Type1,Type2,Types...>::type>::ref_type tuple_impl<mpl::sequence<Index1,Index2,Indexs...>,Type1,Type2,Types...>::set(Arg&& i_arg)
{
    typedef typename mpl::nth_type_of<IIndex,Type1,Type2,Types...>::type nth_type;

	assign<nth_type>(m_storage.template at<IIndex>(), std::forward<Arg>(i_arg));

    return get<nth_type>(m_storage.template at<IIndex>());
}
template<size_t Index1, size_t Index2, size_t ... Indexs, typename Type1, typename Type2, typename ... Types>
tuple_impl<mpl::sequence<Index1,Index2,Indexs...>,Type1,Type2,Types...>* tuple_impl<mpl::sequence<Index1,Index2,Indexs...>,Type1,Type2,Types...>::operator->()
{
    return this;
}
template<size_t Index1, size_t Index2, size_t ... Indexs, typename Type1, typename Type2, typename ... Types>
const tuple_impl<mpl::sequence<Index1,Index2,Indexs...>,Type1,Type2,Types...>* tuple_impl<mpl::sequence<Index1,Index2,Indexs...>,Type1,Type2,Types...>::operator->() const
{
    return this;
}
template<size_t Index1, size_t Index2, size_t ... Indexs, typename Type1, typename Type2, typename ... Types>
constexpr size_t tuple_impl<mpl::sequence<Index1,Index2,Indexs...>,Type1,Type2,Types...>::size()
{
	return mpl::get_num_types<Type1,Type2,Types...>();
}
template<size_t Index1,size_t Index2,size_t ... Indexs,typename Type1,typename Type2,typename ... Types>
bool tuple_impl<mpl::sequence<Index1,Index2,Indexs...>,Type1,Type2,Types...>::empty() const
{
	return false;
}

}

template<typename ... Types>
tuple<Types...> make_tuple(Types&& ... vals)
{
    return tuple<Types...>(std::forward<Types>(vals)...);
}

template<typename ... FinalTypes, size_t ... FromIndexs, size_t ... ToIndexs, size_t ... IndexsA, typename ... TypesA, size_t ... IndexsB, typename ... TypesB>
tuple<typename mpl::nth_type_of<ToIndexs,FinalTypes...>::type ...> merge(const mpl::sequence<FromIndexs...>& i_srcSeq, const mpl::sequence<ToIndexs...>&, const mpl::sequence<IndexsA...>&, tuple<TypesA...>& i_lhs, const mpl::sequence<IndexsB...>&, tuple<TypesB...>& i_rhs)
{
    return tuple<typename mpl::nth_type_of<ToIndexs,FinalTypes...>::type ...>(i_srcSeq,i_lhs.template get<IndexsA>() ..., i_rhs.template get<IndexsB>() ...);
}
template<typename ... FinalTypes, size_t ... FromIndexs, size_t ... ToIndexs, size_t ... Indexs, typename ... Types, typename ... Args>
tuple<typename mpl::nth_type_of<ToIndexs,FinalTypes...>::type ...> merge_args(const mpl::sequence<FromIndexs...>& i_srcSeq, const mpl::sequence<ToIndexs...>&, const mpl::sequence<Indexs...>&, tuple<Types...>& i_lhs, Args&& ... i_args)
{
    return tuple<typename mpl::nth_type_of<ToIndexs,FinalTypes...>::type ...>(i_srcSeq,i_lhs.template get<Indexs>() ..., std::forward<Args>(i_args) ...);
}
template<typename ... TypesA, typename ... TypesB>
tuple<TypesA...,TypesB...> merge(const tuple<TypesA...>& i_lhs, const tuple<TypesB...>& i_rhs)
{
    typedef typename mpl::make_sequence<0,mpl::get_num_types<TypesA...>()>::type sequenceA;
    typedef typename mpl::make_sequence<0,mpl::get_num_types<TypesB...>()>::type sequenceB;
    typedef typename mpl::make_sequence<0,mpl::get_num_types<TypesA...>() + mpl::get_num_types<TypesB...>()>::type total_sequence;

    return merge<TypesA...,TypesB...>(total_sequence{},total_sequence{},sequenceA{},i_lhs,sequenceB{},i_rhs);
}
template<typename ... FinalTypes, size_t ... FromIndexs, size_t ... ToIndexs, typename ... TypesA, typename ... TypesB>
tuple<typename mpl::nth_type_of<ToIndexs,FinalTypes...>::type ...> merge(const mpl::sequence<FromIndexs...>& i_srcSeq, const mpl::sequence<ToIndexs...>& i_destSeq, const tuple<TypesA...>& i_lhs, const tuple<TypesB...>& i_rhs)
{
    typedef typename mpl::make_sequence<0,mpl::get_num_types<TypesA...>()>::type sequenceA;
    typedef typename mpl::make_sequence<0,mpl::get_num_types<TypesB...>()>::type sequenceB;

    return merge<FinalTypes...>(i_srcSeq,i_destSeq,sequenceA{},const_cast<tuple<TypesA...>&>(i_lhs),sequenceB{},const_cast<tuple<TypesB...>&>(i_rhs));
}
template<typename ... Types, typename ... Args>
tuple<Types...,Args...> merge_args(const tuple<Types...>& i_lhs, Args&& ... i_args)
{
    typedef typename mpl::make_sequence<0,mpl::get_num_types<Types...>() + mpl::get_num_types<Args...>()>::type total_sequence;
    typedef typename mpl::make_sequence<0,mpl::get_num_types<Types...>()>::type sequence_t;

    return merge_args<Types...,Args...>(total_sequence{},total_sequence{},sequence_t{},const_cast<tuple<Types...>&>(i_lhs),std::forward<Args>(i_args) ...);
}
template<typename ... FinalTypes, size_t ... FromIndexs, size_t ... ToIndexs, typename ... Types, typename ... Args>
tuple<typename mpl::nth_type_of<ToIndexs,FinalTypes...>::type ...> merge_args(const mpl::sequence<FromIndexs...>& i_srcSeq, const mpl::sequence<ToIndexs...>& i_destSeq, const tuple<Types...>& i_lhs, Args&& ... i_args)
{
    typedef typename mpl::make_sequence<0,mpl::get_num_types<Types...>()>::type sequence_t;

    return merge_args<FinalTypes...>(i_srcSeq,i_destSeq,sequence_t{},const_cast<tuple<Types...>&>(i_lhs),std::forward<Args>(i_args) ...);
}

}
