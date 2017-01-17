#ifndef PATTERN_POLAR_RATE_0_LEFT_HPP_
#define PATTERN_POLAR_RATE_0_LEFT_HPP_

#include <cassert>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <typeinfo>

#include "Pattern_polar_i.hpp"

#include "Pattern_polar_r0.hpp"

class Pattern_polar_r0_left : public Pattern_polar_i
{
protected:
	Pattern_polar_r0_left(const int &N, const Binary_node<Pattern_polar_i>* node) : Pattern_polar_i(N, node)
	{
	}

public:
	Pattern_polar_r0_left() : Pattern_polar_i() { }

	virtual Pattern_polar_i* alloc(const int &N, const Binary_node<Pattern_polar_i>* node) const
	{
		return new Pattern_polar_r0_left(N, node);
	}

	virtual ~Pattern_polar_r0_left() {}

	virtual polar_node_t type()       const { return polar_node_t::RATE_0_LEFT; }
	virtual std::string  name()       const { return "Rate 0 left";             }
	virtual std::string  short_name() const { return "r0l";                     }
	virtual std::string  fill_color() const { return "#dadada";                 }
	virtual std::string  font_color() const { return "#000000";                 }

	virtual std::string f() const { return "";    }
	virtual std::string g() const { return "g0";  }
	virtual std::string h() const { return "xo0"; }

	virtual int match(const int &reverse_graph_depth, const Binary_node<Pattern_polar_i>* node_curr) const
	{
		assert(reverse_graph_depth > 0);

		const Pattern_polar_i *pattern_left  = node_curr->get_left ()->get_contents();

		assert(pattern_left != nullptr);

		int match_val = 0;

//		if (typeid(*pattern_left) == typeid(Pattern_polar_r0))
		if (pattern_left->type() == polar_node_t::RATE_0)
		{
			match_val = 20;
		}

		return match_val;
	}

	virtual bool is_terminal() const { return false; }
};

#endif /* PATTERN_POLAR_RATE_0_LEFT_HPP_ */
