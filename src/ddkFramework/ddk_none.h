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
namespace detail
{

struct none_t
{
};

}
}

const ddk::detail::none_t none = ddk::detail::none_t();
