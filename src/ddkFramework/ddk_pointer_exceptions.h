//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include <exception>
#include <string>

namespace ddk
{

class unmanaged_pointer_exception : public std::exception
{
public:
	unmanaged_pointer_exception(const std::string& i_reason);

	const char* what() const noexcept override;

private:
	const std::string m_reason;
};

class null_pointer_exception : public std::exception
{
public:
	null_pointer_exception(const std::string& i_reason);

	const char* what() const noexcept override;

private:
	const std::string m_reason;
};

}
