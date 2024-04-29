//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_thread_local_storage.h"

namespace ddk
{

template<typename T, typename Tag = T, typename Allocator = system_allocator>
class threadlocal
{
	static const size_t k_typeSize = mpl::size_of_qualified_type<T>::value;

public:
	threadlocal() = default;
	threadlocal(const threadlocal&) = delete;
	threadlocal(threadlocal&&) = default;
	template<typename ... Args>
	threadlocal(Args&& ... i_args);
	~threadlocal() = default;

	template<typename ... Args>
	auto acquire(Args&& ... i_args);
	template<typename ... Args>
	auto set(Args&& ... i_args);
	auto get();
	auto get() const;
	auto get_ptr();
	auto get_ptr() const;
	auto extract() &&;
	auto operator*();
	auto operator*() const;
	auto operator->();
	auto operator->() const;
	bool empty() const;
	void clear();

private:
	inline thread_local_storage<T,Allocator>& get_address() const;
};

}

#include "ddk_thread_local.inl"