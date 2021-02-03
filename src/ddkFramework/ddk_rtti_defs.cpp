#include "ddk_rtti_defs.h"
#include "ddk_hash.h"

namespace ddk
{
namespace rtti
{

TypeInfo::TypeInfo(size_t i_id)
: m_id(i_id)
{
}
size_t TypeInfo::get_id() const
{
	return m_id;
}
bool TypeInfo::operator==(const TypeInfo& other) const
{
	return m_id == other.m_id;
}
bool TypeInfo::empty() const
{
	return m_id == s_invalid;
}

}
}
