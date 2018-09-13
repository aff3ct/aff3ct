#include "range_error.hpp"

using namespace aff3ct::tools;

const std::string range_error::default_message = "Range error.";

range_error
::range_error() throw()
: exception()
{
}

range_error
::range_error(const std::string &message) throw()
: exception(message.empty() ? default_message : message)
{
}

range_error
::range_error(const std::string &filename,
              const int line_num,
              const std::string &funcname,
              const std::string &message) throw()
: exception(filename, line_num, funcname, message.empty() ? default_message : message)
{
}
