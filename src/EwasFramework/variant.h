#pragma once

#include "any_value.h"
#include "ewas_template_helper.h"

namespace ewas
{
namespace detail
{

template<bool,typename,typename>
struct _max_type2;

template<typename T1, typename T2>
struct _max_type2<true,T1,T2>
{
	typedef T1 type;
};

template<typename T1, typename T2>
struct _max_type2<false,T1,T2>
{
	typedef T2 type;
};

template<bool,typename,typename,typename>
struct _max_type3;

template<typename T1,typename T2,typename T3>
struct _max_type3<true,T1,T2,T3>
{
	typedef typename _max_type2<(mpl::size_of_qualified_type<T1>::value>mpl::size_of_qualified_type<T3>::value),T1,T3>::type type;
};

template<typename T1,typename T2,typename T3>
struct _max_type3<false,T1,T2,T3>
{
	typedef typename _max_type2<(mpl::size_of_qualified_type<T2>::value>mpl::size_of_qualified_type<T3>::value),T2,T3>::type type;
};

template<bool,typename,typename,typename,typename>
struct _max_type4;

template<typename T1,typename T2,typename T3,typename T4>
struct _max_type4<true,T1,T2,T3,T4>
{
	typedef typename _max_type3<(mpl::size_of_qualified_type<T1>::value>mpl::size_of_qualified_type<T3>::value),T1,T3,T4>::type type;
};

template<typename T1,typename T2,typename T3,typename T4>
struct _max_type4<false,T1,T2,T3,T4>
{
	typedef typename _max_type3<(mpl::size_of_qualified_type<T2>::value>mpl::size_of_qualified_type<T3>::value),T2,T3,T4>::type type;
};


template<typename T1, typename T2>
struct max_type2
{
	typedef typename _max_type2<(mpl::size_of_qualified_type<T1>::value>mpl::size_of_qualified_type<T2>::value),T1,T2>::type type;
};
template<typename T1, typename T2, typename T3>
struct max_type3
{
	typedef typename _max_type3<(mpl::size_of_qualified_type<T1>::value>mpl::size_of_qualified_type<T2>::value),T1,T2,T3>::type type;
};
template<typename T1, typename T2, typename T3, typename T4>
struct max_type4
{
	typedef typename _max_type4<(mpl::size_of_qualified_type<T1>::value>mpl::size_of_qualified_type<T2>::value),T1,T2,T3,T4>::type type;
};

}

template<typename T1, typename T2>
struct variant2 : protected any_value
{
	typedef value_wrapper<typename detail::max_type2<T1,T2>::type> variant_type;
	static const size_t s_type1HashCode;
	static const size_t s_type2HashCode;
	static const size_t s_numTypes = 2;

#if defined(WIN32) && defined(EWAS_DEBUG)
	typedef value_wrapper<T1> variant_type_1;
	typedef value_wrapper<T2> variant_type_2;
#endif

public:
	using any_value::getValue;
	using any_value::extractValue;
	using any_value::isOfType;
	using any_value::getCurrTypeId;
	using any_value::k_invalidType;
	using any_value::operator bool;
	using any_value::clear;

	typedef typename value_wrapper<T1>::raw_type raw_T1;
	typedef typename std::add_lvalue_reference<raw_T1>::type T1_lreference;
	typedef typename std::add_lvalue_reference<typename std::add_const<raw_T1>::type>::type T1_const_lreference;
	typedef typename std::add_rvalue_reference<raw_T1>::type T1_rreference;
	typedef typename std::add_rvalue_reference<typename std::add_const<T1_rreference>::type>::type T1_const_rreference;
	typedef typename value_wrapper<T2>::raw_type raw_T2;
	typedef typename std::add_lvalue_reference<raw_T2>::type T2_lreference;
	typedef typename std::add_lvalue_reference<typename std::add_const<raw_T2>::type>::type T2_const_lreference;
	typedef typename std::add_rvalue_reference<raw_T2>::type T2_rreference;
	typedef typename std::add_rvalue_reference<typename std::add_const<T2_rreference>::type>::type T2_const_rreference;
	
