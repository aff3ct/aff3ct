#include "Tools/Code/Polar/Patterns/Pattern_polar_r1.hpp"

namespace aff3ct
{
namespace tools
{

Pattern_polar_r1
::Pattern_polar_r1(const int &N, const Binary_node<Pattern_polar_i>* node, const int min_level, const int max_level)
: Pattern_polar_i(N, node, min_level, max_level)
{
}

Pattern_polar_r1
::Pattern_polar_r1(const int min_level, const int max_level)
: Pattern_polar_i(min_level, max_level)
{
}

Pattern_polar_r1* Pattern_polar_r1
::alloc(const int &N, const Binary_node<Pattern_polar_i>* node) const
{
	if (node == nullptr)
		return new Pattern_polar_r1(min_level, max_level);
	else
		return new Pattern_polar_r1(N, node, min_level, max_level);
}

polar_node_t Pattern_polar_r1
::type() const
{
	return polar_node_t::RATE_1;
}

std::string Pattern_polar_r1
::name() const
{
	return "Rate 1";
}

std::string Pattern_polar_r1
::short_name() const
{
	return "r1";
}

std::string Pattern_polar_r1
::fill_color() const
{
	return "#000000";
}

std::string Pattern_polar_r1
::font_color() const
{
	return "#FFFFFF";
}

std::string Pattern_polar_r1
::f() const
{
	return "";
}

std::string Pattern_polar_r1
::g() const
{
	return "";
}

std::string Pattern_polar_r1
::h() const
{
	return "h";
}

bool Pattern_polar_r1
::recursive_check(const int reverse_graph_depth, const Binary_node<Pattern_polar_i>* node_curr)
{
	if (!node_curr->is_leaf())
	{
		const auto pattern_left  = node_curr->get_left ()->get_contents();
		const auto pattern_right = node_curr->get_right()->get_contents();

		if (pattern_left ->type() == polar_node_t::RATE_1 &&
		    pattern_right->type() == polar_node_t::RATE_1)
		{
			return true;
		}
		else
		{
			return Pattern_polar_r1::recursive_check(reverse_graph_depth -1, node_curr->get_left ()) &&
			       Pattern_polar_r1::recursive_check(reverse_graph_depth -1, node_curr->get_right());
		}
	}
	else
	{
		const auto pattern_leaf = node_curr->get_contents();
		return pattern_leaf->type() == polar_node_t::RATE_1;
	}
}

int Pattern_polar_r1
::_match(const int &reverse_graph_depth, const Binary_node<Pattern_polar_i>* node_curr) const
{
	return Pattern_polar_r1::recursive_check(reverse_graph_depth, node_curr) ? 99 : 0;
}

bool Pattern_polar_r1
::is_terminal() const
{
	return true;
}
}
}
