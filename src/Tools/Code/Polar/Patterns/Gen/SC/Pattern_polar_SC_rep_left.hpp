#ifndef PATTERN_POLAR_SC_REP_LEFT_HPP_
#define PATTERN_POLAR_SC_REP_LEFT_HPP_

#include "../../Pattern_polar_rep_left.hpp"

class Pattern_polar_SC_rep_left : public Pattern_polar_rep_left
{
protected:
	Pattern_polar_SC_rep_left(const int &N, const Binary_node<Pattern_polar_i>* node)
	: Pattern_polar_rep_left(N, node)
	{
	}

public:
	Pattern_polar_SC_rep_left() : Pattern_polar_rep_left() { }

	virtual ~Pattern_polar_SC_rep_left() {}

	virtual Pattern_polar_i* alloc(const int &N, const Binary_node<Pattern_polar_i>* node) const
	{
		return new Pattern_polar_SC_rep_left(N, node);
	}

	virtual std::string apply_f(std::string start_indent = "", std::string str_off_l = "", std::string str_off_s = "") const
	{
		// using namespace std;
		if (str_off_l.empty()) str_off_l = std::to_string(this->off_l);
		if (str_off_s.empty()) str_off_s = std::to_string(this->off_s);

		auto apply_f = f() + "  ";
		std::string spaces = ""; for (auto i = 0; i < 2*this->n_dig+1; i++) spaces += " ";

		std::stringstream stream;
		stream << "API_polar::template "  << apply_f    << "<" << std::setw(this->n2_dig) << this->si_2 << ">("
		       << "   "
		       << "l, "
		       << std::setw(this->n_dig ) << str_off_l  << "+" << std::setw(this->n_dig ) << 0          << ", "
		       << std::setw(this->n_dig ) << str_off_l  << "+" << std::setw(this->n_dig ) << this->si_2 << ", "
		       << spaces                                                                                << "  "
		       << std::setw(this->n_dig ) << str_off_l  << "+" << std::setw(this->n_dig ) << this->size << ", "
		       << std::setw(this->n2_dig) << this->si_2 << ");" << std::endl;

		return stream.str();
	}

	virtual std::string apply_g(std::string start_indent = "", std::string str_off_l = "", std::string str_off_s = "") const
	{
		if (str_off_l.empty()) str_off_l = std::to_string(this->off_l);
		if (str_off_s.empty()) str_off_s = std::to_string(this->off_s);

		auto apply_gr = g() + " ";

		std::stringstream stream;
		stream << "API_polar::template "  << apply_gr   << "<" << std::setw(this->n2_dig) << this->si_2 << ">("
		       << "s, "
		       << "l, "
		       << std::setw(this->n_dig ) << str_off_l  << "+" << std::setw(this->n_dig ) << 0          << ", "
		       << std::setw(this->n_dig ) << str_off_l  << "+" << std::setw(this->n_dig ) << this->si_2 << ", "
		       << std::setw(this->n_dig ) << str_off_s  << "+" << std::setw(this->n_dig ) << 0          << ", "
		       << std::setw(this->n_dig ) << str_off_l  << "+" << std::setw(this->n_dig ) << this->size << ", "
		       << std::setw(this->n2_dig) << this->si_2 << ");" << std::endl;

		return stream.str();
	}

	virtual std::string apply_h(std::string start_indent = "", std::string str_off_l = "", std::string str_off_s = "") const
	{
		if (str_off_l.empty()) str_off_l = std::to_string(this->off_l);
		if (str_off_s.empty()) str_off_s = std::to_string(this->off_s);

		auto apply_xo = h() + " ";
		std::string spaces = ""; for (auto i = 0; i < 2*this->n_dig+1; i++) spaces += " ";

		std::stringstream stream;
		stream << "API_polar::template "  << apply_xo   << "<" << std::setw(this->n2_dig) << this->si_2 << ">("
		       << "s, "
		       << "   "
		       << std::setw(this->n_dig ) << str_off_s  << "+" << std::setw(this->n_dig ) << 0          << ", "
		       << std::setw(this->n_dig ) << str_off_s  << "+" << std::setw(this->n_dig ) << this->si_2 << ", "
		       << spaces                                                                                << "  "
		       << std::setw(this->n_dig ) << str_off_s  << "+" << std::setw(this->n_dig ) << 0          << ", "
		       << std::setw(this->n2_dig) << this->si_2 << ");" << std::endl;

		return stream.str();
	}
};

#endif /* PATTERN_POLAR_SC_REP_LEFT_HPP_ */
