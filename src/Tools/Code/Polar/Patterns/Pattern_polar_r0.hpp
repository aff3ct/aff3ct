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
	Pattern_polar_r0(const int &N, const Binary_node<Pattern_polar_i>* node) : Pattern_polar_i(N, node)
	{
	}

public:
	Pattern_polar_r0() : Pattern_polar_i() { }

	virtual Pattern_polar_i* alloc(const int &N, const Binary_node<Pattern_polar_i>* node) const
	{
		return new Pattern_polar_r0(N, node);
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

	virtual int match(const int &reverse_graph_depth, const Binary_node<Pattern_polar_i>* node_curr) const
	{
		assert(reverse_graph_depth > 0);

		const Pattern_polar_i *pattern_left  = node_curr->get_left ()->get_contents();
		const Pattern_polar_i *pattern_right = node_curr->get_right()->get_contents();

		assert(pattern_left  != nullptr);
		assert(pattern_right != nullptr);

		int match_val = 0;

		if (pattern_left ->type() == polar_node_t::RATE_0 &&
		    pattern_right->type() == polar_node_t::RATE_0)
		{
			match_val = 100;
		}

		return match_val;
	}

	virtual bool is_terminal() const { return true; }
};

#endif /* PATTERN_POLAR_RATE_0_HPP_ */
