#ifndef PATTERN_SC_YIELD0_LEFT_HPP_
#define PATTERN_SC_YIELD0_LEFT_HPP_

#include <cassert>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <typeinfo>

#include "Pattern_SC_interface.hpp"
#include "Pattern_SC_rate0.hpp"

namespace aff3ct
{
template <>
class Pattern_SC<pattern_SC_type::RATE_0_LEFT> : public Pattern_SC_interface
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
		return new Pattern_SC<pattern_SC_type::RATE_0_LEFT>(N, node);
	}

	virtual ~Pattern_SC() {}

	virtual pattern_SC_type type()   const { return RATE_0_LEFT;   }
	virtual std::string name()       const { return "Rate 0 left"; }
	virtual std::string short_name() const { return "r0l";         }
	virtual std::string fill_color() const { return "#dadada";     }
	virtual std::string font_color() const { return "#000000";     }

	virtual std::string f() const { return "";    }
	virtual std::string g() const { return "g0";  }
	virtual std::string h() const { return "xo0"; }

	virtual std::string apply_f(std::string str_off_l = "", std::string str_off_s = "") const
	{
		return "";
	}

	virtual std::string apply_g(std::string str_off_l = "", std::string str_off_s = "") const
	{
		// using namespace std;
		if (str_off_l.empty()) str_off_l = std::to_string(this->off_l);
		if (str_off_s.empty()) str_off_s = std::to_string(this->off_s);

		auto apply_g0 = g() + " ";
		std::string spaces = ""; for (auto i = 0; i < 2*this->n_dig+1; i++) spaces += " ";

		std::stringstream stream;
		stream << "API_polar::template "  << apply_g0   << "<" << std::setw(this->n2_dig) << this->si_2 << ">("
		       << "   "
		       << "l, "
		       << std::setw(this->n_dig ) << str_off_l  << "+" << std::setw(this->n_dig ) << 0          << ", "
		       << std::setw(this->n_dig ) << str_off_l  << "+" << std::setw(this->n_dig ) << this->si_2 << ", "
		       << spaces                                                                                << "  "
		       << std::setw(this->n_dig ) << str_off_l  << "+" << std::setw(this->n_dig ) << this->size << ", "
		       << std::setw(this->n2_dig) << this->si_2 << ");" << std::endl;

		return stream.str();
	}

	virtual std::string apply_h(std::string str_off_l = "", std::string str_off_s = "") const
	{
		if (str_off_l.empty()) str_off_l = std::to_string(this->off_l);
		if (str_off_s.empty()) str_off_s = std::to_string(this->off_s);

		auto apply_xo0 = h();
		std::string spaces = ""; for (auto i = 0; i < 2*this->n_dig+1; i++) spaces += " ";

		std::stringstream stream;
		stream << "API_polar::template "  << apply_xo0  << "<" << std::setw(this->n2_dig) << this->si_2 << ">("
		       << "s, "
		       << "   "
		       << std::setw(this->n_dig ) << str_off_s  << "+" << std::setw(this->n_dig ) << this->si_2 << ", "
		       << spaces                                                                                << "  "
		       << spaces                                                                                << "  "
		       << std::setw(this->n_dig ) << str_off_s  << "+" << std::setw(this->n_dig ) << 0          << ", " 
		       << std::setw(this->n2_dig) << this->si_2 << ");" << std::endl;

		return stream.str();
	}

	virtual int match(const int &reverse_graph_depth,
	                  const Binary_node<Pattern_SC_interface>* node_curr) const
	{
		assert(reverse_graph_depth > 0);

		const Pattern_SC_interface *pattern_left  = node_curr->get_left ()->get_contents();

		assert(pattern_left != nullptr);

		int match_val = 0;

		if (typeid(*pattern_left) == typeid(Pattern_SC<pattern_SC_type::RATE_0>))
		{
			match_val = 20;
		}

		return match_val;
	}

	virtual bool is_terminal() const { return false; }
};
}

#endif /* PATTERN_SC_YIELD0_LEFT_HPP_ */
