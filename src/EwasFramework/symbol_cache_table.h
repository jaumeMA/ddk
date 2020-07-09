#pragma once

#include <unordered_map>
#include <pthread.h>
#include <string>

#if defined(WIN32)
#pragma comment(lib, "dbghelp")
#endif // defined

namespace ewas
{
namespace detail
{

struct symbol_cache_table
{
public:
	struct symbol_info
	{
		std::string m_name;
		std::string m_filename;
		size_t m_lineNumber;
	};
	typedef symbol_info& reference;
	typedef const symbol_info& const_reference;

	symbol_cache_table();
	~symbol_cache_table();
	const_reference symbolicate(void* i_address);

private:
	typedef std::unordered_map<size_t,symbol_info>::iterator iterator;
	typedef std::unordered_map<size_t,symbol_info>::const_iterator const_iterator;

	std::unordered_map<size_t,symbol_info> m_symbolInfo;
	mutable pthread_mutex_t m_refMutex;
};

}
}
