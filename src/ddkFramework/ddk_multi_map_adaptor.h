//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_multi_map.h"
#include "ddk_iterable_action_defs.h"
#include "ddk_awaitable.h"

namespace ddk
{

template<typename Key,typename Value,template<typename,typename,template<typename>class> class Map,template<typename> class Allocator>
class iterable_adaptor<multi_map<Key,Value,Map,Allocator>>
{
	typedef typename multi_map<Key,Value,Map,Allocator>::value_t value_t;
	typedef typename value_t::iterator iterator;

public:
	typedef std::pair<const std::vector<Key>,Value&> value_type;
	typedef std::pair<const std::vector<Key>,Value&>& reference;
	typedef std::pair<const std::vector<Key>,const Value&>& const_reference;
	typedef long long difference_type;

	iterable_adaptor(multi_map<Key,Value,Map,Allocator>& i_iterable);

	inline auto get_value();
	inline auto get_value() const;
	template<typename Sink>
	inline auto forward_value(Sink&& i_sink);
	template<typename Sink>
	inline auto forward_value(Sink&& i_sink) const;
	inline bool perform_action(const begin_action_tag&) const;
	inline bool perform_action(const end_action_tag&) const;
	inline bool perform_action(const forward_action_tag&) const;
	inline bool perform_action(const backward_action_tag&) const;
	inline bool perform_action(const displace_action_tag&) const;
	inline bool valid() const;

private:
	multi_map<Key,Value,Map,Allocator>& m_iterable;
};

template<typename Key,typename Value,template<typename,typename,template<typename>class> class Map,template<typename> class Allocator>
class iterable_adaptor<const multi_map<Key,Value,Map,Allocator>>
{
	typedef typename multi_map<Key,Value,Map,Allocator>::value_t value_t;
	typedef typename value_t::iterator iterator;

public:
	typedef std::pair<const std::vector<Key>,const Value&> value_type;
	typedef std::pair<const std::vector<Key>,const Value&>& reference;
	typedef std::pair<const std::vector<Key>,const Value&>& const_reference;
	typedef long long difference_type;

	iterable_adaptor(const multi_map<Key,Value,Map,Allocator>& i_iterable);
	inline auto get_value() const;
	template<typename Sink>
	inline auto forward_value(Sink&& i_sink) const;
	inline bool perform_action(const begin_action_tag&) const;
	inline bool perform_action(const end_action_tag&) const;
	inline bool perform_action(const forward_action_tag&) const;
	inline bool perform_action(const backward_action_tag&) const;
	inline bool perform_action(const displace_action_tag&) const;
	inline bool valid() const;

private:
	const multi_map<Key,Value,Map,Allocator>& m_iterable;
};

}

#include "ddk_multi_map_adaptor.inl"