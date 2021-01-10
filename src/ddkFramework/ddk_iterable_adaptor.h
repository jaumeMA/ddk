#pragma once

#include "ddk_optional.h"
#include "ddk_iterable_action.h"

namespace ddk
{
namespace iter
{
namespace detail
{

template<typename>
class iterable_adaptor_base;
	
template<typename Iterable>
class iterable_adaptor_base
{
public:
	typedef typename Iterable::iterator iterator;
	typedef typename Iterable::const_iterator const_iterator;
	typedef typename Iterable::reference reference;
	typedef typename Iterable::const_reference const_reference;
	typedef typename Iterable::pointer pointer;
	typedef typename Iterable::const_pointer const_pointer;

	template<typename Action>
	iterable_adaptor_base(Iterable& i_iterable, Action&& i_initialAction);

	inline bool valid() const noexcept;
	template<typename Sink>
	inline bool forward_add_value_in(const_reference i_value, Sink&& i_sink);
	template<typename Sink>
	inline bool forward_erase_value_in(Sink&& i_sink);

protected:
	Iterable& m_iterable;
	iterator m_currIterator;
	const iterator m_endIterator;
};

template<typename Iterable>
class iterable_adaptor_base<const Iterable>
{
public:
	typedef typename Iterable::iterator iterator;
	typedef typename Iterable::const_iterator const_iterator;
	typedef typename Iterable::const_reference reference;
	typedef typename Iterable::const_reference const_reference;
	typedef typename Iterable::const_pointer pointer;
	typedef typename Iterable::const_pointer const_pointer;

	template<typename Action>
	iterable_adaptor_base(const Iterable& i_iterable,Action&& i_initialAction);

	inline bool valid() const noexcept;

protected:
	const Iterable& m_iterable;
	const_iterator m_currIterator;
	const const_iterator m_endIterator;
};

template<typename Iterable>
class forward_iterable_adaptor : public iterable_adaptor_base<Iterable>
{
public:
	using iterable_adaptor_base<Iterable>::iterable_adaptor_base;
	using typename iterable_adaptor_base<Iterable>::reference;
	using typename iterable_adaptor_base<Iterable>::const_reference;
	using typename iterable_adaptor_base<Iterable>::pointer;
	using typename iterable_adaptor_base<Iterable>::const_pointer;

	template<typename Sink>
	inline bool forward_next_value_in(Sink&& i_sink);
	template<typename Sink>
	inline bool forward_next_value_in(Sink&& i_sink) const;
};

template<typename Iterable>
class bidirectional_iterable_adaptor : public forward_iterable_adaptor<Iterable>
{
	typedef typename mpl::static_if<std::is_const<Iterable>::value,typename Iterable::const_reverse_iterator,typename Iterable::reverse_iterator>::type reverse_iterator;

public:
	using typename forward_iterable_adaptor<Iterable>::reference;
	using typename forward_iterable_adaptor<Iterable>::const_reference;
	using typename forward_iterable_adaptor<Iterable>::pointer;
	using typename forward_iterable_adaptor<Iterable>::const_pointer;

	template<typename Action>
	bidirectional_iterable_adaptor(Iterable& i_iterable,Action&& i_initialAction);
	template<typename Sink>
	inline bool forward_prev_value_in(Sink&& i_sink);
	template<typename Sink>
	inline bool forward_prev_value_in(Sink&& i_sink) const;

protected:
	const reverse_iterator m_endReverseIterator;
};

template<typename>
class random_access_iterable_adaptor;

template<typename Iterable>
class random_access_iterable_adaptor : public bidirectional_iterable_adaptor<Iterable>
{
public:
	using bidirectional_iterable_adaptor<Iterable>::bidirectional_iterable_adaptor;
	using typename bidirectional_iterable_adaptor<Iterable>::reference;
	using typename bidirectional_iterable_adaptor<Iterable>::const_reference;
	using typename bidirectional_iterable_adaptor<Iterable>::pointer;
	using typename bidirectional_iterable_adaptor<Iterable>::const_pointer;

	template<typename Sink>
	inline bool forward_shift_value_in(int i_shift, Sink&& i_sink);
	template<typename Sink>
	inline bool forward_shift_value_in(int i_shift,Sink&& i_sink) const;
};

template<typename Iterable, typename Action>
const forward_iterable_adaptor<Iterable>& adaptor_resolver(Iterable&, Action,typename std::enable_if<std::is_same<input_action,Action>::value || std::is_same<const_input_action,Action>::value>::type* = nullptr);
template<typename Iterable,typename Action>
const bidirectional_iterable_adaptor<Iterable>& adaptor_resolver(Iterable&,Action,typename std::enable_if<std::is_same<bidirectional_action,Action>::value || std::is_same<const_bidirectional_action,Action>::value>::type* = nullptr);
template<typename Iterable,typename Action>
const random_access_iterable_adaptor<Iterable>& adaptor_resolver(Iterable&,Action,typename std::enable_if<std::is_same<random_access_action,Action>::value || std::is_same<const_random_access_action,Action>::value>::type* = nullptr);

template<typename Iterable, typename Action>
struct iterable_adaptor
{
private:
	typedef decltype(adaptor_resolver(std::declval<Iterable>(),std::declval<Action>())) raw_type;

public:
	typedef typename std::remove_const<typename std::remove_reference<raw_type>::type>::type type;
};

}
}
}

#include "ddk_iterable_adaptor.inl"