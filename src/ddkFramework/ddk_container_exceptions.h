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

class bad_access_exception : public std::exception
{
public:
    bad_access_exception(const std::string& i_reason);

    std::string reason() const;

private:
    const std::string m_reason;
};

}
