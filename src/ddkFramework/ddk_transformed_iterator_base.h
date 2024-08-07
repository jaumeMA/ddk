//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include <iterator>

namespace ddk
{
namespace detail
{

template<typename IteratorTag>
class IIteratorBase;

template<typename ReturnType, typename IteratorTag>
class IIterator : public IIteratorBase<IteratorTag>
{
public:
	virtual ReturnType getValue() const = 0;
	virtual IIterator<ReturnType,IteratorTag>* clone() const = 0;
};

template<typename Iterator>
class IIterator_private
{
public:
	virtual ~IIterator_private(){};
	virtual Iterator& get_private_iterator() = 0;
	virtual const Iterator& get_private_iterator() const = 0;
};

template<typename,typename,typename>
class PrivateIterator;

}
}