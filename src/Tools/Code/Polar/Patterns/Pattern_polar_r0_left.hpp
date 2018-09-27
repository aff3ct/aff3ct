#ifndef PATTERN_POLAR_RATE_0_LEFT_HPP_
#define PATTERN_POLAR_RATE_0_LEFT_HPP_

#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <typeinfo>

#include "Tools/Exception/exception.hpp"

#include "Pattern_polar_i.hpp"

#include "Pattern_polar_r0.hpp"

namespace aff3ct
{
namespace tools
{
class Pattern_polar_r0_left : public Pattern_polar_i
{
protected:
	Pattern_polar_r0_left(const int &N, const Binary_node<Pattern_polar_i>* node,
	                      const int min_level = 1, const int max_level = -1)
	: Pattern_polar_i(N, node, min_level, max_level)
	{
		if (min_level < 1)
		{
			std::stringstream message;
			message << "'min_level' has to be equal or greater than 1 ('min_level' = " << min_level << ").";
			throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}
	}

public:
	Pattern_polar_r0_left(const int min_level = 1, const int max_level = -1)
	: Pattern_polar_i(min_level, max_level)
	{
		if (min_level < 1)
		{
			std::stringstream message;
			message << "'min_level' has to be equal or greater than 1 ('min_level' = " << min_level << ").";
			throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}
	}

	virtual Pattern_polar_i* alloc(const int &N, const Binary_node<Pattern_polar_i>* node) const
	{
		return new Pattern_polar_r0_left(N, node, min_level, max_level);
	}

	virtual ~Pattern_polar_r0_left() = default;

	virtual polar_node_t type()       const { return polar_node_t::RATE_0_LEFT; }
	virtual std::string  name()       const { return "Rate 0 left";             }
	virtual std::string  short_name() const { return "r0l";                     }
	virtual std::string  fill_color() const { return "#dadada";                 }
	virtual std::string  font_color() const { return "#000000";                 }

	virtual std::string f() const { return "f";   }
	virtual std::string g() const { return "g0";  }
	virtual std::string h() const { return "xo0"; }

	virtual int _match(const int &reverse_graph_depth, const Binary_node<Pattern_polar_i>* node_curr) const
	{
		const Pattern_polar_i *pattern_left = node_curr->get_left()->get_contents();

		if (pattern_left == nullptr)
			throw runtime_error(__FILE__, __LINE__, __func__, "'pattern_left' can't be null.");

		int match_val = 0;

		if (pattern_left->type() == polar_node_t::RATE_0)
		{
			match_val = 20;
		}

		return match_val;
	}

	virtual bool is_terminal() const { return false; }
};
}
}

#endif /* PATTERN_POLAR_RATE_0_LEFT_HPP_ */
