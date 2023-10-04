#pragma once

namespace ddk
{

template<typename T>
class holder
{
public:
	holder(T& i_obj);
	holder(holder&&) = default;
	holder(const holder&) = default;

	inline operator bool() const;
	inline T& operator->();
	inline const T& operator->() const;
	inline T& operator*();
	inline const T& operator*()const;

private:
	T& m_obj;
};
template<typename T>
holder(T&) -> holder<T>;

}

#include "ddk_holder.inl"