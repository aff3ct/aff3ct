#ifndef PATTERN_POLAR_RATE_0_HPP_
#define PATTERN_POLAR_RATE_0_HPP_

#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <typeinfo>

#include "Pattern_polar_i.hpp"

namespace aff3ct
{
namespace tools
{
class Pattern_polar_r0 : public Pattern_polar_i
{
protected:
	Pattern_polar_r0(const int &N, const Binary_node<Pattern_polar_i>* node,
	                 const int min_level = 0, const int max_level = -1)
	: Pattern_polar_i(N, node, min_level, max_level)
	{
	}

public:
	Pattern_polar_r0(const int min_level = 0, const int max_level = -1)
	: Pattern_polar_i(min_level, max_level)
	{
	}

	virtual Pattern_polar_i* alloc(const int &N, const Binary_node<Pattern_polar_i>* node) const
	{
		return new Pattern_polar_r0(N, node, min_level, max_level);
	}

	virtual ~Pattern_polar_r0() = default;

	virtual polar_node_t type()       const { return polar_node_t::RATE_0; }
	virtual std::string  name()       const { return "Rate 0";             }
	virtual std::string  short_name() const { return "r0";                 }
	virtual std::string  fill_color() const { return "#FFFFFF";            }
	virtual std::string  font_color() const { return "#000000";            }

	virtual std::string f() const { return ""; }
	virtual std::string g() const { return ""; }
	virtual std::string h() const { return ""; }

	static bool recursive_check(const int reverse_graph_depth, const Binary_node<Pattern_polar_i>* node_curr)
	{
		if (!node_curr->is_leaf())
		{
			const auto pattern_left  = node_curr->get_left ()->get_contents();
			const auto pattern_right = node_curr->get_right()->get_contents();

			if (pattern_left ->type() == polar_node_t::RATE_0 &&
			    pattern_right->type() == polar_node_t::RATE_0)
			{
				return true;
			}
			else
			{
				return Pattern_polar_r0::recursive_check(reverse_graph_depth -1, node_curr->get_left ()) &&
				       Pattern_polar_r0::recursive_check(reverse_graph_depth -1, node_curr->get_right());
			}
		}
		else
		{
			const auto pattern_leaf = node_curr->get_contents();
			return pattern_leaf->type() == polar_node_t::RATE_0;
		}
	}

	virtual int _match(const int &reverse_graph_depth, const Binary_node<Pattern_polar_i>* node_curr) const
	{
		return Pattern_polar_r0::recursive_check(reverse_graph_depth, node_curr) ? 100 : 0;
	}

	virtual bool is_terminal() const { return true; }
};
}
}

#endif /* PATTERN_POLAR_RATE_0_HPP_ */
