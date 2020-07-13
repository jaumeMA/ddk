#pragma once

#include <type_traits>
#include "ddk_template_helper.h"
#include "ddk_scoped_enum_tag.h"

namespace ddk
{

template<typename Enum, typename UnderlyingType = int>
class flag_set
{
	typedef UnderlyingType underlying_type;
	ASSERT_CONTAINS_SYMBOL(Enum,scoped_enum_tag,"Your enum shall be a scoped enum");
	ASSERT_CONTAINS_SYMBOL(Enum,underlying_type,"Your enum shall declare an underlying type");
	static_assert(std::is_integral<UnderlyingType>::value, "You shall provide a numeric underlying type");

public:
    flag_set()
	{
		m_flags = static_cast<underlying_type>(0);
	}
	template<typename EEnum>
    flag_set(const EEnum& i_flag1, const typename EEnum::underlying_type* = NULL)
	{
		m_flags = (1 << static_cast<underlying_type>(i_flag1));
	}
    flag_set(const Enum& i_flag1)
	{
		m_flags = (1 << static_cast<underlying_type>(i_flag1));
	}
    flag_set(const typename Enum::underlying_type& i_flag1)
	{
		m_flags = (1 << static_cast<underlying_type>(i_flag1));
	}
	static flag_set all()
	{
		return flag_set<Enum,UnderlyingType>((1 << static_cast<underlying_type>(Enum::Count)) - 1);
	}
	static flag_set none()
	{
		return flag_set();
	}
	flag_set operator|(const Enum& i_flag) const
	{
		flag_set<Enum,UnderlyingType> res;

		res.m_flags = m_flags | (1 << static_cast<underlying_type>(i_flag));

		return res;
	}
    flag_set& operator|=(const Enum& i_flag)
	{
		m_flags |= 1 << static_cast<underlying_type>(i_flag);

		return *this;
	}
    flag_set operator&(const Enum& i_flag) const
	{
		flag_set<Enum,UnderlyingType> res;
	
		res.m_flags = m_flags & (1 << static_cast<underlying_type>(i_flag));

		return res;
	}
    flag_set& operator&=(const Enum& i_flag)
	{
		m_flags &= 1 << static_cast<underlying_type>(i_flag);

		return *this;
	}
    flag_set operator^(const Enum& i_flag) const
	{
		flag_set<Enum,UnderlyingType> res;

		res.m_flags = m_flags ^ (1 << static_cast<underlying_type>(i_flag));

		return res;
	}
    flag_set& operator^=(const Enum& i_flag)
	{
		m_flags ^= 1 << static_cast<underlying_type>(i_flag);

		return *this;
	}
    flag_set operator|(const flag_set& other) const
	{
		flag_set<Enum,UnderlyingType> res;

		res.m_flags = m_flags | other.m_flags;

		return res;
	}
    flag_set& operator|=(const flag_set& other)
	{
		m_flags |= other.m_flags;

		return *this;
	}
    flag_set operator&(const flag_set& other) const
	{
		flag_set<Enum,UnderlyingType> res;

		res.m_flags = m_flags & other.m_flags;

		return res;
	}
    flag_set& operator&=(const flag_set& other)
	{
		m_flags &= other.m_flags;

		return *this;
	}
    flag_set operator^(const flag_set& other) const
	{
		flag_set<Enum,UnderlyingType> res;

		res.m_flags = m_flags ^ other.m_flags;

		return res;
	}
    flag_set& operator^=(const flag_set& other)
	{
		m_flags ^= other.m_flags;

		return *this;
	}
    flag_set operator!()
	{
		return !m_flags;
	}
    bool operator==(const flag_set& other) const
	{
		return m_flags == other.m_flags;
	}
    bool operator!=(const flag_set& other) const
	{
		return m_flags != other.m_flags;
	}
    void set(const Enum& i_flag)
	{
		m_flags |= 1 << static_cast<underlying_type>(i_flag);
	}
    bool test(const Enum& i_flag) const
	{
		bool res = (m_flags & (1 << static_cast<underlying_type>(i_flag))) != 0;

		return res;
	}
	void reset()
	{
		m_flags = static_cast<underlying_type>(0);
	}
	void reset(const Enum& i_flag)
	{
		m_flags &= ~(1 << static_cast<underlying_type>(i_flag));
	}
    bool empty() const
	{
		return m_flags == static_cast<underlying_type>(0);
	}
	underlying_type getValue() const
	{
		return m_flags;
	}
	inline operator int() const
	{
		return static_cast<int>(m_flags);
	}

private:
    flag_set(int i_content)
	: m_flags(i_content)
	{
	}

    underlying_type m_flags;
};

}