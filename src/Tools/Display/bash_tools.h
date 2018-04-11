#ifndef BASH_TOOLS_H_
#define BASH_TOOLS_H_

#include <string>
#include <numeric>

namespace aff3ct
{
namespace tools
{
std::string format_error  (const std::string &str);
std::string format_warning(const std::string &str);
std::string format_info   (const std::string &str);

using format_function = std::add_pointer<std::string(const std::string&)>::type;
std::string apply_on_each_line(const std::string& str, const format_function fptr);
}
}

#endif /* BASH_TOOLS_H_ */
