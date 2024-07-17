//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_type_concepts.h"
#include "ddk_concepts.h"

namespace ddk
{

template<typename T>
class embedded_ptr
{
	template<typename>
	friend class embedded_ptr;

	friend inline T* get_raw_ptr(embedded_ptr& i_ptr)
	{
		return &*i_ptr;
	}
	friend inline const T* get_raw_ptr(const embedded_ptr& i_ptr)
	{
		return &*i_ptr;
	}
	friend inline T* extract_raw_ptr(embedded_ptr& i_ptr)
	{
		T* res = &*i_ptr;

		i_ptr = nullptr;

		return res;
	}
	friend inline void clear_ptr(embedded_ptr& i_ptr)
	{
		i_ptr = nullptr;
	}

public:
	typedef T value_type;
	typedef typename std::add_const<T>::type const_value_type;
	typedef value_type& reference;
	typedef const_value_type& const_reference;
	typedef value_type&& rreference;
	typedef value_type* pointer;
	typedef const_value_type* const_pointer;

	embedded_ptr() = default;
	embedded_ptr(const std::nullptr_t&);
	embedded_ptr(const embedded_ptr&);
	embedded_ptr(embedded_ptr&&);
	embedded_ptr(T* i_ptr);
	TEMPLATE(typename TT)
	REQUIRES(IS_BASE_OF(T,TT))
	embedded_ptr(const embedded_ptr<TT>& other);
	TEMPLATE(typename TT)
	REQUIRES(IS_BASE_OF(T,TT))
	embedded_ptr(embedded_ptr<TT>&& other);
	TEMPLATE(typename TT)
	REQUIRES(IS_BASE_OF(T,TT))
	embedded_ptr(TT* i_ptr);

	embedded_ptr& operator=(const embedded_ptr&);
	TEMPLATE(typename TT)
	REQUIRES(IS_BASE_OF(T,TT))
	embedded_ptr& operator=(const embedded_ptr<TT>& other);
	T* operator->();
	const T* operator->() const;
	T& operator*();
	const T& operator*() const;
	operator bool() const;
	bool operator==(const embedded_ptr& other) const;
	bool operator!=(const embedded_ptr& other) const;

private:
	std::ptrdiff_t m_rel = 0;
};

}

#include "ddk_embedded_ptr.inl"