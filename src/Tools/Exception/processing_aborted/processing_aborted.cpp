#include "Tools/Exception/processing_aborted/processing_aborted.hpp"

using namespace aff3ct::tools;

const std::string processing_aborted::default_message = "Processing has been aborted.";

processing_aborted
::processing_aborted() throw()
: exception()
{
}

processing_aborted
::processing_aborted(const std::string &message) throw()
: exception(message.empty() ? default_message : message)
{
}

processing_aborted
::processing_aborted(const std::string &filename,
                     const int line_num,
                     const std::string &funcname,
                     const std::string &message) throw()
: exception(filename, line_num, funcname, message.empty() ? default_message : message)
{
}