	variant2()
	: any_value()
#if defined(WIN32) && defined(EWAS_DEBUG)
	, __debug_posType(k_invalidType)
#endif
	{
	}
	variant2(T1_const_lreference i_val)
	: any_value()
#if defined(WIN32) && defined(EWAS_DEBUG)
	, __debug_posType(k_invalidType)
#endif
	{
#if defined(WIN32) && defined(EWAS_DEBUG)
		__debug_posType = getTypePos<T1>();
#endif
		this->construct<T1>(std::forward<T1_const_lreference>(i_val), reinterpret_cast<null_value_wrapper<variant_type>*>(&m_embeddedArena));
	}
	variant2(T2_const_lreference i_val)
	: any_value()
#if defined(WIN32) && defined(EWAS_DEBUG)
	, __debug_posType(k_invalidType)
#endif
	{
#if defined(WIN32) && defined(EWAS_DEBUG)
		__debug_posType = getTypePos<T2>();
#endif
		this->construct<T2>(std::forward<T2_const_lreference>(i_val), reinterpret_cast<null_value_wrapper<variant_type>*>(&m_embeddedArena));
	}
	variant2(T1_rreference i_val)
	: any_value()
#if defined(WIN32) && defined(EWAS_DEBUG)
	, __debug_posType(k_invalidType)
#endif
	{
#if defined(WIN32) && defined(EWAS_DEBUG)
		__debug_posType = getTypePos<T1>();
#endif
		this->construct<T1>(std::move(i_val),reinterpret_cast<null_value_wrapper<variant_type>*>(&m_embeddedArena));
	}
	variant2(T2_rreference i_val)
	: any_value()
#if defined(WIN32) && defined(EWAS_DEBUG)
	, __debug_posType(k_invalidType)
#endif
	{
#if defined(WIN32) && defined(EWAS_DEBUG)
		__debug_posType = getTypePos<T2>();
#endif
		this->construct<T2>(std::move(i_val),reinterpret_cast<null_value_wrapper<variant_type>*>(&m_embeddedArena));
	}
	variant2(const variant2<T1,T2>& i_val)
	: any_value(reinterpret_cast<null_value_wrapper<variant_type>*>(&m_embeddedArena))
#if defined(WIN32) && defined(EWAS_DEBUG)
	, __debug_posType(k_invalidType)
#endif
	{
		if(i_val.isOfType<T1>())
		{
#if defined(WIN32) && defined(EWAS_DEBUG)
			__debug_posType = getTypePos<T1>();
#endif
			this->construct<T1>(i_val.getValue<T1>(),reinterpret_cast<null_value_wrapper<variant_type>*>(&m_embeddedArena));
		}
		else if(i_val.isOfType<T2>())
		{
#if defined(WIN32) && defined(EWAS_DEBUG)
			__debug_posType = getTypePos<T2>();
#endif
			this->construct<T2>(i_val.getValue<T2>(),reinterpret_cast<null_value_wrapper<variant_type>*>(&m_embeddedArena));
		}
	}
	variant2(variant2<T1,T2>&& i_val)
	: any_value(reinterpret_cast<null_value_wrapper<variant_type>*>(&m_embeddedArena))
#if defined(WIN32) && defined(EWAS_DEBUG)
	, __debug_posType(k_invalidType)
#endif
	{
		if(i_val.isOfType<T1>())
		{
#if defined(WIN32) && defined(EWAS_DEBUG)
			__debug_posType = getTypePos<T1>();
#endif
			this->construct<T1>(i_val.extractValue<T1>(),reinterpret_cast<null_value_wrapper<variant_type>*>(&m_embeddedArena));
		}
		else if(i_val.isOfType<T2>())
		{
#if defined(WIN32) && defined(EWAS_DEBUG)
			__debug_posType = getTypePos<T2>();
#endif
			this->construct<T2>(i_val.extractValue<T2>(),reinterpret_cast<null_value_wrapper<variant_type>*>(&m_embeddedArena));
		}
	}
	inline variant2& operator=(T1_const_lreference i_val)
	{
#if defined(WIN32) && defined(EWAS_DEBUG)
		__debug_posType = getTypePos<T1>();
#endif
		this->setValue<T1>(i_val);

		return *this;
	}
	inline variant2& operator=(T2_const_lreference i_val)
	{
#if defined(WIN32) && defined(EWAS_DEBUG)
		__debug_posType = getTypePos<T2>();
#endif
		this->setValue<T2>(i_val);

		return *this;
	}
	inline variant2& operator=(T1_rreference i_val)
	{
#if defined(WIN32) && defined(EWAS_DEBUG)
		__debug_posType = getTypePos<T1>();
#endif
		this->setValue<T1>(std::move(i_val));

		return *this;
	}
	inline variant2& operator=(T2_rreference i_val)
	{
#if defined(WIN32) && defined(EWAS_DEBUG)
		__debug_posType = getTypePos<T2>();
#endif
		this->setValue<T2>(std::move(i_val));

		return *this;
	}
	inline variant2& operator=(const variant2<T1,T2>& other)
	{
		if(other.isOfType<T1>())
		{
#if defined(WIN32) && defined(EWAS_DEBUG)
			__debug_posType = getTypePos<T1>();
#endif
			this->setValue<T1>(other.getValue<T1>());
		}
		else if(other.isOfType<T2>())
		{
#if defined(WIN32) && defined(EWAS_DEBUG)
			__debug_posType = getTypePos<T2>();
#endif
			this->setValue<T2>(other.getValue<T2>());
		}

		return *this;
	}
	inline variant2& operator=(variant2&& other)
	{
		if(other.isOfType<T1>())
		{
#if defined(WIN32) && defined(EWAS_DEBUG)
			__debug_posType = getTypePos<T1>();
#endif
			this->setValue<T1>(other.extractValue<T1>());
		}
		else if(other.isOfType<T2>())
		{
#if defined(WIN32) && defined(EWAS_DEBUG)
			__debug_posType = getTypePos<T2>();
#endif
			this->setValue<T2>(other.extractValue<T2>());
		}

		return *this;
	}
	template<typename T>
	static inline size_t getTypePos()
	{
		static const size_t s_typeHashCode = getTypeId<T>();

		if(s_typeHashCode == s_type1HashCode)
		{
			return 0;
		}
		else if(s_typeHashCode == s_type2HashCode)
		{
			return 1;
		}
		else
		{
			return k_invalidType;
		}
	}
	bool operator==(const variant2& other) const
	{
		if(this->isOfType<T1>() && other.isOfType<T1>())
		{
			return this->getValue<T1>() == other.getValue<T1>();
		}
		else if(this->isOfType<T2>() && other.isOfType<T2>())
		{
			return this->getValue<T2>() == other.getValue<T2>();
		}
		else
		{
			return false;
		}
	}
	bool operator!=(const variant2& other) const
	{
		if(this->isOfType<T1>() && other.isOfType<T1>())
		{
			return this->getValue<T1>() != other.getValue<T1>();
		}
		else if(this->isOfType<T2>() && other.isOfType<T2>())
		{
			return this->getValue<T2>() != other.getValue<T2>();
		}
		else
		{
			return true;
		}
	}
	bool empty() const
	{
		return this->getCurrTypeId() == k_invalidType;
	}
	virtual size_t getArenaSize() const override
	{
		return sizeof(variant_type);
	}
	template<typename Visitor>
	typename Visitor::return_type visit(const Visitor& i_visitor) const
	{
		if(this->isOfType<T1>())
		{
			return i_visitor.visit(this->getValue<T1>());
		}
		else if(this->isOfType<T2>())
		{
			return i_visitor.visit(this->getValue<T2>());
		}
		else
		{
			return ewas::crash_on_return<typename Visitor::return_type>::value();
		}
	}
	template<typename Visitor>
	typename Visitor::return_type visit(Visitor& i_visitor) const
	{
		if(this->isOfType<T1>())
		{
			return i_visitor.visit(this->getValue<T1>());
		}
		else if(this->isOfType<T2>())
		{
			return i_visitor.visit(this->getValue<T2>());
		}
		else
		{
			return ewas::crash_on_return<typename Visitor::return_type>::value();
		}
	}

private:
	typename std::aligned_storage<sizeof(variant_type), std::alignment_of<variant_type>::value>::type m_embeddedArena;
#if defined(WIN32) && defined(EWAS_DEBUG)
	size_t __debug_posType;
#endif
};

template<typename T1, typename T2>
const size_t variant2<T1, T2>::s_type1HashCode = getTypeId<T1>();
template<typename T1, typename T2>
const size_t variant2<T1, T2>::s_type2HashCode = getTypeId<T2>();

template<typename T1, typename T2, typename T3>
struct variant3 : protected any_value
{
	typedef value_wrapper<typename detail::max_type3<T1,T2,T3>::type> variant_type;
	static const size_t s_type1HashCode;
	static const size_t s_type2HashCode;
	static const size_t s_type3HashCode;
	static const size_t s_numTypes = 3;

#if defined(WIN32) && defined(EWAS_DEBUG)
	typedef value_wrapper<T1> variant_type_1;
	typedef value_wrapper<T2> variant_type_2;
	typedef value_wrapper<T3> variant_type_3;
#endif

public:
	using any_value::getValue;
	using any_value::extractValue;
	using any_value::isOfType;
	using any_value::getCurrTypeId;
	using any_value::k_invalidType;
	using any_value::operator bool;
	using any_value::clear;

