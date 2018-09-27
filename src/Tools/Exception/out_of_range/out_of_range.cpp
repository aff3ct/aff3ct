#include "out_of_range.hpp"

using namespace aff3ct::tools;

const std::string out_of_range::default_message = "Out of range.";

out_of_range
::out_of_range() throw()
: exception()
{
}

out_of_range
::out_of_range(const std::string &message) throw()
: exception(message.empty() ? default_message : message)
{
}

out_of_range
::out_of_range(const std::string &filename,
               const int line_num,
               const std::string &funcname,
               const std::string &message) throw()
: exception(filename, line_num, funcname, message.empty() ? default_message : message)
{
}