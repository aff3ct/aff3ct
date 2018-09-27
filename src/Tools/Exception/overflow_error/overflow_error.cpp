#include "overflow_error.hpp"

using namespace aff3ct::tools;

const std::string overflow_error::default_message = "Overflow error.";

overflow_error
::overflow_error() throw()
: exception()
{
}

overflow_error
::overflow_error(const std::string &message) throw()
: exception(message.empty() ? default_message : message)
{
}

overflow_error
::overflow_error(const std::string &filename,
                 const int line_num,
                 const std::string &funcname,
                 const std::string &message) throw()
: exception(filename, line_num, funcname, message.empty() ? default_message : message)
{
}