	typedef typename value_wrapper<T1>::raw_type raw_T1;
	typedef typename std::add_lvalue_reference<raw_T1>::type T1_lreference;
	typedef typename std::add_lvalue_reference<typename std::add_const<raw_T1>::type>::type T1_const_lreference;
	typedef typename std::add_rvalue_reference<raw_T1>::type T1_rreference;
	typedef typename std::add_rvalue_reference<typename std::add_const<T1_rreference>::type>::type T1_const_rreference;
	typedef typename value_wrapper<T2>::raw_type raw_T2;
	typedef typename std::add_lvalue_reference<raw_T2>::type T2_lreference;
	typedef typename std::add_lvalue_reference<typename std::add_const<raw_T2>::type>::type T2_const_lreference;
	typedef typename std::add_rvalue_reference<raw_T2>::type T2_rreference;
	typedef typename std::add_rvalue_reference<typename std::add_const<T2_rreference>::type>::type T2_const_rreference;
	typedef typename value_wrapper<T3>::raw_type raw_T3;
	typedef typename std::add_lvalue_reference<raw_T3>::type T3_lreference;
	typedef typename std::add_lvalue_reference<typename std::add_const<raw_T3>::type>::type T3_const_lreference;
	typedef typename std::add_rvalue_reference<raw_T3>::type T3_rreference;
	typedef typename std::add_rvalue_reference<typename std::add_const<T3_rreference>::type>::type T3_const_rreference;

