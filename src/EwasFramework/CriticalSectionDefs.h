#pragma once

#include "ewas_scoped_enum.h"
#include "type_id.h"

struct critical_access_identifier_t;
typedef Id<size_t,critical_access_identifier_t> CriticalAccessIdentifier;
const CriticalAccessIdentifier k_invalidCriticalAccessIdentifier = CriticalAccessIdentifier(0);

SCOPED_ENUM_DECL(Reentrancy,
		REENTRANT,
		NON_REENTRANT
	);

namespace detail
{

struct context_acquired_t
{
	context_acquired_t(bool i_acquired)
	: m_acquired(i_acquired)
	{}

	bool m_acquired;
};

}
