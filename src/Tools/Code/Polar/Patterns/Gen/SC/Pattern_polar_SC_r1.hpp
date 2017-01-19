#ifndef PATTERN_POLAR_SC_RATE_1_HPP_
#define PATTERN_POLAR_SC_RATE_1_HPP_

#include "../../Pattern_polar_r1.hpp"

class Pattern_polar_SC_r1 : public Pattern_polar_r1
{
protected:
	Pattern_polar_SC_r1(const int &N, const Binary_node<Pattern_polar_i>* node) : Pattern_polar_r1(N, node)
	{
	}

public:
	Pattern_polar_SC_r1() : Pattern_polar_r1() {}

	virtual ~Pattern_polar_SC_r1() {}

	virtual Pattern_polar_i* alloc(const int &N, const Binary_node<Pattern_polar_i>* node) const
	{
		return new Pattern_polar_SC_r1(N, node);
	}

	virtual std::string apply_f(std::string start_indent = "", std::string str_off_l = "", std::string str_off_s = "") const
	{
		return "";
	}

	virtual std::string apply_g(std::string start_indent = "", std::string str_off_l = "", std::string str_off_s = "") const
	{
		return "";
	}

	virtual std::string apply_h(std::string start_indent = "", std::string str_off_l = "", std::string str_off_s = "") const
	{
		if (str_off_l.empty()) str_off_l = std::to_string(this->off_l);
		if (str_off_s.empty()) str_off_s = std::to_string(this->off_s);

		auto apply_h = h() + "  ";
		std::string spaces = ""; for (auto i = 0; i < 2*this->n_dig+1; i++) spaces += " ";

		std::stringstream stream;
		stream << "API_polar::template "  << apply_h    << "<" << std::setw(this->n2_dig) << this->size << ">("
		       << "s, "
		       << "l, "
		       << std::setw(this->n_dig ) << str_off_l  << "+" << std::setw(this->n_dig ) << 0          << ", "
		       << spaces                                                                                << "  "
		       << spaces                                                                                << "  "
		       << std::setw(this->n_dig ) << str_off_s  << "+" << std::setw(this->n_dig ) << 0          << ", "
		       << std::setw(this->n2_dig) << this->size << ");" << std::endl;

		return stream.str();
	}
};

#endif /* PATTERN_POLAR_SC_RATE_1_HPP_ */
