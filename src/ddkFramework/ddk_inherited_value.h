#pragma once

#include "ddk_macros.h"
#include "type_id.h"
#include "any_value.h"

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
	value_wrapper(const TT& i_value)
	: m_value(i_value)
	{}
	virtual value_wrapper_interface<T>* clone() const override
	{
		return new value_wrapper<TT>(m_value);
	}
	virtual void destroy() override
	{
		//if anybody access this data after destruction something  very bad will happen
		m_value.~TT();
	}
	void setValue(const TT& i_value)
	{
		m_value = i_value;
	}
	virtual const T& getValue() const override
	{
		return m_value;
	}
	virtual T& getValue() override
	{
		return m_value;
	}

private:
	TT m_value;
};

public:
	inherited_value()
	: m_currentAgnosticType(k_invalidType)
	{
	}
	inherited_value(const inherited_value& other)
	{
		clone(other);
	}
	template<typename TT>
	inherited_value(const TT& i_val, ...)
	: m_currentAgnosticType(k_invalidType)
	{
		static_assert(std::is_base_of<T,TT>::value, "You shall provide an inherited type from T");

		static const size_t s_TypeHashCode = ddk::getTypeId<T>();

		m_currentAgnosticType = s_TypeHashCode;

		m_arena = new value_wrapper<TT>(i_val);
	}
	~inherited_value()
	{
		destroy();
	}
	inherited_value& operator=(const inherited_value& other)
	{
		destroy();

		clone(other);

		return *this;
	}
	template<typename TT>
	inline bool isOfType() const
	{
		static_assert(std::is_base_of<T,TT>::value, "You shall provide an inherited type from T");
		static const size_t s_TypeHashCode = ddk::getTypeId<T>();

		return m_currentAgnosticType == s_TypeHashCode;
	}
	template<typename TT>
	inline void setValue(const TT& i_value)
	{
		static_assert(std::is_base_of<T,TT>::value, "You shall provide an inherited type from T");

		static const size_t s_TypeHashCode = ddk::getTypeId<T>();

		if(m_currentAgnosticType == s_TypeHashCode)
		{
			value_wrapper<TT>* innerValue = dynamic_cast<value_wrapper<TT>*>(m_arena);

			innerValue->setValue(i_value);
		}
		else
		{
			destroy();

			m_currentAgnosticType = s_TypeHashCode;

			m_arena = new value_wrapper<TT>(i_value);
		}
	}
	template<typename TT>
	inline TT& getValue()
	{
		static_assert(std::is_base_of<T,TT>::value, "You shall provide an inherited type from T");

		static const size_t s_TypeHashCode = ddk::getTypeId<T>();
		static const size_t s_InterfaceHashCode = ddk::getTypeId<T>();

		if(s_TypeHashCode == s_InterfaceHashCode || m_currentAgnosticType == s_TypeHashCode)
		{
			return dynamic_cast<TT&>(m_arena->getValue());
		}
		else
		{
			MAKE_IT_CRASH

			return *reinterpret_cast<TT*>(0xDEAD);
		}
	}
	template<typename TT>
	inline const TT& getValue() const
	{
		static_assert(std::is_base_of<T,TT>::value, "You shall provide an inherited type from T");

		static const size_t s_TypeHashCode = ddk::getTypeId<T>();
		static const size_t s_InterfaceHashCode = ddk::getTypeId<T>();

		if(s_TypeHashCode == s_InterfaceHashCode || m_currentAgnosticType == s_TypeHashCode)
		{
			return dynamic_cast<const TT&>(m_arena->getValue());
		}
		else
		{
			MAKE_IT_CRASH

			return *reinterpret_cast<const TT*>(0xDEAD);
		}
	}
	operator bool() const
	{
		return m_currentAgnosticType != k_invalidType;
	}
	size_t getCurrTypeId() const
	{
		return m_currentAgnosticType;
	}

	static const size_t k_invalidType = -1;

private:
	void destroy()
	{
		if(m_currentAgnosticType != k_invalidType)
		{
			delete m_arena;

			m_arena = NULL;

			m_currentAgnosticType = k_invalidType;
		}
	}
	void clone(const inherited_value& other)
	{
		if(other.m_currentAgnosticType != k_invalidType)
		{
			m_arena = other.m_arena->clone();
		}

		m_currentAgnosticType = other.m_currentAgnosticType;
	}

	size_t m_currentAgnosticType;
	value_wrapper_interface<T>* m_arena;
};
