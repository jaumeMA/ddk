#include "ddk_iterable_action_tags.h"

namespace ddk
{

displace_action_tag::displace_action_tag(difference_type i_displacement)
: m_displacement(i_displacement)
{
}
displace_action_tag::difference_type displace_action_tag::displacement() const
{
	return m_displacement;
}

}