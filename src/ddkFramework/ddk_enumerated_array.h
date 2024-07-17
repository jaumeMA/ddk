//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include <array>

namespace ddk
{

template<typename T, typename Enum, std::size_t Size = Enum::Count>
class enumerated_array : public std::array<T,Size>
{
public:
	using std::array<T,Size>::operator[];

	T& operator[](const Enum& i_case)
	{
		return std::array<T,Size>::operator[](static_cast<std::size_t>(i_case));
	}
	const T& operator[](const Enum& i_case) const
	{
		return std::array<T,Size>::operator[](static_cast<std::size_t>(i_case));
	}
};

}
