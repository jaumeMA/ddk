#pragma once

#include "ddk_template_helper.h"
#include "ddk_type_concepts.h"
#include "ddk_concepts.h"

namespace ddk
{

struct begin_action_tag
{
	typedef begin_action_tag recovery_tag;
};
struct last_action_tag
{
	typedef last_action_tag recovery_tag;
};
struct forward_action_tag
{
	typedef forward_action_tag recovery_tag;
};
struct backward_action_tag
{
	typedef backward_action_tag recovery_tag;
};
struct displace_action_tag
{
public:
	typedef long long difference_type;
	typedef displace_action_tag recovery_tag;

	displace_action_tag(difference_type i_displacement);

	difference_type displacement() const;

private:
	difference_type m_displacement;
};
template<typename T>
struct add_action_tag
{
public:
	TEMPLATE(typename ... Args)
	REQUIRES(IS_CONSTRUCTIBLE(T,Args...))
	add_action_tag(Args&& ... i_args);

	T extract() &&;

private:
	T m_payload;
};

struct remove_action_tag
{
};

}

#include "ddk_iterable_action_tag.inl"