#include "ddk_rtti_defs.h"
#include "ddk_hash.h"

namespace ddk
{

TypeInfo::TypeInfo()
: m_nameHash(0)
{
}
TypeInfo::TypeInfo(const char* i_name)
: m_name(i_name)
{
	m_nameHash = ewas::hash(m_name);
}
TypeInfo::TypeInfo(const std::string& i_name)
: m_name(i_name)
{
	m_nameHash = ewas::hash(m_name);
}
TypeInfo::TypeInfo(size_t i_nameHash)
: m_nameHash(i_nameHash)
{
}
std::string TypeInfo::get_name() const
{
	return m_name;
}
size_t TypeInfo::get_name_hash() const
{
	return m_nameHash;
}
bool TypeInfo::operator==(const TypeInfo& other) const
{
	return m_nameHash == other.m_nameHash;
}

}
