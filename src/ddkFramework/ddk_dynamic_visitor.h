#pragma once

#include "ddk_rtti_defs.h"
#include <unordered_map>

namespace ddk
{

template<typename Interface>
class dynamic_visitor
{
    template<typename FinalVisitorType, typename VisitorType>
    friend bool __expand_type_visitor_layout();

public:
	struct callable_tag;
	typedef Interface type_interface;
	static const size_t nvisitor = -1;

	virtual ~dynamic_visitor() = default;
	template<typename T>
	void operator()(T&&) const
	{
		static_assert(sizeof(T)==0, "You have to provide a visit method for this type");
	}
	template<typename T>
	void operator()(T&&)
	{
		static_assert(sizeof(T)==0, "You have to provide a visit method for this type");
	}
	static size_t get_dynamic_visitor(const TypeInfo& i_typeInfo)
	{
		std::unordered_map<size_t,size_t>& registeredDynamicVisitors = __get_dynamic_visitor_layout();

		std::unordered_map<size_t,size_t>::const_iterator itVisitor = registeredDynamicVisitors.find(i_typeInfo.get_id());

		return (itVisitor != registeredDynamicVisitors.end()) ? itVisitor->second : nvisitor;
	}

private:
	static void add_dynamic_visitor(const TypeInfo& i_typeInfo, size_t i_layout)
	{
		std::unordered_map<size_t,size_t>& registeredDynamicVisitors = __get_dynamic_visitor_layout();

		registeredDynamicVisitors.insert(std::make_pair(i_typeInfo.get_id(),i_layout));
	}
	static inline std::unordered_map<size_t,size_t>& __get_dynamic_visitor_layout()
	{
		static std::unordered_map<size_t,size_t> s_dynamicVisitorLayout;

		return s_dynamicVisitorLayout;
	}
};

template<typename Type>
class typed_dynamic_visitor
{
public:
	virtual void visit(const Type&) const = 0;
	virtual void visit(const Type&) = 0;
};

}
