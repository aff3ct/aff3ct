#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Tools/Code/Polar/Patterns/Pattern_polar_rep_left.hpp"

namespace aff3ct
{
namespace tools
{
Pattern_polar_rep_left
::Pattern_polar_rep_left(const int &N, const Binary_node<Pattern_polar_i>* node, const int min_level,
                         const int max_level)
: Pattern_polar_i(N, node, min_level, max_level)
{
	if (min_level < 2)
	{
		std::stringstream message;
		message << "'min_level' has to be equal or greater than 2 ('min_level' = " << min_level << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

Pattern_polar_rep_left
::Pattern_polar_rep_left(const int min_level, const int max_level)
: Pattern_polar_i(min_level, max_level)
{
	if (min_level < 2)
	{
		std::stringstream message;
		message << "'min_level' has to be equal or greater than 2 ('min_level' = " << min_level << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

Pattern_polar_rep_left* Pattern_polar_rep_left
::alloc(const int &N, const Binary_node<Pattern_polar_i>* node) const
{
	if (node == nullptr)
		return new Pattern_polar_rep_left(min_level, max_level);
	else
		return new Pattern_polar_rep_left(N, node, min_level, max_level);
}

polar_node_t Pattern_polar_rep_left
::type() const
{
	return polar_node_t::REP_LEFT;
}

std::string Pattern_polar_rep_left
::name() const
{
	return "Rep left";
}

std::string Pattern_polar_rep_left
::short_name() const
{
	return "rl";
}

std::string Pattern_polar_rep_left
::fill_color() const
{
	return "#8e726f";
}

std::string Pattern_polar_rep_left
::font_color() const
{
	return "#FFFFFF";
}

std::string Pattern_polar_rep_left
::f() const
{
	return "f";
}

std::string Pattern_polar_rep_left
::g() const
{
	return "gr";
}

std::string Pattern_polar_rep_left
::h() const
{
	return "xo";
}

int Pattern_polar_rep_left
::_match(const int &reverse_graph_depth, const Binary_node<Pattern_polar_i>* node_curr) const
{
	const Pattern_polar_i *pattern_left  = node_curr->get_left ()->get_contents();

	if (pattern_left == nullptr)
		throw runtime_error(__FILE__, __LINE__, __func__, "'pattern_left' can't be null.");

	int match_val = 0;

	if (pattern_left->type() == polar_node_t::REP)
	{
		match_val = 19;
	}

	return match_val;
}

bool Pattern_polar_rep_left
::is_terminal() const
{
	return false;
}
}
}
