//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include "ddk_type_id.h"

namespace ddk
{

size_t hash(const std::string& i_value);
template<typename Hasher>
inline size_t hash(const Hasher& i_hasher,const std::string& i_value);

#if !defined(WIN32)

constexpr size_t hash(const int& i_value);
constexpr size_t hash(const unsigned long& i_value);
template<typename Hasher>
constexpr size_t hash(const Hasher& i_hasher,const int& i_value);
template<typename Hasher>
constexpr size_t hash(const Hasher& i_hasher,const unsigned long& i_value);

#endif

constexpr size_t hash(const time_t& i_value);
template<typename UnderlyingType,typename T>
constexpr size_t hash(const Id<UnderlyingType,T>& i_id);
template<typename Hasher>
constexpr size_t hash(const Hasher& i_hasher,const time_t& i_value);
template<typename Hasher,typename UnderlyingType,typename T>
constexpr size_t hash(const Hasher& i_hasher,const Id<UnderlyingType,T>& i_id);
template<typename Arg, typename ... Args>
constexpr size_t hash_combine(Arg&& i_arg, Args&& ... i_args);

struct builtin_hasher
{
	constexpr builtin_hasher() = default;
	constexpr builtin_hasher(size_t i_seed);
	template<typename T>
	constexpr builtin_hasher(T&& i_value);

	template<typename ... Args>
	size_t operator()(Args&& ... i_args) const;
	constexpr size_t get() const;

protected:
	mutable size_t m_hash = 0;
};

struct commutative_builtin_hasher: builtin_hasher
{
	using builtin_hasher::builtin_hasher;

	template<typename ... Args>
	size_t operator()(Args&& ... i_args) const;
};

}

#include "ddk_hash.inl"