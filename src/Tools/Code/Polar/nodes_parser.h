#ifndef NODES_PARSER_HPP
#define NODES_PARSER_HPP

#include <string>
#include <vector>

#include "Tools/Code/Polar/Patterns/Pattern_polar_i.hpp"

namespace aff3ct
{
namespace tools
{
std::vector<Pattern_polar_i*> nodes_parser(const std::string &str_polar,
                                                 int         &idx_r0,
                                                 int         &idx_r1,
                                           const std::string  type = "STD");
}
}

#endif /* NODES_PARSER_HPP */
