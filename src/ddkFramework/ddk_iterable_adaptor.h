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

	template<typename Action>
	iterable_adaptor_base(Iterable& i_iterable, Action&& i_initialAction);

	inline reference get_value();
	inline const_reference get_value() const;
	inline optional<reference> add_item(const_reference i_value);
	inline optional<reference> remove_item();

protected:
	Iterable& m_iterable;
	iterator m_currIterator;
};

template<typename Iterable>
class iterable_adaptor_base<const Iterable>
{
public:
	typedef typename Iterable::iterator iterator;
	typedef typename Iterable::const_iterator const_iterator;
	typedef typename Iterable::const_reference reference;
	typedef typename Iterable::const_reference const_reference;

	template<typename Action>
	iterable_adaptor_base(const Iterable& i_iterable,Action&& i_initialAction);

	inline const_reference get_value() const;

protected:
	const Iterable& m_iterable;
	const_iterator m_currIterator;
};

template<typename Iterable>
class forward_iterable_adaptor : public iterable_adaptor_base<Iterable>
{
public:
	using iterable_adaptor_base<Iterable>::iterable_adaptor_base;
	using typename iterable_adaptor_base<Iterable>::reference;
	using typename iterable_adaptor_base<Iterable>::const_reference;

	inline optional<reference> next_value();
	inline optional<const_reference> next_value() const;
};

template<typename Iterable>
class bidirectional_iterable_adaptor : public forward_iterable_adaptor<Iterable>
{
public:
	using forward_iterable_adaptor<Iterable>::forward_iterable_adaptor;
	using typename forward_iterable_adaptor<Iterable>::reference;
	using typename forward_iterable_adaptor<Iterable>::const_reference;

	inline optional<reference> prev_value();
	inline optional<const_reference> prev_value() const;
};

template<typename Iterable>
class random_access_iterable_adaptor : public bidirectional_iterable_adaptor<Iterable>
{
public:
	using bidirectional_iterable_adaptor<Iterable>::bidirectional_iterable_adaptor;
	using typename bidirectional_iterable_adaptor<Iterable>::reference;
	using typename bidirectional_iterable_adaptor<Iterable>::const_reference;

	inline optional<reference> shift_value(int i_shift);
	inline optional<const_reference> shift_value(int i_shift) const;
};

template<typename Iterable, typename Action>
forward_iterable_adaptor<Iterable> adaptor_resolver(Iterable&, Action,typename std::enable_if<std::is_same<input_action,Action>::value>::type* = nullptr);
template<typename Iterable,typename Action>
bidirectional_iterable_adaptor<Iterable> adaptor_resolver(Iterable&,Action,typename std::enable_if<std::is_same<bidirectional_action,Action>::value>::type* = nullptr);
template<typename Iterable,typename Action>
random_access_iterable_adaptor<Iterable> adaptor_resolver(Iterable&,Action,typename std::enable_if<std::is_same<random_access_action,Action>::value>::type* = nullptr);

template<typename Iterable, typename Action>
struct iterable_adaptor
{
	typedef decltype(adaptor_resolver(std::declval<Iterable>(),std::declval<Action>())) type;
};

}
}
}

#include "ddk_iterable_adaptor.inl"