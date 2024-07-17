//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_atomics.h"
#include "ddk_cond_var.h"

namespace ddk
{

struct counter_variable
{
public:
	counter_variable(int i_initCounter = 0);

	void push() const;
	void pull() const;
	int get_value() const;

private:
	mutable mutex m_mutex;
	mutable cond_var m_condVar;
	mutable int m_reference;
};

}