#pragma once

#include <string>

namespace ewas
{
namespace detail
{

struct rtti_tag_t;

template<typename T>
struct is_rtti_available
{
private:
	typedef char yes;
	typedef int no;

	template<typename TT>
	static inline yes _checker(const TT*, const typename TT::rtti_tag* = NULL);
	template<typename TT>
	static inline no _checker(const TT*,...);

public:
	static const bool value = (sizeof(_checker(reinterpret_cast<const T*>(NULL),0)) == sizeof(yes));
};

}

struct TypeInfo
{
public:
	TypeInfo();
	TypeInfo(const std::string& i_name);
	TypeInfo(const char* i_name);
	TypeInfo(size_t i_nameHash);
	std::string get_name() const;
	size_t get_name_hash() const;
	bool operator==(const TypeInfo& other) const;

private:
	std::string m_name;
	size_t m_nameHash;
};

}
