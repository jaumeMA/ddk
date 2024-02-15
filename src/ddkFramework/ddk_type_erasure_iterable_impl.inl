
namespace ddk
{
namespace detail
{

template<typename Traits>
TEMPLATE(typename Iterable)
REQUIRED(ARE_TRAITS_COMPATIBLE(typename Iterable::traits,Traits))
type_erasure_iterable_impl<Traits>::type_erasure_iterable_impl(const iterable<Iterable>& i_iterable)
: m_iterable(make_iterable<Traits>(i_iterable.get()))
{
}
template<typename Traits>
TEMPLATE(typename Iterable)
REQUIRED(ARE_TRAITS_COMPATIBLE(typename Iterable::traits,Traits))
type_erasure_iterable_impl<Traits>::type_erasure_iterable_impl(iterable<Iterable>&& i_iterable)
: m_iterable(make_iterable<Traits>(std::move(i_iterable).extract()))
{
}
template<typename Traits>
TEMPLATE(typename Iterable,typename Allocator)
REQUIRED(ARE_TRAITS_COMPATIBLE(typename Iterable::traits,Traits),IS_ALLOCATOR(Allocator))
type_erasure_iterable_impl<Traits>::type_erasure_iterable_impl(const iterable<Iterable>& i_iterable,const Allocator& i_allocator)
: m_iterable(make_iterable<Traits>(i_allocator,i_iterable.get(),i_allocator))
{
}
template<typename Traits>
TEMPLATE(typename Iterable,typename Allocator)
REQUIRED(ARE_TRAITS_COMPATIBLE(typename Iterable::traits,Traits),IS_ALLOCATOR(Allocator))
type_erasure_iterable_impl<Traits>::type_erasure_iterable_impl(iterable<Iterable>&& i_iterable, const Allocator& i_allocator)
: m_iterable(make_iterable<Traits>(std::move(i_iterable).extract(),i_allocator))
{
}
template<typename Traits>
type_erasure_iterable_impl<Traits>::type_erasure_iterable_impl(const iterable<type_erasure_iterable_impl<Traits>>& i_iterable)
: m_iterable(i_iterable.get().m_iterable)
{
}
template<typename Traits>
type_erasure_iterable_impl<Traits>::type_erasure_iterable_impl(iterable<type_erasure_iterable_impl<Traits>>&& i_iterable)
: m_iterable(std::move(i_iterable).extract().m_iterable)
{
}
template<typename Traits>
TEMPLATE(typename TTraits)
REQUIRED(ARE_TRAITS_COMPATIBLE(TTraits,Traits))
type_erasure_iterable_impl<Traits>::type_erasure_iterable_impl(const iterable<type_erasure_iterable_impl<TTraits>>& i_iterable)
: m_iterable(make_iterable<Traits>(i_iterable.get()))
{
}
template<typename Traits>
TEMPLATE(typename TTraits)
REQUIRED(ARE_TRAITS_COMPATIBLE(TTraits,Traits))
type_erasure_iterable_impl<Traits>::type_erasure_iterable_impl(iterable<type_erasure_iterable_impl<TTraits>>&& i_iterable)
: m_iterable(make_iterable<Traits>(std::move(i_iterable).extract()))
{
}
template<typename Traits>
TEMPLATE(typename TTraits, typename Allocator)
REQUIRED(ARE_TRAITS_COMPATIBLE(TTraits,Traits))
type_erasure_iterable_impl<Traits>::type_erasure_iterable_impl(const iterable<type_erasure_iterable_impl<TTraits>>& i_iterable, const Allocator& i_allocator)
: m_iterable(make_iterable<Traits>(i_iterable.get(),i_allocator))
{
}
template<typename Traits>
TEMPLATE(typename TTraits,typename Allocator)
REQUIRED(ARE_TRAITS_COMPATIBLE(TTraits,Traits))
type_erasure_iterable_impl<Traits>::type_erasure_iterable_impl(iterable<type_erasure_iterable_impl<TTraits>>&& i_iterable,const Allocator& i_allocator)
: m_iterable(make_iterable<Traits>(std::move(i_iterable).extract(),i_allocator))
{
}
template<typename Traits>
template<typename Action>
iterable_result type_erasure_iterable_impl<Traits>::iterate_impl(const Action& i_initialAction)
{
	return m_iterable->iterate(i_initialAction);
}
template<typename Traits>
template<typename Action>
iterable_result type_erasure_iterable_impl<Traits>::iterate_impl(const Action& i_initialAction) const
{
	return m_iterable->iterate(i_initialAction);
}
template<typename Traits>
distributed_reference_wrapper<iterable_base<Traits>> type_erasure_iterable_impl<Traits>::get_iterable() const
{
	return m_iterable;
}

}
}