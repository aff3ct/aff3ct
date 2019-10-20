#include "Tools/Code/Polar/Pattern_polar_parser.hpp"

namespace aff3ct
{
namespace tools
{
polar_node_t Pattern_polar_parser
::get_node_type(const int node_id) const
{
	return (polar_node_t)pattern_types[node_id];
}

bool Pattern_polar_parser
::exist_node_type(const polar_node_t node_type, const int rev_depth) const
{
	if (rev_depth <= 0)
	{
		if (node_type == polar_node_t::RATE_0) return true;
		if (node_type == polar_node_t::RATE_1) return true;
	}

	for (auto i = 0; i < (int)patterns.size(); i++)
		if (patterns[i]->type() == node_type)
		{
			if (rev_depth == -1)
			{
				return true;
			}
			else
			{
				auto min_lvl = patterns[i]->get_min_lvl();
				auto max_lvl = patterns[i]->get_max_lvl();

				if (rev_depth >= min_lvl && (rev_depth <= max_lvl || max_lvl == -1))
					return true;
				else
					return false;
			}
		}

	return false;
}
}
}
