#pragma once

namespace ddk
{
namespace detail
{

struct yielder_context;
class yielder_interface;

struct yielder
{
public:
	explicit yielder(yielder_interface& i_executor);
	void yield(yielder_context* i_context);
	void suspend(yielder_context* i_context);

private:
	yielder_interface& m_executor;
};

}
}
