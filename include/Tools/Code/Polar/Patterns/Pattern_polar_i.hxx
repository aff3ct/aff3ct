#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <cmath>
#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Tools/Code/Polar/Patterns/Pattern_polar_i.hpp"

namespace aff3ct
{
namespace tools
{
Pattern_polar_i
::Pattern_polar_i(const int &N, const Binary_node<Pattern_polar_i>* node, const int min_level, const int max_level)
: N(N),
  m((int)std::log2(N)),
  size(N),
  si_2(0),
  node(node),
  off_l(0),
  off_s(0),
  rev_depth(m - node->get_depth()),
  n_dig((int) (1 + std::log10(2 * N))),
  n2_dig((int) (1 + std::log10(N >> 1))),
  tab("\t"),
  id(0),
  key(""),
  min_level(min_level),
  max_level(max_level)
{
	if (N <= 0)
	{
		std::stringstream message;
		message << "'N' has to be greater than 0 ('N' = " << N << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (node == nullptr)
		throw invalid_argument(__FILE__, __LINE__, __func__, "'node' can't be null.");

	if (min_level < 0)
	{
		std::stringstream message;
		message << "'min_level' has to be equal or greater than 0 ('min_level' = " << min_level << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (max_level != -1 && (max_level < 0 || max_level < min_level))
	{
		std::stringstream message;
		message << "'max_level' has to be equal or greater than 'min_level' ('max_level' = " << max_level
		        << ", 'min_level' = " << min_level << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	const int *p_size = &size;
	for (auto i = 0; i < node->get_depth(); i++) *const_cast<int*>(p_size) >>= 1;

	const int *p_si_2 = &si_2;
	*const_cast<int*>(p_si_2) = size >> 1;

	const int *p_off_l = &off_l;
	for (auto i = N; i > (N >> node->get_depth()); i >>= 1) *const_cast<int*>(p_off_l) += i;

	const int *p_off_s = &off_s;
	*const_cast<int*>(p_off_s) = node->get_lane_id() * size;
}

Pattern_polar_i
::Pattern_polar_i(const int min_level, const int max_level)
: N(0), m(0), size(0), si_2(0), node(nullptr), off_l(0), off_s(0), rev_depth(0), n_dig(0), n2_dig(0), tab("\t"),
  id(0), key(""), min_level(min_level), max_level(max_level)
{
	if (min_level < 0)
	{
		std::stringstream message;
		message << "'min_level' has to be equal or greater than 0 ('min_level' = " << min_level << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (max_level != -1 && (max_level < 0 || max_level < min_level))
	{
		std::stringstream message;
		message << "'max_level' has to be equal or greater than 'min_level' ('max_level' = " << max_level
		        << ", 'min_level' = " << min_level << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

void Pattern_polar_i
::set_id(const unsigned id)
{
	this->id = id;
}

unsigned Pattern_polar_i
::get_id()
{
	return this->id;
}

void Pattern_polar_i
::set_key(const std::string &key)
{
	this->key = key;
}

std::string Pattern_polar_i
::get_key()
{
	return this->key;
}

int Pattern_polar_i
::get_size()
{
	return size;
}

int Pattern_polar_i
::get_si_2 ()
{
	return si_2;
}

int Pattern_polar_i
::get_off_l()
{
	return off_l;
}

int Pattern_polar_i
::get_off_s()
{
	return off_s;
}

int Pattern_polar_i
::get_min_lvl()
{
	return min_level;
}

int Pattern_polar_i
::get_max_lvl()
{
	return max_level;
}

std::string Pattern_polar_i
::apply_f(std::string start_indent, std::string str_off_l, std::string str_off_s) const
{
	return "";
}

std::string Pattern_polar_i
::apply_g(std::string start_indent, std::string str_off_l, std::string str_off_s) const
{
	return "";
}

std::string Pattern_polar_i
::apply_h(std::string start_indent, std::string str_off_l, std::string str_off_s) const
{
	return "";
}

std::string Pattern_polar_i
::range() const
{
	if (min_level == 0 && max_level == -1)
		return "";
	else if (min_level > 0 && max_level == -1)
		return " (" + std::to_string((int)std::exp2(min_level)) + "+)";
	else if (max_level == min_level)
		return " (" + std::to_string((int)std::exp2(min_level)) + ")";
	else
		return " (" + std::to_string((int)std::exp2(min_level)) + "-" + std::to_string((int)std::exp2(max_level)) + ")";
}

int Pattern_polar_i
::match(const int &reverse_graph_depth, const Binary_node<Pattern_polar_i>* node_curr)
{
	if (reverse_graph_depth <= 0)
	{
		std::stringstream message;
		message << "'reverse_graph_depth' has to be greater than 0 ('reverse_graph_depth' = "
		        << reverse_graph_depth << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (node_curr == nullptr)
		throw invalid_argument(__FILE__, __LINE__, __func__, "'node_curr' can't be null.");

	if ((reverse_graph_depth >= min_level) && (max_level == -1 || reverse_graph_depth <= max_level))
		return _match(reverse_graph_depth, node_curr);
	else
		return 0;
}
}
}
