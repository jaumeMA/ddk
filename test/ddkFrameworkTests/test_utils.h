#pragma once

#include "ddk_reference_wrapper_deleter.h"
#include <utility>
#include <stddef.h>

template<typename T>
class TestDynamicFactory : public ddk::IReferenceWrapperDeleter
{
public:
	template<typename ... Args>
	T* Allocate(Args&& ... i_args)
	{
		return new T(std::forward<Args>(i_args) ...);
	}
	void Deallocate(const void* i_object) const override
	{
		if(const T* typedObj = reinterpret_cast<const T*>(i_object))
		{
			delete typedObj;
		}
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
