#include "Tools/Code/Polar/Patterns/Pattern_polar_std.hpp"

namespace aff3ct
{
namespace tools
{
Pattern_polar_std
::Pattern_polar_std(const int &N, const Binary_node<Pattern_polar_i>* node)
: Pattern_polar_i(N, node)
{
}

Pattern_polar_std
::Pattern_polar_std()
: Pattern_polar_i()
{
}

Pattern_polar_std* Pattern_polar_std
::alloc(const int &N, const Binary_node<Pattern_polar_i>* node) const
{
	if (node == nullptr)
		return new Pattern_polar_std();
	else
		return new Pattern_polar_std(N, node);
}

polar_node_t Pattern_polar_std
::type() const
{
	return polar_node_t::STANDARD;
}

std::string Pattern_polar_std
::name() const
{
	return "Default";
}

std::string Pattern_polar_std
::short_name() const
{
	return "d";
}

std::string Pattern_polar_std
::fill_color() const
{
	return "#7F7F7F";
}

std::string Pattern_polar_std
::font_color() const
{
	return "#FFFFFF";
}

std::string Pattern_polar_std
::f() const
{
	return "f";
}

std::string Pattern_polar_std
::g() const
{
	return "g";
}

std::string Pattern_polar_std
::h() const
{
	return "xo";
}

int Pattern_polar_std
::_match(const int &reverse_graph_depth, const Binary_node<Pattern_polar_i>* node_curr) const
{
	int match_val = 1;

	return match_val;
}

bool Pattern_polar_std
::is_terminal() const
{
	return false;
}
}
}
