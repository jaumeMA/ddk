#include "ddk_assert.h"

#if defined(EWAS_DEBUG)
#if defined(WIN32)
    #include <windows.h>
#elif defined(__LINUX__)
    #include <X11/Xlib.h>
#endif
#endif

#include <csignal>
#include <iostream>
#include <sstream>

namespace ddk
{
namespace detail
{

void signal_handler(int signal)
{
#if defined(EWAS_DEBUG)
#if defined(WIN32)
    if (signal == SIGABRT)
	{
    }
#elif defined(__LINUX__) || defined(__APPLE__)
    if(signal == SIGTRAP)
    {
    }
#endif
	else
	{
        std::cerr << "Unexpected signal " << signal << " received\n";
    }
#endif
}

Assert::Assert(const char* i_file, size_t i_fileLine, const std::string& i_title, const std::string& i_message)
: m_file(i_file)
, m_line(i_fileLine)
, m_title(i_title)
, m_message(i_message)
{
	std::stringstream stream;

	stream << m_message << "Assertion: fail in file " << m_file << ", line " << m_line;

	m_output = stream.str();
}

Assert::Assert(const char* i_expression, const char* i_file, size_t i_fileLine, const std::string& i_title, const std::string& i_message)
: m_expression(i_expression)
, m_file(i_file)
, m_line(i_fileLine)
, m_title(i_title)
, m_message(i_message)
{
	std::stringstream stream;

	stream << m_message << "Assertion: " << m_expression << "failed in file " << m_file << ", line " << m_line;

	m_output = stream.str();
}
std::string Assert::what() const
{
	return m_output.c_str();
}
std::string Assert::getExpression() const
{
	return m_expression;
}
std::string Assert::getFile() const
{
	return m_file;
}
size_t Assert::getLine() const
{
	return m_line;
}
std::string Assert::getMessage()
{
	return m_message;
}
Assert::Response Assert::raise() const
{
#if defined(EWAS_DEBUG)
#if defined(WIN32)
	switch (MessageBox(NULL, m_output.c_str(), "Assert", MB_ICONWARNING | MB_RETRYCANCEL | MB_SYSTEMMODAL | MB_DEFBUTTON2))
	{
		case IDRETRY:
		{
			return Break;
		}
		case IDCANCEL:
		default:
		{
			return Continue;
		}
	}
#elif defined(__LINUX__)

    return Break;

    Display *d;
    Window w;
    XEvent e;
    const char *msg = "Hello, World!";
    int s;

    d = XOpenDisplay(NULL);
    if (d == NULL)
    {
        fprintf(stderr, "Cannot open display\n");
        exit(1);
    }

    s = DefaultScreen(d);
    w = XCreateSimpleWindow(d, RootWindow(d, s), 10, 10, 400, 150, 1,
                        BlackPixel(d, s), WhitePixel(d, s));
    XSelectInput(d, w, ExposureMask | KeyPressMask);
    XMapWindow(d, w);
    XStoreName(d,w,"Assert");

    while (true)
    {
        XNextEvent(d, &e);
        if (e.type == Expose)
        {
            const std::string contents = m_output + ". (d)ebug, (c)ontinue";
            XFillRectangle(d, w, DefaultGC(d, s), 20, 20, 10, 10);
            XDrawString(d, w, DefaultGC(d, s), 10, 200, contents.c_str(), contents.size());
        }
        else if (e.type == KeyPress)
        {
            ::raise(SIGTRAP);
//            if(e.xkey.keycode == 0x40)
//            {
//                break;
//            }
//            else if(e.xkey.keycode == 0x54)
//            {
//                break;
//            }
        }
   }

    XCloseDisplay(d);

#endif // defined
#endif

	return Continue;
}

}
}
