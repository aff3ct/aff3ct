#ifndef DOCUMENTATION_H_
#define DOCUMENTATION_H_

#include <string>

#include "Tools/Arguments/Argument_tools.hpp"

namespace aff3ct
{
namespace tools
{
void add_arg(Argument_map_info &args, const std::string &prefix, const std::string &key, Argument_type *arg_t,
             const arg_rank rank = arg_rank::OPT);
}
}

#endif // DOCUMENTATION_H_