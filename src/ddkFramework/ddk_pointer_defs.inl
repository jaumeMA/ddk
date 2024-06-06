
namespace ddk
{

#ifdef DDK_DEBUG

template<typename T>
T* get_raw_ptr(lent_pointer_wrapper<T> i_ref)
{
	return i_ref.m_data;
}
template<typename T>
void set_raw_ptr(lent_pointer_wrapper<T>& i_ref, T* i_value)
{
	i_ref.m_data = i_value;
}
template<typename T>
T* extract_raw_ptr(lent_pointer_wrapper<T>& i_ref)
{
	T* res = i_ref.m_data;

	i_ref = nullptr;

	return res;
}
template<typename T>
inline void clear_ptr(lent_pointer_wrapper<T>& i_ref)
{
	i_ref = nullptr;
}
template<typename T>
void rebase_ptr(lent_pointer_wrapper<T>& i_ref,std::ptrdiff_t i_diff)
{
	i_ref.m_data = reinterpret_cast<T*>(reinterpret_cast<char*>(i_ref.m_data) + i_diff);
	i_ref.m_refCounter.set_pointer(reinterpret_cast<tagged_reference_counter::interface_ptr>(reinterpret_cast<char*>(i_ref.m_refCounter.get_pointer()) + i_diff));
}

#else

template<typename T>
T* get_raw_ptr(T* i_ref)
{
	return i_ref;
}
template<typename T>
void set_raw_ptr(T*& i_ref,T* i_value)
{
	i_ref = i_value;
}
template<typename T>
T* extract_raw_ptr(T*& i_ref)
{
	T* res = i_ref;

	i_ref = nullptr;

	return res;
}
template<typename T>
void clear_ptr(T*& i_ref)
{
	i_ref = nullptr;
}
template<typename T>
void rebase_ptr(T*& i_ref,std::ptrdiff_t i_diff)
{
	i_ref = reinterpret_cast<T*>(reinterpret_cast<char*>(i_ref) + i_diff);
}

#endif

}
