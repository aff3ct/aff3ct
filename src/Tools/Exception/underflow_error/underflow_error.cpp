#include "underflow_error.hpp"

using namespace aff3ct::tools;

const std::string underflow_error::default_message = "Underflow error.";

underflow_error
::underflow_error() throw()
: exception()
{
}

underflow_error
::underflow_error(const std::string &message) throw()
: exception(message.empty() ? default_message : message)
{
}

underflow_error
::underflow_error(const std::string &filename,
                  const int line_num,
                  const std::string &funcname,
                  const std::string &message) throw()
: exception(filename, line_num, funcname, message.empty() ? default_message : message)
{
}
