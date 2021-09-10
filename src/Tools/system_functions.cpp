#if (defined(__GNUC__) || defined(__clang__) || defined(__llvm__)) && (defined(__linux__) || defined(__linux) || defined(__APPLE__)) && !defined(__ANDROID__)
#include <execinfo.h> // backtrace, backtrace_symbols
#include <cxxabi.h>   // __cxa_demangle
#endif

#if defined(__linux__) || defined(linux) || defined(__linux) || defined(__FreeBSD__)
#include <unistd.h>
#include <string.h>
#include <errno.h>
#elif defined(__APPLE__) || defined(__MACH__)
#include <mach-o/dyld.h>
#elif defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#endif

#include <stdexcept>
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <vector>

#include "Tools/Exception/exception.hpp"
#include "Tools/system_functions.h"

std::string aff3ct::tools::get_back_trace(int first_call)
{
	std::string bt_str;

#if (defined(__GNUC__) || defined(__clang__) || defined(__llvm__)) && (defined(__linux__) || defined(__linux) || defined(__APPLE__)) && !defined(__ANDROID__)
	const int bt_max_depth = 32;
	void *bt_array[bt_max_depth];

	size_t size = backtrace(bt_array, bt_max_depth);
	char** bt_symbs = backtrace_symbols(bt_array, size);


	bt_str += "Backtrace:";
	for (size_t i = first_call; i < size; i++)
	{
		std::string symbol = bt_symbs[i];

		auto pos_beg_func = symbol.find('('              );
		auto pos_off      = symbol.find('+', pos_beg_func);
		auto pos_end_func = symbol.find(')', pos_off     );
		auto pos_beg_addr = symbol.find('[', pos_end_func);
		auto pos_end_addr = symbol.find(']', pos_beg_addr);

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

			bt_str += "\n" + program + ": " + function_name + " " + offset + " " + address;

			if (   status ==  0  // good
			    || status == -2) // mangled_name is not a valid name under the C++ ABI mangling rules.
			{} // does nothing more
			else if (status == -3) // One of the arguments is invalid.
				bt_str += " !! Error: Invalid abi::__cxa_demangle argument(s) !!";
			else if (status == -1) // A memory allocation failiure occurred.
				bt_str += " !! Error: Memory allocation error in abi::__cxa_demangle !!";
		}
	}
	free(bt_symbs);
#endif

	return bt_str;
}

std::string aff3ct::tools::run_system_command(std::string cmd)
{
	std::string data;

	cmd.append(" 2>&1");

#if (defined(__GNUC__) || defined(__clang__) || defined(__llvm__)) && (defined(__linux__) || defined(__linux) || defined(__APPLE__)) && !defined(__ANDROID__)
	FILE *stream = popen(cmd.c_str(), "r");
#elif defined(_WIN64) || defined(_WIN32)
	FILE *stream = _popen(cmd.c_str(), "r");
#else
	FILE *stream = nullptr;
#endif

	if (stream)
	{
		const int max_buffer = 256;
		char buffer[max_buffer];
		while (!feof(stream))
			if (fgets(buffer, max_buffer, stream) != NULL)
				data.append(buffer);

#if (defined(__GNUC__) || defined(__clang__) || defined(__llvm__)) && (defined(__linux__) || defined(__linux) || defined(__APPLE__)) && !defined(__ANDROID__)
		pclose(stream);
#elif defined(_WIN64) || defined(_WIN32)
		_pclose(stream);
#endif
	}
	else
		throw std::runtime_error("run_system_command error: Couldn't open the pipe to run system command.");

	return data;
}

std::string aff3ct::tools::addr_to_line(const std::string& backtrace)
{
#ifdef NDEBUG
	return backtrace;
#elif (defined(__GNUC__) || defined(__clang__) || defined(__llvm__)) && (defined(__linux__) || defined(__linux) || defined(__APPLE__)) && !defined(__ANDROID__)
	// TODO Bug: lines does not always match with the real line where are called the functions.

	std::string bt_str;
	// first separate the backtrace to find back the stack
	std::vector<std::string> stack;
	size_t pos = 0;

	while (true)
	{
		size_t found_pos = backtrace.find('\n', pos);

		if (found_pos == std::string::npos)
		{
			stack.push_back(backtrace.substr(pos));
			break;
		}
		else
			stack.push_back(backtrace.substr(pos, found_pos -pos));

		pos = found_pos +1;
	}

	// parse each line to transform them and fill bt_str
	for (unsigned i = 0; i < stack.size(); ++i)
	{
		auto pos_beg_func = stack[i].find(':'              );
		auto pos_off      = stack[i].find('+', pos_beg_func);
		auto pos_beg_addr = stack[i].find('[', pos_off);
		auto pos_end_addr = stack[i].find(']', pos_beg_addr);

		if (   (pos_beg_func != std::string::npos)
		    && (pos_off      != std::string::npos)
		    && (pos_beg_addr != std::string::npos)
		    && (pos_end_addr != std::string::npos))
		{
			auto program  = stack[i].substr(0,               pos_beg_func                  );
			auto function = stack[i].substr(pos_beg_func,    pos_off      - pos_beg_func   );
			auto address  = stack[i].substr(pos_beg_addr +1, pos_end_addr - pos_beg_addr -1);

			std::string cmd = "addr2line -e " + program + " " + address;
			std::string filename_and_line = run_system_command(cmd);
			filename_and_line = filename_and_line.substr(0, filename_and_line.size() -1); // remove the '\n'

			bt_str += program + function + " [" + filename_and_line + "]";
		}
		else
		{
			bt_str += stack[i];
		}

		if (i < (stack.size()-1))
			bt_str += '\n';
	}

	return bt_str;

#else
	return backtrace;

#endif
}

std::string aff3ct::tools::get_binary_path()
{
	std::string binary_path;

#if defined(__linux__) || defined(linux) || defined(__linux) || defined(__FreeBSD__)
	constexpr size_t path_size = 8192;
	char path[path_size];
#ifdef __FreeBSD__
	auto len = readlink("/proc/curproc/file", path, path_size);
#else
	auto len = readlink("/proc/self/exe", path, path_size);
#endif
	if (len == -1)
	{
		std::stringstream message;
		message << "'readlink' failed ('errno' = " << strerror(errno) << ").";
		throw runtime_error(__FILE__, __LINE__, __func__, message.str());
	}
	path[len] = '\0';
	binary_path = path;
#elif defined(__APPLE__) || defined(__MACH__)
	constexpr size_t path_size = 8192;
	char path[path_size];
	uint32_t size = sizeof(path);
	if (_NSGetExecutablePath(path, &size) != 0)
	{
		std::stringstream message;
		message << "'_NSGetExecutablePath' path buffer is too small; need 'size' = " << size << "('path_size' = "
		        << path_size << ").";
		throw runtime_error(__FILE__, __LINE__, __func__, message.str());
	}
	binary_path = path;
#elif defined(_WIN32) || defined(_WIN64)
	constexpr size_t path_size = 8192;
	char path[path_size];
	DWORD copied = 0;
	copied = GetModuleFileName(NULL, path, path_size);
	if (copied >= path_size)
	{
		std::stringstream message;
		message << "'GetModuleFileName' path buffer is too small ('path_size' = " << path_size << ").";
		throw runtime_error(__FILE__, __LINE__, __func__, message.str());
	}
	binary_path = path;
#endif

	return binary_path;
}

void aff3ct::tools::split_path(const std::string& path, std::string &basedir, std::string &filename)
{
  size_t found;
  found = path.find_last_of("/\\");
  basedir = path.substr(0,found);
  filename = path.substr(found+1);
}