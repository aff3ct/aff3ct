/*!
 * \file
 * \brief Struct tools::Nodes_parser.
 */
#ifndef NODES_PARSER_HPP
#define NODES_PARSER_HPP

#include <string>
#include <vector>
#include <memory>

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
struct Nodes_parser
{
private:
	static void push_back_polar_pattern(std::vector<Pattern_polar_i*> *polar_patterns_ptr,
	                                    std::vector<std::unique_ptr<Pattern_polar_i>> *polar_patterns_uptr,
	                                    Pattern_polar_i* polar_pattern);

	static void parse(const std::string &str_polar, int &idx_r0, int &idx_r1,
	                        std::vector<Pattern_polar_i*> *polar_patterns_ptr,
	                        std::vector<std::unique_ptr<Pattern_polar_i>> *polar_patterns_uptr);

public:
	static std::vector<std::unique_ptr<Pattern_polar_i>> parse_uptr(const std::string &str_polar, int &idx_r0, int &idx_r1);

	static std::vector<Pattern_polar_i*> parse_ptr(const std::string &str_polar, int &idx_r0, int &idx_r1);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Tools/Code/Polar/Nodes_parser.hxx"
#endif

#endif /* NODES_PARSER_HPP */
