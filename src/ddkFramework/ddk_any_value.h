#pragma once

#include "ddk_macros.h"
#include "ddk_any_value_wrapper.h"
#include "ddk_type_id.h"
#include "ddk_tagged_pointer.h"
#include "ddk_crash.h"

namespace ddk
{
namespace detail
{

inline size_t __getTypeId__()
{
	static size_t s_typeId = 0;

	return s_typeId++;
}

}

template<typename T>
inline size_t getTypeId()
{
	static const size_t s_typeId = detail::__getTypeId__();

	return s_typeId;
}

struct any_value
{
typedef value_wrapper_interface* InterfacePtr;

public:
	static const size_t k_invalidType = -1;

	any_value();
	any_value(const any_value& other);
	template<typename T>
	any_value(const T& i_val, typename std::enable_if<std::is_same<T,any_value>::value == false>::type* = NULL)
	: m_currentAgnosticType(k_invalidType)
	{
		static const size_t s_TypeHashCode = getTypeId<T>();

		m_currentAgnosticType = s_TypeHashCode;

		m_arena = make_tagged_pointer<value_wrapper<T>>(0x01,i_val);
	}
	template<typename T>
	any_value(void* i_arena)
	: m_currentAgnosticType(k_invalidType)
	, m_arena(NULL)
	{
		if(i_arena)
		{
			//just for the sake of size
			m_arena = tagged_pointer<null_value_wrapper<T>>(new (i_arena) null_value_wrapper<T>(),0x01);
		}
	}
	~any_value();
	any_value& operator=(const any_value& other);
	template<typename T>
	inline bool isOfType() const
	{
		static const size_t s_TypeHashCode = getTypeId<T>();

		return m_currentAgnosticType == s_TypeHashCode;
	}
	template<typename T, typename TT>
	inline void setValue(TT&& i_value)
	{
		static const size_t s_TypeHashCode = getTypeId<T>();

		if(m_currentAgnosticType == s_TypeHashCode)
		{
			value_wrapper<T>* innerValue = static_cast<value_wrapper<T>*>(m_arena.get_pointer());

			innerValue->setValue(std::forward<TT>(i_value),m_arena.get_pointer());
		}
		else
		{
			const bool fitsNewValue = getArenaSize() >= sizeof(value_wrapper<T>);

			destroy();

			if(m_arena.is_tagged())
			{
				if(fitsNewValue)
				{
					m_arena = tagged_pointer<value_wrapper<T>>(new (m_arena.get_pointer()) value_wrapper<T>(std::forward<TT>(i_value)),0x01);
				}
				else
				{
					m_arena = make_tagged_pointer<value_wrapper<T>>(0x01,std::forward<TT>(i_value));
				}
			}
			else
			{
				m_arena = make_tagged_pointer<value_wrapper<T>>(0x01,std::forward<TT>(i_value));
			}

			m_currentAgnosticType = s_TypeHashCode;
		}
	}
	template<typename T>
	inline typename value_wrapper<T>::lreference getValue()
	{
		static const size_t s_TypeHashCode = getTypeId<T>();

		if(m_currentAgnosticType == s_TypeHashCode)
		{
			if(value_wrapper<T>* innerValue = static_cast<value_wrapper<T>*>(m_arena.get_pointer()))
			{
				return innerValue->getValue();
			}
			else
			{
				MAKE_IT_CRASH

				return ewas::crash_on_return<typename value_wrapper<T>::lreference>::value();
			}
		}
		else
		{
			MAKE_IT_CRASH

			return ewas::crash_on_return<typename value_wrapper<T>::lreference>::value();
		}
	}
	template<typename T>
	inline typename value_wrapper<T>::const_lreference getValue() const
	{
		static const size_t s_TypeHashCode = getTypeId<T>();

		if(m_currentAgnosticType == s_TypeHashCode)
		{
			if(const value_wrapper<T>* innerValue = static_cast<const value_wrapper<T>*>(m_arena.get_pointer()))
			{
				return innerValue->getValue();
			}
			else
			{
				MAKE_IT_CRASH

				return ewas::crash_on_return<typename value_wrapper<T>::const_lreference>::value();
			}
		}
		else
		{
			MAKE_IT_CRASH

			return ewas::crash_on_return<typename value_wrapper<T>::const_lreference>::value();
		}
	}
	template<typename T>
	inline typename value_wrapper<T>::raw_type extractValue()
	{
		static const size_t s_TypeHashCode = getTypeId<T>();

		if(m_currentAgnosticType == s_TypeHashCode)
		{
			if(value_wrapper<T>* innerValue = static_cast<value_wrapper<T>*>(m_arena.get_pointer()))
			{
				typename value_wrapper<T>::raw_type res = innerValue->extractValue();

				destroy();

				return std::move(res);
			}
			else
			{
				MAKE_IT_CRASH

				return ewas::crash_on_return<typename value_wrapper<T>::rreference>::value();
			}
		}
		else
		{
			MAKE_IT_CRASH

			return ewas::crash_on_return<typename value_wrapper<T>::rreference>::value();
		}
	}
	operator bool() const;
	size_t getCurrTypeId() const;
	void clear();

private:
	void destroy();
	void clone(const any_value& other);
	virtual size_t getArenaSize() const;

	size_t m_currentAgnosticType;
	tagged_pointer<value_wrapper_interface> m_arena;
};

}
