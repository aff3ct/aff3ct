#if (defined(__GNUC__) || defined(__clang__) || defined(__llvm__)) && (defined(__linux__) || defined(__linux) || defined(__APPLE__))
#include <execinfo.h> // backtrace, backtrace_symbols
#include <cxxabi.h>   // __cxa_demangle
#include <unistd.h>
#include <cstdlib>
#include <iostream>
#endif

#include "exception.hpp"

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
	this->message += get_back_trace();
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
	this->message += get_back_trace();
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
std::string exception
::get_back_trace()
{
	std::string bt_str;
#if (defined(__GNUC__) || defined(__clang__) || defined(__llvm__)) && (defined(__linux__) || defined(__linux) || defined(__APPLE__))
	const int bt_max_depth = 32;
	void *bt_array[bt_max_depth];

	size_t size = backtrace(bt_array, bt_max_depth);
	char** bt_symbs = backtrace_symbols(bt_array, size);

	bt_str += "\nBacktrace:";
	for (size_t i = 3; i < size; i++)
	{
		std::string symbol = bt_symbs[i];

		auto pos_beg_func = symbol.find('(');
		auto pos_off      = symbol.find('+');
		auto pos_end_func = symbol.find(')');
		auto pos_beg_addr = symbol.find('[');
		auto pos_end_addr = symbol.find(']');

		if (   (pos_beg_func != std::string::npos)
		    && (pos_off      != std::string::npos)
		    && (pos_end_func != std::string::npos)
		    && (pos_beg_addr != std::string::npos)
		    && (pos_end_addr != std::string::npos))
		{
			auto program  = symbol.substr(0,               pos_beg_func                  );
			auto function = symbol.substr(pos_beg_func +1, pos_off      - pos_beg_func -1);
			auto offset   = symbol.substr(pos_off,         pos_end_func - pos_off        );
			auto address  = symbol.substr(pos_beg_addr,    pos_end_addr - pos_beg_addr +1);

			std::string function_name;

			int status;
			auto demangled_name = abi::__cxa_demangle(function.data(), NULL, NULL, &status);

			if (demangled_name != NULL)
			{
				function_name = demangled_name;
				free(demangled_name);
			}
			else
			{
				function_name = function + "()";
			}

			if (   status ==  0  // good
			    || status == -2) // mangled_name is not a valid name under the C++ ABI mangling rules.
			{
				bt_str += "\n" + program + ": " + function_name + " " + offset + " " + address;
			}
			else // error
			{
				if (status == -3) // One of the arguments is invalid.
				{
					std::cerr << "Invalid abi::__cxa_demangle argument(s)." << std::endl;
					std::exit(EXIT_FAILURE);
				}

				if (status == -1) // A memory allocation failiure occurred.
				{
					std::cerr << "Memory allocation error in abi::__cxa_demangle." << std::endl;
					std::exit(EXIT_FAILURE);
				}
			}
		}
	}
	free(bt_symbs);
#endif

	return bt_str;
}