	variant3()
	: any_value(reinterpret_cast<null_value_wrapper<variant_type>*>(&m_embeddedArena))
#if defined(WIN32) && defined(EWAS_DEBUG)
	, __debug_posType(k_invalidType)
#endif
	{
	}
	variant3(T1_const_lreference i_val)
	: any_value()
#if defined(WIN32) && defined(EWAS_DEBUG)
	, __debug_posType(k_invalidType)
#endif
	{
#if defined(WIN32) && defined(EWAS_DEBUG)
		__debug_posType = getTypePos<T1>();
#endif
		this->construct<T1>(std::forward<T1_const_lreference>(i_val), reinterpret_cast<null_value_wrapper<variant_type>*>(&m_embeddedArena));
	}
	variant3(T2_const_lreference i_val)
	: any_value()
#if defined(WIN32) && defined(EWAS_DEBUG)
	, __debug_posType(k_invalidType)
#endif
	{
#if defined(WIN32) && defined(EWAS_DEBUG)
		__debug_posType = getTypePos<T2>();
#endif
		this->construct<T2>(std::forward<T2_const_lreference>(i_val), reinterpret_cast<null_value_wrapper<variant_type>*>(&m_embeddedArena));
	}
	variant3(T3_const_lreference i_val)
	: any_value()
#if defined(WIN32) && defined(EWAS_DEBUG)
	, __debug_posType(k_invalidType)
#endif
	{
#if defined(WIN32) && defined(EWAS_DEBUG)
		__debug_posType = getTypePos<T3>();
#endif
		this->construct<T3>(std::forward<T3_const_lreference>(i_val), reinterpret_cast<null_value_wrapper<variant_type>*>(&m_embeddedArena));
	}
	variant3(T1_rreference i_val)
	: any_value()
#if defined(WIN32) && defined(EWAS_DEBUG)
	, __debug_posType(k_invalidType)
#endif
	{
#if defined(WIN32) && defined(EWAS_DEBUG)
		__debug_posType = getTypePos<T1>();
#endif
		this->construct<T1>(std::move(i_val),reinterpret_cast<null_value_wrapper<variant_type>*>(&m_embeddedArena));
	}
	variant3(T2_rreference i_val)
	: any_value()
#if defined(WIN32) && defined(EWAS_DEBUG)
	, __debug_posType(k_invalidType)
#endif
	{
#if defined(WIN32) && defined(EWAS_DEBUG)
		__debug_posType = getTypePos<T2>();
#endif
		this->construct<T2>(std::move(i_val),reinterpret_cast<null_value_wrapper<variant_type>*>(&m_embeddedArena));
	}
	variant3(T3_rreference i_val)
	: any_value()
#if defined(WIN32) && defined(EWAS_DEBUG)
	, __debug_posType(k_invalidType)
#endif
	{
#if defined(WIN32) && defined(EWAS_DEBUG)
		__debug_posType = getTypePos<T3>();
#endif
		this->construct<T3>(std::move(i_val),reinterpret_cast<null_value_wrapper<variant_type>*>(&m_embeddedArena));
	}
	variant3(const variant3<T1,T2,T3>& i_val)
	: any_value(reinterpret_cast<null_value_wrapper<variant_type>*>(&m_embeddedArena))
#if defined(WIN32) && defined(EWAS_DEBUG)
	, __debug_posType(k_invalidType)
#endif
	{
		if(i_val.isOfType<T1>())
		{
#if defined(WIN32) && defined(EWAS_DEBUG)
			__debug_posType = getTypePos<T1>();
#endif
			this->construct<T1>(i_val.getValue<T1>(),reinterpret_cast<null_value_wrapper<variant_type>*>(&m_embeddedArena));
		}
		else if(i_val.isOfType<T2>())
		{
#if defined(WIN32) && defined(EWAS_DEBUG)
			__debug_posType = getTypePos<T2>();
#endif
			this->construct<T2>(i_val.getValue<T2>(),reinterpret_cast<null_value_wrapper<variant_type>*>(&m_embeddedArena));
		}
		else if(i_val.isOfType<T3>())
		{
#if defined(WIN32) && defined(EWAS_DEBUG)
			__debug_posType = getTypePos<T3>();
#endif
			this->construct<T3>(i_val.getValue<T3>(),reinterpret_cast<null_value_wrapper<variant_type>*>(&m_embeddedArena));
		}
	}
	variant3(variant3<T1,T2,T3>&& i_val)
	: any_value(reinterpret_cast<null_value_wrapper<variant_type>*>(&m_embeddedArena))
#if defined(WIN32) && defined(EWAS_DEBUG)
	, __debug_posType(k_invalidType)
#endif
	{
		if(i_val.isOfType<T1>())
		{
#if defined(WIN32) && defined(EWAS_DEBUG)
			__debug_posType = getTypePos<T1>();
#endif
			this->construct<T1>(i_val.extractValue<T1>(),reinterpret_cast<null_value_wrapper<variant_type>*>(&m_embeddedArena));
		}
		else if(i_val.isOfType<T2>())
		{
#if defined(WIN32) && defined(EWAS_DEBUG)
			__debug_posType = getTypePos<T2>();
#endif
			this->construct<T2>(i_val.extractValue<T2>(),reinterpret_cast<null_value_wrapper<variant_type>*>(&m_embeddedArena));
		}
		else if(i_val.isOfType<T3>())
		{
#if defined(WIN32) && defined(EWAS_DEBUG)
			__debug_posType = getTypePos<T3>();
#endif
			this->construct<T3>(i_val.extractValue<T3>(),reinterpret_cast<null_value_wrapper<variant_type>*>(&m_embeddedArena));
		}
	}
	inline variant3& operator=(T1_const_lreference i_val)
	{
#if defined(WIN32) && defined(EWAS_DEBUG)
		__debug_posType = getTypePos<T1>();
#endif
		this->setValue<T1>(i_val);

		return *this;
	}
	inline variant3& operator=(T2_const_lreference i_val)
	{
#if defined(WIN32) && defined(EWAS_DEBUG)
		__debug_posType = getTypePos<T2>();
#endif
		this->setValue<T2>(i_val);

		return *this;
	}
	inline variant3& operator=(T3_const_lreference i_val)
	{
#if defined(WIN32) && defined(EWAS_DEBUG)
		__debug_posType = getTypePos<T3>();
#endif
		this->setValue<T3>(i_val);

		return *this;
	}
	inline variant3& operator=(T1_rreference i_val)
	{
#if defined(WIN32) && defined(EWAS_DEBUG)
		__debug_posType = getTypePos<T1>();
#endif
		this->setValue<T1>(std::move(i_val));

		return *this;
	}
	inline variant3& operator=(T2_rreference i_val)
	{
#if defined(WIN32) && defined(EWAS_DEBUG)
		__debug_posType = getTypePos<T2>();
#endif
		this->setValue<T2>(std::move(i_val));

		return *this;
	}
	inline variant3& operator=(T3_rreference i_val)
	{
#if defined(WIN32) && defined(EWAS_DEBUG)
		__debug_posType = getTypePos<T3>();
#endif
		this->setValue<T3>(std::move(i_val));

		return *this;
	}
	inline variant3& operator=(const variant3& other)
	{
		if(other.isOfType<T1>())
		{
#if defined(WIN32) && defined(EWAS_DEBUG)
			__debug_posType = getTypePos<T1>();
#endif
			this->setValue<T1>(other.getValue<T1>());
		}
		else if(other.isOfType<T2>())
		{
#if defined(WIN32) && defined(EWAS_DEBUG)
			__debug_posType = getTypePos<T2>();
#endif
			this->setValue<T2>(other.getValue<T2>());
		}
		else if(other.isOfType<T3>())
		{
#if defined(WIN32) && defined(EWAS_DEBUG)
			__debug_posType = getTypePos<T3>();
#endif
			this->setValue<T3>(other.getValue<T3>());
		}

		return *this;
	}
	inline variant3& operator=(variant3&& other)
	{
		if(other.isOfType<T1>())
		{
#if defined(WIN32) && defined(EWAS_DEBUG)
			__debug_posType = getTypePos<T1>();
#endif
			this->setValue<T1>(other.extractValue<T1>());
		}
		else if(other.isOfType<T2>())
		{
#if defined(WIN32) && defined(EWAS_DEBUG)
			__debug_posType = getTypePos<T2>();
#endif
			this->setValue<T2>(other.extractValue<T2>());
		}
		else if(other.isOfType<T3>())
		{
#if defined(WIN32) && defined(EWAS_DEBUG)
			__debug_posType = getTypePos<T3>();
#endif
			this->setValue<T3>(other.extractValue<T3>());
		}

		return *this;
	}
	template<typename T>
	static inline size_t getTypePos()
	{
		static const size_t s_typeHashCode = getTypeId<T>();

		if(s_typeHashCode == s_type1HashCode)
		{
			return 0;
		}
		else if(s_typeHashCode == s_type2HashCode)
		{
			return 1;
		}
		else if(s_typeHashCode == s_type3HashCode)
		{
			return 2;
		}
		else
		{
			return k_invalidType;
		}
	}
	bool operator==(const variant3& other) const
	{
		if(this->isOfType<T1>() && other.isOfType<T1>())
		{
			return this->getValue<T1>() == other.getValue<T1>();
		}
		else if(this->isOfType<T2>() && other.isOfType<T2>())
		{
			return this->getValue<T2>() == other.getValue<T2>();
		}
		else if(this->isOfType<T3>() && other.isOfType<T3>())
		{
			return this->getValue<T3>() == other.getValue<T3>();
		}
		else
		{
			return false;
		}
	}
	bool operator!=(const variant3& other) const
	{
		if(this->isOfType<T1>() && other.isOfType<T1>())
		{
			return this->getValue<T1>() != other.getValue<T1>();
		}
		else if(this->isOfType<T2>() && other.isOfType<T2>())
		{
			return this->getValue<T2>() != other.getValue<T2>();
		}
		else if(this->isOfType<T3>() && other.isOfType<T3>())
		{
			return this->getValue<T3>() != other.getValue<T3>();
		}
		else
		{
			return true;
		}
	}
	bool empty() const
	{
		return this->getCurrTypeId() == k_invalidType;
	}
	virtual size_t getArenaSize() const override
	{
		return sizeof(variant_type);
	}
	template<typename Visitor>
	typename Visitor::return_type visit(const Visitor& i_visitor) const
	{
		if(this->isOfType<T1>())
		{
			return i_visitor.visit(this->getValue<T1>());
		}
		else if(this->isOfType<T2>())
		{
			return i_visitor.visit(this->getValue<T2>());
		}
		else if(this->isOfType<T3>())
		{
			return i_visitor.visit(this->getValue<T3>());
		}
		else
		{
			return ewas::crash_on_return<typename Visitor::return_type>::value();
		}
	}
	template<typename Visitor>
	typename Visitor::return_type visit(Visitor& i_visitor) const
	{
		if(this->isOfType<T1>())
		{
			return i_visitor.visit(this->getValue<T1>());
		}
		else if(this->isOfType<T2>())
		{
			return i_visitor.visit(this->getValue<T2>());
		}
		else if(this->isOfType<T3>())
		{
			return i_visitor.visit(this->getValue<T3>());
		}
		else
		{
			return ewas::crash_on_return<typename Visitor::return_type>::value();
		}
	}

private:
	typename std::aligned_storage<sizeof(variant_type), std::alignment_of<variant_type>::value>::type m_embeddedArena;
#if defined(WIN32) && defined(EWAS_DEBUG)
	size_t __debug_posType;
#endif
};

template<typename T1, typename T2, typename T3>
const size_t variant3<T1, T2, T3>::s_type1HashCode = getTypeId<T1>();
template<typename T1, typename T2, typename T3>
const size_t variant3<T1, T2, T3>::s_type2HashCode = getTypeId<T2>();
template<typename T1, typename T2, typename T3>
const size_t variant3<T1, T2, T3>::s_type3HashCode = getTypeId<T3>();

template<typename T1, typename T2, typename T3, typename T4>
struct variant4 : protected any_value
{
	typedef value_wrapper<typename detail::max_type4<T1,T2,T3,T4>::type> variant_type;
	static const size_t s_type1HashCode;
	static const size_t s_type2HashCode;
	static const size_t s_type3HashCode;
	static const size_t s_type4HashCode;
	static const size_t s_numTypes = 4;

#if defined(WIN32) && defined(EWAS_DEBUG)
	typedef value_wrapper<T1> variant_type_1;
	typedef value_wrapper<T2> variant_type_2;
	typedef value_wrapper<T3> variant_type_3;
	typedef value_wrapper<T4> variant_type_4;
#endif

public:
	using any_value::getValue;
	using any_value::extractValue;
	using any_value::isOfType;
	using any_value::getCurrTypeId;
	using any_value::k_invalidType;
	using any_value::operator bool;
	using any_value::clear;

