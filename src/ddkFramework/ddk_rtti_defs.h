#pragma once

#include <string>

namespace ddk
{
namespace detail
{

struct rtti_tag_t;

}

struct TypeInfo
{
	template<typename,typename>
	friend TypeInfo make_type_info();
	template<typename>
	friend TypeInfo make_type_info();

public:
	static const size_t s_invalid = -1;

	TypeInfo() = default;
	size_t get_id() const;
	bool operator==(const TypeInfo& other) const;
	bool empty() const;

private:
	TypeInfo(size_t i_id);

	size_t m_id = s_invalid;
};

template<typename T, typename TT>
inline TypeInfo make_type_info();

template<typename T>
inline TypeInfo make_type_info();

}

#include "ddk_rtti_defs.inl"