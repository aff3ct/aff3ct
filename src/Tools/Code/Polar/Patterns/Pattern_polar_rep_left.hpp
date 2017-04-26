#ifndef PATTERN_POLAR_REP_LEFT_HPP_
#define PATTERN_POLAR_REP_LEFT_HPP_

#include <stdexcept>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <typeinfo>

#include "Pattern_polar_i.hpp"

#include "Pattern_polar_rep.hpp"

namespace aff3ct
{
namespace tools
{
class Pattern_polar_rep_left : public Pattern_polar_i
{
protected:
	Pattern_polar_rep_left(const int &N, const Binary_node<Pattern_polar_i>* node,
	                       const int min_level = 2, const int max_level = -1)
	: Pattern_polar_i(N, node, min_level, max_level)
	{
		if (min_level < 2)
			throw std::invalid_argument("aff3ct::module::Pattern_polar_rep_left: \"min_level\" has to be "
			                            "equal or greater than 2.");
	}

public:
	Pattern_polar_rep_left(const int min_level = 2, const int max_level = -1)
	: Pattern_polar_i(min_level, max_level)
	{
		if (min_level < 2)
			throw std::invalid_argument("aff3ct::module::Pattern_polar_rep_left: \"min_level\" has to be "
			                            "equal or greater than 2.");
	}

	virtual Pattern_polar_i* alloc(const int &N, const Binary_node<Pattern_polar_i>* node) const
	{
		return new Pattern_polar_rep_left(N, node, min_level, max_level);
	}

	virtual ~Pattern_polar_rep_left() {}

	virtual polar_node_t type()       const { return polar_node_t::REP_LEFT; }
	virtual std::string  name()       const { return "Rep left";             }
	virtual std::string  short_name() const { return "rl";                   }
	virtual std::string  fill_color() const { return "#8e726f";              }
	virtual std::string  font_color() const { return "#FFFFFF";              }

	virtual std::string f() const { return "f";  }
	virtual std::string g() const { return "gr"; }
	virtual std::string h() const { return "xo"; }

	virtual int _match(const int &reverse_graph_depth, const Binary_node<Pattern_polar_i>* node_curr) const
	{
		const Pattern_polar_i *pattern_left  = node_curr->get_left ()->get_contents();

		if (pattern_left == nullptr)
			throw std::runtime_error("aff3ct::module::Pattern_polar_rep_left: \"pattern_left\" can't be null.");

		int match_val = 0;

		if (pattern_left->type() == polar_node_t::REP)
		{
			match_val = 19;
		}

		return match_val;
	}

	virtual bool is_terminal() const { return false; }
};
}
}

#endif /* PATTERN_POLAR_REP_LEFT_HPP_ */
