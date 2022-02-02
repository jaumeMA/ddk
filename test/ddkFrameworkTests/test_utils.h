#pragma once

#include "ddk_resource_deleter.h"
#include "ddk_rtti.h"
#include <utility>
#include <stddef.h>

template<typename T>
class TestDynamicDeleter
{
public:
	void deallocate(T* i_object) const
	{
		delete i_object;
	}
};

template<typename T>
class TestDynamicFactory : public TestDynamicDeleter<T>
{
public:
	typedef TestDynamicFactory allocator;
	typedef T type;
	typedef T* pointer;
	typedef const T* const_pointer;
	typedef std::ptrdiff_t difference_type;

	template<typename ... Args>
	T* allocate(Args&& ... i_args) const
	{
		return new T(std::forward<Args>(i_args) ...);
	}
	void* allocate(size_t i_size) const
	{
		return malloc(i_size);
	}
};

class DefaultType
{
public:
	DefaultType(int i_value = 0);
	virtual ~DefaultType() = default;
	bool operator==(int i_value) const;
	void setValue(int i_value);
	int getValue() const;
	bool operator==(const DefaultType& other) const;
	bool operator!=(const DefaultType& other) const;
	bool operator<(const DefaultType& other) const;
	bool operator>(const DefaultType& other) const;

private:
	int m_value;
};
class DerivedDefaultType : public DefaultType
{
public:
	using DefaultType::DefaultType;
	using DefaultType::operator==;
	using DefaultType::operator!=;
};
class nonMoveConstructibleType : public DefaultType
{
public:
	using DefaultType::DefaultType;
	using DefaultType::operator==;
	using DefaultType::operator!=;

	nonMoveConstructibleType() = default;
	nonMoveConstructibleType(const nonMoveConstructibleType&) = default;
	nonMoveConstructibleType(nonMoveConstructibleType&&) = delete;
	~nonMoveConstructibleType() = default;
	nonMoveConstructibleType& operator=(const nonMoveConstructibleType&) = default;
	nonMoveConstructibleType& operator=(nonMoveConstructibleType&&) = default;
};
class nonCopyConstructibleType : public DefaultType
{
public:
	using DefaultType::DefaultType;
	nonCopyConstructibleType() = default;
	nonCopyConstructibleType(const nonCopyConstructibleType&) = delete;
	nonCopyConstructibleType(nonCopyConstructibleType&&) = default;
	~nonCopyConstructibleType() = default;
	nonCopyConstructibleType& operator=(const nonCopyConstructibleType&) = default;
	nonCopyConstructibleType& operator=(nonCopyConstructibleType&&) = default;
};
class nonMoveAssignableType : public DefaultType
{
public:
	using DefaultType::DefaultType;
	nonMoveAssignableType() = default;
	nonMoveAssignableType(const nonMoveAssignableType&) = default;
	nonMoveAssignableType(nonMoveAssignableType&&) = default;
	~nonMoveAssignableType() = default;

	nonMoveAssignableType& operator=(const nonMoveAssignableType&) = default;
	nonMoveAssignableType& operator=(nonMoveAssignableType&&) = delete;
};
class nonCopyAssignableType : public DefaultType
{
public:
	using DefaultType::DefaultType;
	nonCopyAssignableType() = default;
	nonCopyAssignableType(const nonCopyAssignableType&) = default;
	nonCopyAssignableType(nonCopyAssignableType&&) = default;
	~nonCopyAssignableType() = default;

	nonCopyAssignableType& operator=(const nonCopyAssignableType&) = delete;
	nonCopyAssignableType& operator=(nonCopyAssignableType&&) = default;
};

class ConstructionDeletionBalancer : public DefaultType
{
public:

	ConstructionDeletionBalancer(int i_value);
	ConstructionDeletionBalancer(const ConstructionDeletionBalancer& other);
	ConstructionDeletionBalancer(ConstructionDeletionBalancer&& other);
	~ConstructionDeletionBalancer();
	static bool isBalanced();

private:
	static size_t s_constructionDeletionBalance;
};
