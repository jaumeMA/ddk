#pragma once

#include "ddk_optional.h"
#include "ddk_iterable_action.h"

#define DDK_ITERABLE_TYPE(_ITERABLE,_ADAPTOR,_TAG) \
template<typename Action> \
friend _ADAPTOR adaptor_resolver(const _ITERABLE&,const Action&); \
friend _TAG iterable_tag_resolver(const _ITERABLE&);

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
	inline reference get_value() noexcept;
	inline const_reference get_value() const noexcept;
	inline pointer add_item(const_reference i_value) noexcept;
	inline pointer remove_item() noexcept;

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
	inline const_reference get_value() const noexcept;

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

	inline pointer next_value() noexcept;
	inline const_pointer next_value() const noexcept;
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
	inline pointer prev_value() noexcept;
	inline const_pointer prev_value() const noexcept;

protected:
	const reverse_iterator m_endReverseIterator;
};

template<typename Iterable>
class random_access_iterable_adaptor : public bidirectional_iterable_adaptor<Iterable>
{
public:
	using bidirectional_iterable_adaptor<Iterable>::bidirectional_iterable_adaptor;
	using typename bidirectional_iterable_adaptor<Iterable>::reference;
	using typename bidirectional_iterable_adaptor<Iterable>::const_reference;
	using typename bidirectional_iterable_adaptor<Iterable>::pointer;
	using typename bidirectional_iterable_adaptor<Iterable>::const_pointer;

	inline pointer shift_value(int i_shift) noexcept;
	inline const_pointer shift_value(int i_shift) const noexcept;
};

template<typename Iterable, typename Action>
forward_iterable_adaptor<Iterable> adaptor_resolver(Iterable&, Action,typename std::enable_if<std::is_same<input_action,Action>::value || std::is_same<const_input_action,Action>::value>::type* = nullptr);
template<typename Iterable,typename Action>
bidirectional_iterable_adaptor<Iterable> adaptor_resolver(Iterable&,Action,typename std::enable_if<std::is_same<bidirectional_action,Action>::value || std::is_same<const_bidirectional_action,Action>::value>::type* = nullptr);
template<typename Iterable,typename Action>
random_access_iterable_adaptor<Iterable> adaptor_resolver(Iterable&,Action,typename std::enable_if<std::is_same<random_access_action,Action>::value || std::is_same<const_random_access_action,Action>::value>::type* = nullptr);

template<typename Iterable, typename Action>
struct iterable_adaptor
{
	typedef decltype(adaptor_resolver(std::declval<Iterable>(),std::declval<Action>())) type;
};

}
}
}

#include "ddk_iterable_adaptor.inl"