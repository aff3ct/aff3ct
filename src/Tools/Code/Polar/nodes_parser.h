#ifndef NODES_PARSER_HPP
#define NODES_PARSER_HPP

#include <string>
#include <vector>

#include "Tools/Code/Polar/Patterns/Pattern_polar_i.hpp"
#include "Tools/Code/Polar/Patterns/Pattern_polar_r0.hpp"
#include "Tools/Code/Polar/Patterns/Pattern_polar_r0_left.hpp"
#include "Tools/Code/Polar/Patterns/Pattern_polar_r1.hpp"
#include "Tools/Code/Polar/Patterns/Pattern_polar_rep.hpp"
#include "Tools/Code/Polar/Patterns/Pattern_polar_rep_left.hpp"
#include "Tools/Code/Polar/Patterns/Pattern_polar_spc.hpp"
#include "Tools/Code/Polar/Patterns/Pattern_polar_std.hpp"

namespace aff3ct
{
namespace tools
{
template <class R0   = Pattern_polar_r0,
          class R0L  = Pattern_polar_r0_left,
          class R1   = Pattern_polar_r1,
          class REP  = Pattern_polar_rep,
          class REPL = Pattern_polar_rep_left,
          class SPC  = Pattern_polar_spc,
          class STD  = Pattern_polar_std>
std::vector<Pattern_polar_i*> nodes_parser(const std::string &str_polar, int &idx_r0, int &idx_r1);
}
}

#include "nodes_parser.hxx"

#endif /* NODES_PARSER_HPP */
