
namespace ddk
{

template<typename Return>
attachable<Return>::attachable(cancellable_executor_unique_ref<Return> i_executorImpl)
: m_executorImpl(std::move(i_executorImpl))
{
}

}
