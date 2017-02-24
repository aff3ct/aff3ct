#ifndef PATTERN_SC_YIELD0_HPP_
#define PATTERN_SC_YIELD0_HPP_

#include <cassert>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <typeinfo>

#include "Pattern_SC_interface.hpp"

namespace aff3ct
{
template <>
class Pattern_SC<pattern_SC_type::RATE_0> : public Pattern_SC_interface
{
protected:
	Pattern_SC(const int &N,
	           const Binary_node<Pattern_SC_interface>* node)
	 : Pattern_SC_interface(N, node)
	{
	}

public:
	Pattern_SC() : Pattern_SC_interface() { }

	virtual Pattern_SC_interface* alloc(const int &N,
	                                    const Binary_node<Pattern_SC_interface>* node) const
	{
		return new Pattern_SC<pattern_SC_type::RATE_0>(N, node);
	}

	virtual ~Pattern_SC() {}

	virtual pattern_SC_type type()   const { return RATE_0;    }
	virtual std::string name()       const { return "Rate 0";  }
	virtual std::string short_name() const { return "r0";      }
	virtual std::string fill_color() const { return "#FFFFFF"; }
	virtual std::string font_color() const { return "#000000"; }

	virtual std::string f() const { return ""; }
	virtual std::string g() const { return ""; }
	virtual std::string h() const { return ""; }

	virtual std::string apply_f(std::string str_off_l = "", std::string str_off_s = "") const
	{
		return "";
	}

	virtual std::string apply_g(std::string str_off_l = "", std::string str_off_s = "") const
	{
		return "";
	}

	virtual std::string apply_h(std::string str_off_l = "", std::string str_off_s = "") const
	{
		return "";
	}

	virtual int match(const int &reverse_graph_depth,
	                  const Binary_node<Pattern_SC_interface>* node_curr) const
	{
		assert(reverse_graph_depth > 0);

		const Pattern_SC_interface *pattern_left  = node_curr->get_left ()->get_contents();
		const Pattern_SC_interface *pattern_right = node_curr->get_right()->get_contents();

		assert(pattern_left  != nullptr);
		assert(pattern_right != nullptr);

		int match_val = 0;

		if (typeid(*pattern_left)  == typeid(Pattern_SC<pattern_SC_type::RATE_0>) &&
		    typeid(*pattern_right) == typeid(Pattern_SC<pattern_SC_type::RATE_0>))
		{
			match_val = 100;
		}

		return match_val;
	}

	virtual bool is_terminal() const { return true; }
};
}

#endif /* PATTERN_SC_YIELD0_HPP_ */
