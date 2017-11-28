#if (defined(__GNUC__) || defined(__clang__) || defined(__llvm__)) && (defined(__linux__) || defined(__linux) || defined(__APPLE__))
#include <unistd.h>
#include <cstdlib>
#include <iostream>
#endif

#include "exception.hpp"
#include "Tools/system_functions.hpp"

#define ENABLE_BACK_TRACE

using namespace aff3ct::tools;

exception
::exception() throw()
{
}

exception
::exception(const std::string message) throw()
: message(message)
{
#if defined(ENABLE_BACK_TRACE)
	this->message += get_back_trace(3);
#endif
}

exception
::exception(const std::string filename,
            const int line_num,
            const std::string funcname,
            const std::string message) throw()
{
	if (!filename.empty())
		this->message += "In the '" + filename + "' file";
	if (line_num >= 0)
		this->message += " at line " + std::to_string(line_num);
	if (!funcname.empty())
		this->message += " ('" + funcname + "' function)";
	this->message += ": ";
	this->message += "\"" + message + "\"";

#if defined(ENABLE_BACK_TRACE)
	this->message += get_back_trace(3);
#endif
}

exception
::~exception() throw()
{
}

const char* exception
::what() const throw()
{
	return message.c_str();
}