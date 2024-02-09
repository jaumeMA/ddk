
namespace ddk
{

constexpr iterable_exception::iterable_exception(const char* i_reason)
: m_reason(i_reason)
{
}

constexpr inline void stop_iteration(const char* i_msg)
{
	if (i_msg == nullptr)
	{
		return;
	}

	throw iterable_exception{ i_msg };
}

}