//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_embedded_ptr.h"

namespace ddk
{

template<typename T>
class embedded_ref : public embedded_ptr<T>
{
public:
	using embedded_ptr<T>::embedded_ptr;
	embedded_ref() = delete;

	embedded_ref& operator=(const embedded_ref&) = delete;
};

}