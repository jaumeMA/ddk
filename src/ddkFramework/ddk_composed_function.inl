
namespace ddk
{
namespace detail
{

template<typename ReturnDst, typename ... TypesDst, typename ReturnSrc, typename ... TypesSrc>
composed_function<ReturnDst(TypesDst...),ReturnSrc(TypesSrc...)>::composed_function(const function<ReturnDst(TypesDst...)>& i_lhs, const function<ReturnSrc(TypesSrc...)>& i_rhs)
: m_lhs(i_lhs)
, m_rhs(i_rhs)
{
}
template<typename ReturnDst, typename ... TypesDst, typename ReturnSrc, typename ... TypesSrc>
ReturnDst composed_function<ReturnDst(TypesDst...),ReturnSrc(TypesSrc...)>::operator()(TypesSrc ... i_args) const
{
    return eval(m_lhs,eval(m_rhs,std::forward<TypesSrc>(i_args) ...));
}

}
}
