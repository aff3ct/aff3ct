#ifndef PATTERN_POLAR_SC_REP_HPP_
#define PATTERN_POLAR_SC_REP_HPP_

#include "../../Pattern_polar_rep.hpp"

class Pattern_polar_SC_rep : public Pattern_polar_rep
{
protected:
	Pattern_polar_SC_rep(const int &N, const Binary_node<Pattern_polar_i>* node,
	                     const int min_level = -1, const int max_level = -1)
	: Pattern_polar_rep(N, node, min_level, max_level)
	{
	}

public:
	Pattern_polar_SC_rep(const int min_level = -1, const int max_level = -1)
	: Pattern_polar_rep(min_level, max_level) {}

	virtual ~Pattern_polar_SC_rep() {}

	virtual Pattern_polar_i* alloc(const int &N, const Binary_node<Pattern_polar_i>* node) const
	{
		return new Pattern_polar_SC_rep(N, node, this->min_level, this->max_level);
	}

	virtual std::string apply_f(std::string start_indent = "", std::string str_off_l = "", std::string str_off_s = "") const {
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

#endif /* PATTERN_POLAR_SC_REP_HPP_ */
