#ifndef DOCUMENTATION_H_
#define DOCUMENTATION_H_

#include <string>
#include <cli.hpp>

namespace aff3ct
{
namespace tools
{
void add_arg(cli::Argument_map_info &args, const std::string &prefix, const std::string &key, cli::Argument_type *arg_t,
             const cli::arg_rank rank = cli::arg_rank::OPT);
}
}

#endif // DOCUMENTATION_H_