#ifndef PATTERN_POLAR_RATE_0_LEFT_HPP_
#define PATTERN_POLAR_RATE_0_LEFT_HPP_

#include <cassert>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <typeinfo>

#include "Pattern_polar_std.hpp"
#include "Pattern_polar_r0.hpp"

template <>
class Pattern_polar<polar_node_t::RATE_0_LEFT> : public Pattern_polar_i
{
protected:
	Pattern_polar(const int &N, const Binary_node<Pattern_polar_i>* node) : Pattern_polar_i(N, node)
	{
	}

public:
	Pattern_polar() : Pattern_polar_i() { }

	virtual Pattern_polar_i* alloc(const int &N, const Binary_node<Pattern_polar_i>* node) const
	{
		return new Pattern_polar<polar_node_t::RATE_0_LEFT>(N, node);
	}

	virtual ~Pattern_polar() {}

	virtual polar_node_t type()       const { return RATE_0_LEFT;   }
	virtual std::string  name()       const { return "Rate 0 left"; }
	virtual std::string  short_name() const { return "r0l";         }
	virtual std::string  fill_color() const { return "#dadada";     }
	virtual std::string  font_color() const { return "#000000";     }

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

	virtual int match(const int &reverse_graph_depth, const Binary_node<Pattern_polar_i>* node_curr) const
	{
		assert(reverse_graph_depth > 0);

		const Pattern_polar_i *pattern_left  = node_curr->get_left ()->get_contents();

		assert(pattern_left != nullptr);

		int match_val = 0;

		if (typeid(*pattern_left) == typeid(Pattern_polar<polar_node_t::RATE_0>))
		{
			match_val = 20;
		}

		return match_val;
	}

	virtual bool is_terminal() const { return false; }
};

#endif /* PATTERN_POLAR_RATE_0_LEFT_HPP_ */
