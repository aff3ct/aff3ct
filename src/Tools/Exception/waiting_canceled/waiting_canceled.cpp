#include "Tools/Exception/waiting_canceled/waiting_canceled.hpp"

using namespace aff3ct::tools;

const std::string waiting_canceled::default_message = "Waiting thread has been canceled.";

waiting_canceled
::waiting_canceled() throw()
: exception()
{
}

waiting_canceled
::waiting_canceled(const std::string &message) throw()
: exception(message.empty() ? default_message : message)
{
}

waiting_canceled
::waiting_canceled(const std::string &filename,
                   const int line_num,
                   const std::string &funcname,
                   const std::string &message) throw()
: exception(filename, line_num, funcname, message.empty() ? default_message : message)
{
}
