#ifndef PATTERN_POLAR_RATE_0_HPP_
#define PATTERN_POLAR_RATE_0_HPP_

#include <cassert>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <typeinfo>

#include "Pattern_polar_i.hpp"

class Pattern_polar_r0 : public Pattern_polar_i
{
protected:
	int min_level;
	int max_level;

	Pattern_polar_r0(const int &N, const Binary_node<Pattern_polar_i>* node,
	                 const int min_level = -1, const int max_level = -1)
	: Pattern_polar_i(N, node), min_level(min_level), max_level(max_level)
	{
		assert(min_level == -1 || min_level >= 0);
		assert(max_level == -1 || (max_level >= 0 && max_level >= min_level));
	}


public:
	Pattern_polar_r0(const int min_level = -1, const int max_level = -1)
	: Pattern_polar_i(), min_level(min_level), max_level(max_level)
	{
		assert(min_level == -1 || min_level >= 0);
		assert(max_level == -1 || (max_level >= 0 && max_level >= min_level));
	}

	virtual Pattern_polar_i* alloc(const int &N, const Binary_node<Pattern_polar_i>* node) const
	{
		return new Pattern_polar_r0(N, node, min_level, max_level);
	}

	virtual ~Pattern_polar_r0() {}

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

	virtual int match(const int &reverse_graph_depth, const Binary_node<Pattern_polar_i>* node_curr) const
	{
		assert(reverse_graph_depth > 0);

		const auto pattern_left  = node_curr->get_left ()->get_contents();
		const auto pattern_right = node_curr->get_right()->get_contents();

		assert(pattern_left  != nullptr);
		assert(pattern_right != nullptr);

		int match_val = 0;

		if ((min_level == -1 || reverse_graph_depth >= min_level) &&
		    (max_level == -1 || reverse_graph_depth <= max_level))
			if (Pattern_polar_r0::recursive_check(reverse_graph_depth, node_curr))
				match_val = 100;

		return match_val;
	}

	virtual bool is_terminal() const { return true; }
};

#endif /* PATTERN_POLAR_RATE_0_HPP_ */
