
namespace ddk
{
namespace detail
{

template<typename Traits>
TEMPLATE(typename Iterable)
REQUIRED(IS_BASE_OF(iterable_impl_interface<Traits>,Iterable))
type_erasure_iterable_impl<Traits>::type_erasure_iterable_impl(const iterable<Iterable>& i_iterable)
: m_iterable(make_distributed_reference<iterable<Iterable>>(i_iterable.get()))
{
}
template<typename Traits>
TEMPLATE(typename Iterable)
REQUIRED(IS_BASE_OF(iterable_impl_interface<Traits>,Iterable))
type_erasure_iterable_impl<Traits>::type_erasure_iterable_impl(iterable<Iterable>&& i_iterable)
: m_iterable(make_distributed_reference<iterable<Iterable>>(std::move(i_iterable).extract()))
{
}
template<typename Traits>
TEMPLATE(typename Iterable,typename Allocator)
REQUIRED(IS_BASE_OF(iterable_impl_interface<Traits>,Iterable),IS_ALLOCATOR(Allocator))
type_erasure_iterable_impl<Traits>::type_erasure_iterable_impl(const iterable<Iterable>& i_iterable,const Allocator& i_allocator)
: m_iterable(make_distributed_reference<iterable<Iterable>>(i_allocator,i_iterable.get()))
{
}
template<typename Traits>
TEMPLATE(typename Iterable,typename Allocator)
REQUIRED(IS_BASE_OF(iterable_impl_interface<Traits>,Iterable),IS_ALLOCATOR(Allocator))
type_erasure_iterable_impl<Traits>::type_erasure_iterable_impl(iterable<Iterable>&& i_iterable, const Allocator& i_allocator)
: m_iterable(make_distributed_reference<iterable<Iterable>>(i_allocator,std::move(i_iterable).extract()))
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
: m_iterable(make_distributed_reference<iterable<transformed_iterable_impl<Traits,TTraits,iterable<type_erasure_iterable_impl<TTraits>>,traits_conversion_callable<TTraits,Traits>>>>(iterable(i_iterable.get()),traits_conversion_callable<TTraits,Traits>{}))
{
}
template<typename Traits>
TEMPLATE(typename TTraits)
REQUIRED(ARE_TRAITS_COMPATIBLE(TTraits,Traits))
type_erasure_iterable_impl<Traits>::type_erasure_iterable_impl(iterable<type_erasure_iterable_impl<TTraits>>&& i_iterable)
: m_iterable(make_distributed_reference<iterable<transformed_iterable_impl<Traits,TTraits,iterable<type_erasure_iterable_impl<TTraits>>,traits_conversion_callable<TTraits,Traits>>>>(iterable(std::move(i_iterable).extract()),traits_conversion_callable<TTraits,Traits>{}))
{
}
template<typename Traits>
template<typename Action>
void type_erasure_iterable_impl<Traits>::iterate_impl(terse_endpoint i_try,const Action& i_initialAction)
{
	m_iterable->iterate(std::move(i_try),i_initialAction);
}
template<typename Traits>
template<typename Action>
void type_erasure_iterable_impl<Traits>::iterate_impl(const_terse_endpoint i_try,const Action& i_initialAction) const
{
	m_iterable->iterate(std::move(i_try),i_initialAction);
}

}
}