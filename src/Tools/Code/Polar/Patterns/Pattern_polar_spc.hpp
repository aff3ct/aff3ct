#ifndef PATTERN_POLAR_SPC_HPP_
#define PATTERN_POLAR_SPC_HPP_

#include <cassert>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <typeinfo>

#include "Pattern_polar_i.hpp"

#include "Pattern_polar_r0.hpp"
#include "Pattern_polar_r1.hpp"
#include "Pattern_polar_rep.hpp"

class Pattern_polar_spc : public Pattern_polar_i
{
protected:
	Pattern_polar_spc(const int &N, const Binary_node<Pattern_polar_i>* node,
	                  const int min_level = 2, const int max_level = -1)
	: Pattern_polar_i(N, node, min_level, max_level)
	{
		assert(min_level >= 2);
	}

public:
	Pattern_polar_spc(const int min_level = 2, const int max_level = -1)
	: Pattern_polar_i(min_level, max_level)
	{
		assert(min_level >= 2);
	}

	virtual Pattern_polar_i* alloc(const int &N, const Binary_node<Pattern_polar_i>* node) const
	{
		return new Pattern_polar_spc(N, node, min_level, max_level);
	}

	virtual ~Pattern_polar_spc() {}

	virtual polar_node_t type()       const { return polar_node_t::SPC;   }
	virtual std::string  name()       const { return "SPC" + range_name();}
	virtual std::string  short_name() const { return "s";                 }
	virtual std::string  fill_color() const { return "#2F3F60";           }
	virtual std::string  font_color() const { return "#FFFFFF";           }

	virtual std::string f() const { return "";    }
	virtual std::string g() const { return "";    }
	virtual std::string h() const { return "spc"; }

	static bool recursive_check(const int reverse_graph_depth, const Binary_node<Pattern_polar_i>* node_curr)
	{
		if (!node_curr->is_leaf())
		{
			const auto pattern_left  = node_curr->get_left ()->get_contents();
			const auto pattern_right = node_curr->get_right()->get_contents();

			if (pattern_right->type() == polar_node_t::RATE_1 ||
			    Pattern_polar_r1::recursive_check(reverse_graph_depth -1, node_curr->get_right()))
			{
				if (pattern_left->type() == polar_node_t::SPC)
					return true;
				else if (pattern_left->type() == polar_node_t::REP && reverse_graph_depth == 2)
					return true;
				else if (pattern_left->type() == polar_node_t::RATE_0 && reverse_graph_depth == 1)
					return true;
				else
					return Pattern_polar_spc::recursive_check(reverse_graph_depth -1, node_curr->get_left());
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

	virtual int _match(const int &reverse_graph_depth, const Binary_node<Pattern_polar_i>* node_curr) const
	{
		return Pattern_polar_spc::recursive_check(reverse_graph_depth, node_curr) ? 49 : 0;
	}

	virtual bool is_terminal() const { return true; }
};

#endif /* PATTERN_POLAR_SPC_HPP_ */
