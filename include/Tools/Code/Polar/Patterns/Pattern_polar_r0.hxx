#include "Tools/Code/Polar/Patterns/Pattern_polar_r0.hpp"

namespace aff3ct
{
namespace tools
{
Pattern_polar_r0
::Pattern_polar_r0(const int &N, const Binary_node<Pattern_polar_i>* node, const int min_level, const int max_level)
: Pattern_polar_i(N, node, min_level, max_level)
{
}

Pattern_polar_r0
::Pattern_polar_r0(const int min_level, const int max_level)
: Pattern_polar_i(min_level, max_level)
{
}

Pattern_polar_r0* Pattern_polar_r0
::alloc(const int &N, const Binary_node<Pattern_polar_i>* node) const
{
	if (node == nullptr)
		return new Pattern_polar_r0(min_level, max_level);
	else
		return new Pattern_polar_r0(N, node, min_level, max_level);
}

polar_node_t Pattern_polar_r0
::type() const
{
	return polar_node_t::RATE_0;
}

std::string Pattern_polar_r0
::name() const
{
	return "Rate 0";
}

std::string Pattern_polar_r0
::short_name() const
{
	return "r0";
}

std::string Pattern_polar_r0
::fill_color() const
{
	return "#FFFFFF";
}

std::string Pattern_polar_r0
::font_color() const
{
	return "#000000";
}

std::string Pattern_polar_r0
::f() const
{
	return "";
}

std::string Pattern_polar_r0
::g() const
{
	return "";
}

std::string Pattern_polar_r0
::h() const
{
	return "";
}

bool Pattern_polar_r0
::recursive_check(const int reverse_graph_depth, const Binary_node<Pattern_polar_i>* node_curr)
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

int Pattern_polar_r0
::_match(const int &reverse_graph_depth, const Binary_node<Pattern_polar_i>* node_curr) const
{
	return Pattern_polar_r0::recursive_check(reverse_graph_depth, node_curr) ? 100 : 0;
}

bool Pattern_polar_r0
::is_terminal() const
{
	return true;
}
}
}