	variant4()
	: any_value(reinterpret_cast<null_value_wrapper<variant_type>*>(&m_embeddedArena))
#if defined(WIN32) && defined(EWAS_DEBUG)
	, __debug_posType(k_invalidType)
#endif
	{
	}
	variant4(const T1& i_val)
	: any_value(i_val, reinterpret_cast<null_value_wrapper<variant_type>*>(&m_embeddedArena))
#if defined(WIN32) && defined(EWAS_DEBUG)
	, __debug_posType(k_invalidType)
#endif
	{
	}
	variant4(const T2& i_val)
	: any_value(i_val,reinterpret_cast<null_value_wrapper<variant_type>*>(&m_embeddedArena))
#if defined(WIN32) && defined(EWAS_DEBUG)
	, __debug_posType(k_invalidType)
#endif
	{
	}
	variant4(const T3& i_val)
	: any_value(i_val,reinterpret_cast<null_value_wrapper<variant_type>*>(&m_embeddedArena))
#if defined(WIN32) && defined(EWAS_DEBUG)
	, __debug_posType(k_invalidType)
#endif
	{
	}
	variant4(const T4& i_val)
	: any_value(i_val,reinterpret_cast<null_value_wrapper<variant_type>*>(&m_embeddedArena))
#if defined(WIN32) && defined(EWAS_DEBUG)
	, __debug_posType(k_invalidType)
#endif
	{
	}
	variant4(const variant4& i_val)
	: any_value(reinterpret_cast<null_value_wrapper<variant_type>*>(&m_embeddedArena))
#if defined(WIN32) && defined(EWAS_DEBUG)
	, __debug_posType(k_invalidType)
#endif
	{
		if(i_val.isOfType<T1>())
		{
#if defined(WIN32) && defined(EWAS_DEBUG)
			__debug_posType = getTypePos<T1>();
#endif
			this->setValue<T1>(i_val.getValue<T1>());
		}
		else if(i_val.isOfType<T2>())
		{
#if defined(WIN32) && defined(EWAS_DEBUG)
			__debug_posType = getTypePos<T2>();
#endif
			this->setValue<T2>(i_val.getValue<T2>());
		}
		else if(i_val.isOfType<T3>())
		{
#if defined(WIN32) && defined(EWAS_DEBUG)
			__debug_posType = getTypePos<T3>();
#endif
			this->setValue<T3>(i_val.getValue<T3>());
		}
		else if(i_val.isOfType<T4>())
		{
#if defined(WIN32) && defined(EWAS_DEBUG)
			__debug_posType = getTypePos<T4>();
#endif
			this->setValue<T4>(i_val.getValue<T4>());
		}
	}
	template<typename T>
	static inline size_t getTypePos()
	{
		static const size_t s_typeHashCode = getTypeId<T>();

		if(s_typeHashCode == s_type1HashCode)
		{
			return 0;
		}
		else if(s_typeHashCode == s_type2HashCode)
		{
			return 1;
		}
		else if(s_typeHashCode == s_type3HashCode)
		{
			return 2;
		}
		else if(s_typeHashCode == s_type4HashCode)
		{
			return 3;
		}
		else
		{
			return k_invalidType;
		}
	}
	inline variant4& operator=(const T1& i_val)
	{
#if defined(WIN32) && defined(EWAS_DEBUG)
		__debug_posType = getTypePos<T1>();
#endif
		any_value::operator=(variant4(i_val));

		return *this;
	}
	inline variant4& operator=(const T2& i_val)
	{
#if defined(WIN32) && defined(EWAS_DEBUG)
		__debug_posType = getTypePos<T2>();
#endif
		any_value::operator=(variant4(i_val));

		return *this;
	}
	inline variant4& operator=(const T3& i_val)
	{
#if defined(WIN32) && defined(EWAS_DEBUG)
		__debug_posType = getTypePos<T3>();
#endif
		any_value::operator=(variant4(i_val));

		return *this;
	}
	inline variant4& operator=(const T4& i_val)
	{
#if defined(WIN32) && defined(EWAS_DEBUG)
		__debug_posType = getTypePos<T4>();
#endif
		any_value::operator=(variant4(i_val));

		return *this;
	}
	inline variant4& operator=(const variant4& other)
	{
		if(other.isOfType<T1>())
		{
#if defined(WIN32) && defined(EWAS_DEBUG)
			__debug_posType = getTypePos<T1>();
#endif
			this->setValue<T1>(other.getValue<T1>());
		}
		else if(other.isOfType<T2>())
		{
#if defined(WIN32) && defined(EWAS_DEBUG)
			__debug_posType = getTypePos<T2>();
#endif
			this->setValue<T2>(other.getValue<T2>());
		}
		else if(other.isOfType<T3>())
		{
#if defined(WIN32) && defined(EWAS_DEBUG)
			__debug_posType = getTypePos<T3>();
#endif
			this->setValue<T3>(other.getValue<T3>());
		}
		else if(other.isOfType<T4>())
		{
#if defined(WIN32) && defined(EWAS_DEBUG)
			__debug_posType = getTypePos<T4>();
#endif
			this->setValue<T4>(other.getValue<T4>());
		}

		return *this;
	}
	bool operator==(const variant4& other) const
	{
		if(this->isOfType<T1>() && other.isOfType<T1>())
		{
			return this->getValue<T1>() == other.getValue<T1>();
		}
		else if(this->isOfType<T2>() && other.isOfType<T2>())
		{
			return this->getValue<T2>() == other.getValue<T2>();
		}
		else if(this->isOfType<T3>() && other.isOfType<T3>())
		{
			return this->getValue<T3>() == other.getValue<T3>();
		}
		else if(this->isOfType<T4>() && other.isOfType<T4>())
		{
			return this->getValue<T4>() == other.getValue<T4>();
		}
		else
		{
			return false;
		}
	}
	bool operator!=(const variant4& other) const
	{
		if(this->isOfType<T1>() && other.isOfType<T1>())
		{
			return this->getValue<T1>() != other.getValue<T1>();
		}
		else if(this->isOfType<T2>() && other.isOfType<T2>())
		{
			return this->getValue<T2>() != other.getValue<T2>();
		}
		else if(this->isOfType<T3>() && other.isOfType<T3>())
		{
			return this->getValue<T3>() != other.getValue<T3>();
		}
		else if(this->isOfType<T4>() && other.isOfType<T4>())
		{
			return this->getValue<T4>() != other.getValue<T4>();
		}
		else
		{
			return true;
		}
	}
	bool empty() const
	{
		return this->getTypePos() == k_invalidType;
	}
	virtual size_t getArenaSize() const override
	{
		return sizeof(variant_type);
	}
	template<typename Visitor>
	typename Visitor::return_type visit(const Visitor& i_visitor) const
	{
		if(this->isOfType<T1>())
		{
			return i_visitor.visit(this->getValue<T1>());
		}
		else if(this->isOfType<T2>())
		{
			return i_visitor.visit(this->getValue<T2>());
		}
		else if(this->isOfType<T3>())
		{
			return i_visitor.visit(this->getValue<T3>());
		}
		else if(this->isOfType<T4>())
		{
			return i_visitor.visit(this->getValue<T4>());
		}
		else
		{
			return ewas::crash_on_return<typename Visitor::return_type>::value();
		}
	}
	template<typename Visitor>
	typename Visitor::return_type visit(Visitor& i_visitor) const
	{
		if(this->isOfType<T1>())
		{
			return i_visitor.visit(this->getValue<T1>());
		}
		else if(this->isOfType<T2>())
		{
			return i_visitor.visit(this->getValue<T2>());
		}
		else if(this->isOfType<T3>())
		{
			return i_visitor.visit(this->getValue<T3>());
		}
		else if(this->isOfType<T4>())
		{
			return i_visitor.visit(this->getValue<T4>());
		}
		else
		{
			return ewas::crash_on_return<typename Visitor::return_type>::value();
		}
	}

private:
	typename std::aligned_storage<sizeof(variant_type), std::alignment_of<variant_type>::value>::type m_embeddedArena;
#if defined(WIN32) && defined(EWAS_DEBUG)
	size_t __debug_posType;
#endif
};

template<typename T1, typename T2, typename T3, typename T4>
const size_t variant4<T1, T2, T3, T4>::s_type1HashCode = getTypeId<T1>();
template<typename T1, typename T2, typename T3, typename T4>
const size_t variant4<T1, T2, T3, T4>::s_type2HashCode = getTypeId<T2>();
template<typename T1, typename T2, typename T3, typename T4>
const size_t variant4<T1, T2, T3, T4>::s_type3HashCode = getTypeId<T3>();
template<typename T1, typename T2, typename T3, typename T4>
const size_t variant4<T1, T2, T3, T4>::s_type4HashCode = getTypeId<T4>();

}