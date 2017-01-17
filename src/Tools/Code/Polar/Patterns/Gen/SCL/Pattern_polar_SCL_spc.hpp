#ifndef PATTERN_POLAR_SCL_SPC_HPP_
#define PATTERN_POLAR_SCL_SPC_HPP_

#include "../../Pattern_polar_spc.hpp"

class Pattern_polar_SCL_spc : public Pattern_polar_spc
{
protected:
	Pattern_polar_SCL_spc(const int &N, const Binary_node<Pattern_polar_i>* node)
	: Pattern_polar_spc(N, node)
	{
	}

public:
	Pattern_polar_SCL_spc() : Pattern_polar_spc() {}

	virtual ~Pattern_polar_SCL_spc() {}

	virtual Pattern_polar_i* alloc(const int &N, const Binary_node<Pattern_polar_i>* node) const
	{
		return new Pattern_polar_SCL_spc(N, node);
	}

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
		if (str_off_l.empty()) str_off_l = std::to_string(this->off_l);
		if (str_off_s.empty()) str_off_s = std::to_string(this->off_s);

		auto apply_spc = h();
		std::string spaces = ""; for (auto i = 0; i < 2*this->n_dig+1; i++) spaces += " ";

		std::stringstream stream;
		stream << "API_polar::template "  << apply_spc  << "<" << std::setw(this->n2_dig) << this->size << ">("
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

#endif /* PATTERN_POLAR_SCL_SPC_HPP_ */
