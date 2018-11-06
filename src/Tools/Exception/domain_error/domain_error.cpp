#include "domain_error.hpp"

using namespace aff3ct::tools;

const std::string domain_error::default_message = "Domain error.";

domain_error
::domain_error() throw()
: exception()
{
}

domain_error
::domain_error(const std::string &message) throw()
: exception(message.empty() ? default_message : message)
{
}

domain_error
::domain_error(const std::string &filename,
               const int line_num,
               const std::string &funcname,
               const std::string &message) throw()
: exception(filename, line_num, funcname, message.empty() ? default_message : message)
{
}