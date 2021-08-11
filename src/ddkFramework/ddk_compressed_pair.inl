
namespace ddk
{
namespace detail
{
namespace
{

template<bool IsFirst, typename First, typename Second>
auto resolve_compressed_pair_construction(First&& i_first, Second&& i_second)
{
	if constexpr(IsFirst)
	{
		return std::forward<First>(i_first);
	}
	else
	{
		return std::forward<Second>(i_second);
	}
}

}

template<typename First,typename Second>
TEMPLATE(typename FFirst,typename SSecond)
REQUIRED(IS_CONSTRUCTIBLE(First,FFirst),IS_CONSTRUCTIBLE(Second,SSecond))
compressed_pair<First,Second>::compressed_pair(FFirst&& i_first,SSecond&& i_second)
: base_type(resolve_compressed_pair_construction<isFirstBased>(std::forward<FFirst>(i_first),std::forward<SSecond>(i_second)))
, m_member(resolve_compressed_pair_construction<!isFirstBased>(std::forward<FFirst>(i_first),std::forward<SSecond>(i_second)))
{
}
template<typename First,typename Second>
template<typename FFirst>
void compressed_pair<First,Second>::set_first(FFirst&& i_first)
{
	if constexpr(isFirstBased)
	{
		static_cast<First&>(*this) = std::forward<FFirst>(i_first);
	}
	else
	{
		m_member = std::forward<FFirst>(i_first);
	}
}
template<typename First,typename Second>
template<typename SSecond>
void compressed_pair<First,Second>::set_second(SSecond&& i_second)
{
	if constexpr(isFirstBased)
	{
		m_member = std::forward<SSecond>(i_second);
	}
	else
	{
		static_cast<Second&>(*this) = std::forward<SSecond>(i_second);
	}
}
template<typename First,typename Second>
First& compressed_pair<First,Second>::get_first()
{
	if constexpr (isFirstBased)
	{
		return static_cast<First&>(*this);
	}
	else
	{
		return m_member;
	}
}
template<typename First,typename Second>
const First& compressed_pair<First,Second>::get_first() const
{
	if constexpr(isFirstBased)
	{
		return static_cast<const First&>(*this);
	}
	else
	{
		return m_member;
	}
}
template<typename First,typename Second>
Second& compressed_pair<First,Second>::get_second()
{
	if constexpr(isFirstBased)
	{
		return m_member;
	}
	else
	{
		return static_cast<Second&>(*this);
	}
}
template<typename First,typename Second>
const Second compressed_pair<First,Second>::get_second() const
{
	if constexpr(isFirstBased)
	{
		return m_member;
	}
	else
	{
		return static_cast<const Second&>(*this);
	}
}

}
}