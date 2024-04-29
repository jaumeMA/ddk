//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

namespace ddk
{

template<typename T>
inline T* get_raw_ptr(T* i_ref);
template<typename T>
inline void set_raw_ptr(T*& i_ref,T* i_value);
template<typename T>
inline T* extract_raw_ptr(T*& i_ref);
template<typename T>
inline void clear_ptr(T*& i_ref);
template<typename T>
inline void rebase_ptr(T*& i_ref,std::ptrdiff_t i_diff);

namespace detail
{

template<typename T>
inline void rebase_ptr(T& i_ref,std::ptrdiff_t i_diff);

}
}

#include "ddk_pointer_defs.inl"
