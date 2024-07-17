//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_exclusion_area.h"

namespace ddk
{

struct lock_reader
{
public:
	lock_reader(exclusion_area& m_exclArea);

	void lock(const Reentrancy& i_reentrancy);
	bool try_lock(const Reentrancy& i_reentrancy);
	void unlock();

private:
	exclusion_area& m_exclArea;
};

struct lock_writer
{
public:
	lock_writer(exclusion_area& m_exclArea);

	void lock(const Reentrancy& i_reentrancy);
	bool try_lock(const Reentrancy& i_reentrancy);
	void unlock();

private:
	exclusion_area& m_exclArea;
};

}