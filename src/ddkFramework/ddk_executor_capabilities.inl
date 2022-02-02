
namespace ddk
{
namespace detail
{

template<typename Context>
executor_capabilities<Context>::executor_capabilities(Context i_context)
: m_context(std::move(i_context))
{
}

}
}