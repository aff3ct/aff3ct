#include "logic_error.hpp"

using namespace aff3ct::tools;

const std::string logic_error::default_message = "Logic error.";

logic_error
::logic_error() throw()
: exception()
{
}

logic_error
::logic_error(const std::string &message) throw()
: exception(message.empty() ? default_message : message)
{
}

logic_error
::logic_error(const std::string &filename,
              const int line_num,
              const std::string &funcname,
              const std::string &message) throw()
: exception(filename, line_num, funcname, message.empty() ? default_message : message)
{
}