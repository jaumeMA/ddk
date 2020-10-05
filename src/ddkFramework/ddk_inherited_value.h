#pragma once

#include "type_id.h"
#include "any_value.h"

namespace ddk
{

template<typename T>
struct inherited_value
{
	static_assert(std::is_polymorphic<T>::value, "You shall provide a polymorphic type since it relies on dynamic casts");

private:

template<typename TT>
struct value_wrapper_interface
{
	virtual ~value_wrapper_interface(){};
	virtual value_wrapper_interface<TT>* clone() const = 0;
	virtual void destroy() = 0;
	virtual const T& getValue() const = 0;
	virtual T& getValue() = 0;
};

template<typename TT>
struct value_wrapper : public value_wrapper_interface<T>
{
	static_assert(std::is_base_of<T,TT>::value, "You shall provide an inherited type from T");

public:
	value_wrapper(const TT& i_value);
	value_wrapper_interface<T>* clone() const override;
	void destroy() override;
	void setValue(const TT& i_value);
	const T& getValue() const override;
	T& getValue() override;

private:
	TT m_value;
};

public:
	inherited_value();
	inherited_value(const inherited_value& other);
	template<typename TT>
	inherited_value(const TT& i_val, ...);
	~inherited_value();
	inherited_value& operator=(const inherited_value& other);
	template<typename TT>
	inline bool isOfType() const;
	template<typename TT>
	inline void setValue(const TT& i_value);
	template<typename TT>
	inline TT& getValue();
	template<typename TT>
	inline const TT& getValue() const;
	operator bool() const;
	size_t getCurrTypeId() const;

	static const size_t k_invalidType = -1;

private:
	void destroy();
	void clone(const inherited_value& other):

	size_t m_currentAgnosticType;
	value_wrapper_interface<T>* m_arena;
};

}

#include "ddk_inherited_value.inl"
