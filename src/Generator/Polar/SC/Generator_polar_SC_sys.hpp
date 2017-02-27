#ifndef GENERATOR_POLAR_SC_SYS_
#define GENERATOR_POLAR_SC_SYS_

#include <map>
#include <vector>

#include "Tools/Perf/MIPP/mipp.h"
#include "Tools/Algo/Tree/Binary_tree.hpp"
#include "Tools/Code/Polar/Patterns/Pattern_polar_i.hpp"
#include "Tools/Code/Polar/Pattern_polar_parser.hpp"

#include "../Generator_polar.hpp"

namespace aff3ct
{
namespace generator
{
class Generator_polar_SC_sys : public Generator_polar
{
public:
	Generator_polar_SC_sys(const int& K,
	                       const int& N,
	                       const float& snr,
	                       const mipp::vector<int>& frozen_bits,
	                       const std::vector<tools::Pattern_polar_i*> &patterns,
	                       const tools::Pattern_polar_i &pattern_rate0,
	                       const tools::Pattern_polar_i &pattern_rate1,
	                       std::ostream &dec_stream         = std::cout,
	                       std::ostream &short_dec_stream   = std::cout,
	                       std::ostream &graph_stream       = std::cout,
	                       std::ostream &short_graph_stream = std::cout);
	virtual ~Generator_polar_SC_sys();

protected:
	void generate_class_header(const std::string class_name,
	                           const std::string fbits_name,
	                                 std::ostream &stream1,
	                                 std::ostream &stream2);
	void generate_class_footer(      std::ostream &stream);

	void recursive_generate_decoder      (const tools::Binary_node<tools::Pattern_polar_i>* node_curr, std::ostream &stream);
	void recursive_generate_short_decoder(const tools::Binary_node<tools::Pattern_polar_i>* node_curr, std::ostream &stream);
};
}
}

#endif /* GENERATOR_POLAR_SC_SYS_ */
