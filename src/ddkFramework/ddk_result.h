#pragma once

#include "ddk_variant.h"
#include "ddk_optional.h"
#include "ddk_macros.h"
#include "ddk_formatter.h"

namespace ddk
{

struct result_success_t{};
const result_success_t success = result_success_t();

template<typename,typename>
class result;

template<typename Error>
class result<void,Error>
{
	friend std::ostringstream& operator<<(std::ostringstream& o_stream, const result<void,Error>& i_result)
	{
		if (i_result.hasError())
		{
			o_stream << "Error: " << i_result.getError();
		}
		else
		{
			o_stream << "Success";
		}

		return o_stream;
	}
	friend std::string operator<<(const std::string& i_str, const result<void,Error>& i_result)
	{
		std::string res;

		if (i_result.hasError())
		{
			res = "Error: " << i_result.getError();
		}
		else
		{
			res = "Success";
		}

		return res;
	}
	result<void,Error>& operator=(const result<void,Error>&);
	
public:
	typedef void payload_t;
	typedef Error error_t;

	result(const result_success_t&)
	: m_nestedRes()
#if defined(EWAS_DEBUG)
	, m_checked(false)
#endif
	{
	}
	result(const Error& i_error)
	: m_nestedRes(i_error)
#if defined(EWAS_DEBUG)
	, m_checked(false)
#endif
	{
	}
	result(result&& other)
	: m_nestedRes(std::move(other.m_nestedRes))
#if defined(EWAS_DEBUG)
	, m_checked(other.m_checked)
#endif
	{
#if defined(EWAS_DEBUG)
		other.m_checked = true;
#endif
	}
	~result()
	{
#if defined(EWAS_DEBUG)
		EWAS_ASSERT(m_checked==true, "Unchecked result on destroy!");
#endif
	}
	bool hasError() const
	{
#if defined(EWAS_DEBUG)
	m_checked = true;
#endif
		return m_nestedRes.empty() == false;
	}
	Error getError() const
	{
#if defined(EWAS_DEBUG)
	m_checked = true;
#endif
		return *m_nestedRes;
	}
	void clear()
	{
		m_nestedRes.clear();
	}

private:
	optional<Error> m_nestedRes;	
#if defined(EWAS_DEBUG)
	mutable bool m_checked;
#endif
};

template<typename Payload, typename Error>
class result
{
	friend std::ostringstream& operator<<(std::ostringstream& o_stream, const result<Payload,Error>& i_result)
	{
		if (i_result.hasError())
		{
			o_stream << "Error: " << i_result.getError();
		}
		else
		{
			o_stream << "Success: " << i_result.getPayload();
		}

		return o_stream;
	}
	friend std::string operator<<(const std::string& i_str, const result<Payload,Error>& i_result)
	{
		std::ostringstream res;

		if (i_result.hasError())
		{
			res << "Error: " << i_result.getError();
		}
		else
		{
			res << "Success " << i_result.getPayload();
		}

		return res.str();
	}

	result<Payload, Error>& operator=(const result<Payload,Error>&);

public:
	typedef Payload payload_t;
	typedef Error error_t;

	result(const Payload& i_payload)
	: m_nestedRes(i_payload)
#if defined(EWAS_DEBUG)
	, m_checked(false)
#endif
	{
	}
	result(Payload&& i_payload)
	: m_nestedRes(std::move(i_payload))
#if defined(EWAS_DEBUG)
	, m_checked(false)
#endif
	{
	}
	result(const Error& i_error)
	: m_nestedRes(i_error)
#if defined(EWAS_DEBUG)
	, m_checked(false)
#endif
	{
	}
	result(result&& other)
	: m_nestedRes(std::move(other.m_nestedRes))
#if defined(EWAS_DEBUG)
	, m_checked(other.m_checked)
#endif
	{
#if defined(EWAS_DEBUG)
		other.m_checked = true;
#endif
	}
	~result()
	{
#if defined(EWAS_DEBUG)
		EWAS_ASSERT(m_checked==true, "Unchecked result on destroy!");
#endif
	}
	bool hasError() const
	{
#if defined(EWAS_DEBUG)
	m_checked = true;
#endif
		return m_nestedRes.template is<Error>();
	}
	Error getError() const
	{
#if defined(EWAS_DEBUG)
	m_checked = true;
#endif
		return m_nestedRes.template get<Error>();
	}
	Payload getPayload() const
	{
		return m_nestedRes.template get<Payload>();
	}
	Payload extractPayload()
	{
		return m_nestedRes.template extract<Payload>();
	}
	void clear()
	{
		m_nestedRes.clear();
	}

private:
	variant<Payload,Error> m_nestedRes;	
#if defined(EWAS_DEBUG)
	mutable bool m_checked;
#endif
};


template<typename Result, typename ... Args>
inline Result make_error(Args&& ... i_args)
{
	return  typename Result::error_t(std::forward<Args>(i_args) ...);
}
template<typename Result>
inline Result make_result()
{
	return  Result(success);
}
template<typename Result, typename ... Args>
inline Result make_result(Args&& ... i_args)
{
	return  typename Result::payload_t(std::forward<Args>(i_args) ...);
}

}
