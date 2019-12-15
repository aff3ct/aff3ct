#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Tools/Code/Polar/Patterns/Pattern_polar_r0.hpp"
#include "Tools/Code/Polar/Patterns/Pattern_polar_rep.hpp"

namespace aff3ct
{
namespace tools
{
Pattern_polar_rep
::Pattern_polar_rep(const int &N, const Binary_node<Pattern_polar_i>* node, const int min_level, const int max_level)
: Pattern_polar_i(N, node, min_level, max_level)
{
	if (min_level < 1)
	{
		std::stringstream message;
		message << "'min_level' has to be equal or greater than 1 ('min_level' = " << min_level << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

Pattern_polar_rep
::Pattern_polar_rep(const int min_level, const int max_level)
: Pattern_polar_i(min_level, max_level)
{
	if (min_level < 1)
	{
		std::stringstream message;
		message << "'min_level' has to be equal or greater than 1 ('min_level' = " << min_level << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

Pattern_polar_rep* Pattern_polar_rep
::alloc(const int &N, const Binary_node<Pattern_polar_i>* node) const
{
	if (node == nullptr)
		return new Pattern_polar_rep(min_level, max_level);
	else
		return new Pattern_polar_rep(N, node, min_level, max_level);
}

polar_node_t Pattern_polar_rep
::type() const
{
	return polar_node_t::REP;
}

std::string Pattern_polar_rep
::name() const
{
	return "Rep";
}

std::string Pattern_polar_rep
::short_name() const
{
	return "re";
}

std::string Pattern_polar_rep
::fill_color() const
{
	return "#8F463F";
}

std::string Pattern_polar_rep
::font_color() const
{
	return "#FFFFFF";
}

std::string Pattern_polar_rep
::f() const
{
	return "";
}

std::string Pattern_polar_rep
::g() const
{
	return "";
}

std::string Pattern_polar_rep
::h() const
{
	return "rep";
}

bool Pattern_polar_rep
::recursive_check(const int reverse_graph_depth, const Binary_node<Pattern_polar_i>* node_curr)
{
	if (!node_curr->is_leaf())
	{
		const auto pattern_left  = node_curr->get_left ()->get_contents();
		const auto pattern_right = node_curr->get_right()->get_contents();

		if (pattern_left->type() == polar_node_t::RATE_0 ||
			Pattern_polar_r0::recursive_check(reverse_graph_depth -1, node_curr->get_left()))
		{
			if (pattern_right->type() == polar_node_t::REP)
				return true;
			else if (pattern_right->type() == polar_node_t::RATE_1 && reverse_graph_depth == 1)
				return true;
			else
				return Pattern_polar_rep::recursive_check(reverse_graph_depth -1, node_curr->get_right());
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

int Pattern_polar_rep
::_match(const int &reverse_graph_depth, const Binary_node<Pattern_polar_i>* node_curr) const
{
	return Pattern_polar_rep::recursive_check(reverse_graph_depth, node_curr) ? 50 : 0;
}

bool Pattern_polar_rep
::is_terminal() const
{
	return true;
}
}
}
