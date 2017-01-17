#ifndef PATTERN_POLAR_SCL_REP_HPP_
#define PATTERN_POLAR_SCL_REP_HPP_

#include "../../Pattern_polar_rep.hpp"

class Pattern_polar_SCL_rep : public Pattern_polar_rep
{
protected:
	Pattern_polar_SCL_rep(const int &N, const Binary_node<Pattern_polar_i>* node)
	: Pattern_polar_rep(N, node)
	{
	}

public:
	Pattern_polar_SCL_rep() : Pattern_polar_rep() {}

	virtual ~Pattern_polar_SCL_rep() {}

	virtual Pattern_polar_i* alloc(const int &N, const Binary_node<Pattern_polar_i>* node) const
	{
		return new Pattern_polar_SCL_rep(N, node);
	}

	virtual std::string apply_f(std::string str_off_l = "", std::string str_off_s = "") const {
		return "";
	}

	virtual std::string apply_g(std::string str_off_l = "", std::string str_off_s = "") const
	{
		return "";
	}

	virtual std::string apply_h(std::string str_off_l = "", std::string str_off_s = "") const
	{
		if (str_off_l.empty()) str_off_l = std::to_string(this->off_l);
		if (str_off_s.empty()) str_off_s = std::to_string(this->off_s);

		auto apply_rep = h();
		std::string spaces = ""; for (auto i = 0; i < 2*this->n_dig+1; i++) spaces += " ";

		std::stringstream stream;
		stream << "API_polar::template "  << apply_rep  << "<" << std::setw(this->n2_dig) << this->size << ">("
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

#endif /* PATTERN_POLAR_SCL_REP_HPP_ */
