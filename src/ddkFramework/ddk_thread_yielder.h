#pragma once

#include "ddk_arena.h"
#include "ddk_lent_reference_wrapper.h"
#include "ddk_lend_from_this.h"
#include "ddk_embedded_type.h"

namespace ddk
{
namespace detail
{

struct yielder_context
{
	virtual ~yielder_context() = default;
};

template<typename T>
struct typed_yielder_context : yielder_context
{
public:
	typedef typename embedded_type<T>::ref_type reference;
	typedef typename embedded_type<T>::cref_type const_reference;
	typedef typename embedded_type<T>::raw_type value_type;

	typed_yielder_context() = default;
    template<typename TT>
	typed_yielder_context(TT&& i_value);
	typed_yielder_context(const typed_yielder_context& other);
	typed_yielder_context(typed_yielder_context&& other);
	~typed_yielder_context();

	typed_yielder_context& operator=(const typed_yielder_context& other);
	typed_yielder_context& operator=(typed_yielder_context&& other);
	void insert_value(T i_value);
	const_reference get_value() const;
	reference get_value();

private:
	mutable typed_arena<T> m_value;
};

struct yielder : public lend_from_this<yielder,yielder>
{
	virtual ~yielder() = default;

	virtual void yield(yielder_context* = nullptr) = 0;
	virtual void suspend(yielder_context* = nullptr) = 0;
};

typedef lent_reference_wrapper<yielder> yielder_lent_ref;
typedef lent_reference_wrapper<const yielder> yielder_const_lent_ref;
typedef lent_pointer_wrapper<yielder> yielder_lent_ptr;
typedef lent_pointer_wrapper<const yielder> yielder_const_lent_ptr;

}
}

#include "ddk_thread_yielder.inl"
