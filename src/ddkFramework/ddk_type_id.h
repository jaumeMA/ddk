#pragma once

#include "ddk_formatter.h"
#include "ddk_type_concepts.h"
#include "ddk_concepts.h"
#include <sstream>

namespace ddk
{

template<typename UnderlyingType, typename T>
struct Id
{
	friend inline std::string& operator<<(std::string& o_str, const Id<UnderlyingType,T>& i_value)
	{
		return o_str << i_value.m_key;
	}
	friend inline std::ostringstream& operator<<(std::ostringstream& o_str, const Id<UnderlyingType,T>& i_value)
	{
		o_str << i_value.m_key;

		return o_str;
	}
	friend inline Id<UnderlyingType,T> operator+(const Id<UnderlyingType,T>& i_lhs, const Id<UnderlyingType,T>& i_rhs)
	{
		return Id<UnderlyingType,T>(i_lhs.m_key + i_rhs.m_key);
	}

public:
	typedef UnderlyingType underlying_type;

	explicit Id() = default;
	Id(const Id&) = default;
	Id(Id&&) = default;
	explicit Id(const UnderlyingType& i_key);
	explicit Id(UnderlyingType&& i_key);
	TEMPLATE(typename UUnderlyingType, typename TT)
	REQUIRES(IS_CONVERTIBLE(UnderlyingType,UUnderlyingType))
	explicit Id(const Id<UUnderlyingType,TT>& other);
	Id& operator=(const Id&) = default;
	Id& operator=(Id&&) = default;
	inline bool operator==(const Id& other) const;
	inline bool operator!=(const Id& other) const;
	inline bool operator==(const UnderlyingType& other) const;
	inline bool operator!=(const UnderlyingType& other) const;
	inline bool operator<(const Id& other) const;
	inline bool operator<=(const Id& other) const;
	inline bool operator>(const Id& other) const;
	inline bool operator>=(const Id& other) const;
	inline UnderlyingType* operator->();
	inline const UnderlyingType* operator->() const;
	inline const UnderlyingType& getValue() const;

private:
	UnderlyingType m_key;
};

template<typename UnderlyingType,typename T>
struct formatter<Id<UnderlyingType,T>>
{
	inline static Id<UnderlyingType,T> format(const std::string& i_str);
};

}

namespace std
{

template<typename UnderlyingType, typename T>
struct hash<ddk::Id<UnderlyingType,T>>
{
    inline std::size_t operator()(const ddk::Id<UnderlyingType,T> s) const noexcept;
};

}

#include "ddk_type_id.inl"