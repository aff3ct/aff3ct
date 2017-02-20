#ifndef PATTERN_POLAR_REP_HPP_
#define PATTERN_POLAR_REP_HPP_

#include <cassert>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <typeinfo>

#include "Pattern_polar_i.hpp"

#include "Pattern_polar_r0.hpp"
#include "Pattern_polar_r1.hpp"

class Pattern_polar_rep : public Pattern_polar_i
{
protected:
	int min_level;
	int max_level;

	Pattern_polar_rep(const int &N, const Binary_node<Pattern_polar_i>* node,
	                  const int min_level = -1, const int max_level = -1)
	: Pattern_polar_i(N, node), min_level(min_level), max_level(max_level)
	{
		assert(min_level == -1 || min_level >= 2);
		assert(max_level == -1 || (max_level >= 2 && max_level >= min_level));
	}

public:
	Pattern_polar_rep(const int min_level = -1, const int max_level = -1)
	: Pattern_polar_i(), min_level(min_level), max_level(max_level)
	{
		assert(min_level == -1 || min_level >= 2);
		assert(max_level == -1 || (max_level >= 2 && max_level >= min_level));
	}

	virtual Pattern_polar_i* alloc(const int &N, const Binary_node<Pattern_polar_i>* node) const
	{
		return new Pattern_polar_rep(N, node, min_level, max_level);
	}

	virtual ~Pattern_polar_rep() {}

	virtual polar_node_t type()       const { return polar_node_t::REP; }
	virtual std::string  name()       const { return "Rep";             }
	virtual std::string  short_name() const { return "re";              }
	virtual std::string  fill_color() const { return "#8F463F";         }
	virtual std::string  font_color() const { return "#FFFFFF";         }

	virtual std::string f() const { return "";    }
	virtual std::string g() const { return "";    }
	virtual std::string h() const { return "rep"; }

	bool recursive_check_rep(const int reverse_graph_depth, const Binary_node<Pattern_polar_i>* node_curr) const
	{
		if (max_level != -1 && reverse_graph_depth > max_level)
			return false;

		if (!node_curr->is_leaf())
		{
			const Pattern_polar_i *pattern_left  = node_curr->get_left ()->get_contents();
			const Pattern_polar_i *pattern_right = node_curr->get_right()->get_contents();

			if (pattern_left->type() != polar_node_t::RATE_0)
			{
				return false;
			}
			else
			{
				if (pattern_right->type() == polar_node_t::REP)
					return true;
				else if (pattern_right->type() == polar_node_t::RATE_1 && reverse_graph_depth == 1)
					return true;
				else
					return this->recursive_check_rep(reverse_graph_depth -1, node_curr->get_right());
			}
		}
		else
		{
			return false;
		}
	}

	virtual int match(const int &reverse_graph_depth, const Binary_node<Pattern_polar_i>* node_curr) const
	{
		assert(reverse_graph_depth > 0);

		const Pattern_polar_i *pattern_left  = node_curr->get_left ()->get_contents();
		const Pattern_polar_i *pattern_right = node_curr->get_right()->get_contents();

		assert(pattern_left  != nullptr);
		assert(pattern_right != nullptr);

		int match_val = 0;

		if (min_level == -1 || reverse_graph_depth >= min_level)
		{
			if (this->recursive_check_rep(reverse_graph_depth, node_curr))
			{
				match_val = 50;
			}
		}

		return match_val;
	}

	virtual bool is_terminal() const { return true; }
};

#endif /* PATTERN_POLAR_REP_HPP_ */
