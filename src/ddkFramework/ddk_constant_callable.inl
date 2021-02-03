
namespace ddk
{

template<typename T>
constant_function<T>::constant_function(T i_value)
: m_value(i_value)
{
}
template<typename T>
template<typename ... Args>
T constant_function<T>::operator()(Args&& ... i_args) const
{
    return m_value;
}

}
