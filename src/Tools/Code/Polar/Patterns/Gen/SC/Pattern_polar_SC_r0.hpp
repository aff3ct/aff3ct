#ifndef PATTERN_POLAR_SC_RATE_0_HPP_
#define PATTERN_POLAR_SC_RATE_0_HPP_

#include "../../Pattern_polar_r0.hpp"

class Pattern_polar_SC_r0 : public Pattern_polar_r0
{
protected:
	Pattern_polar_SC_r0(const int &N, const Binary_node<Pattern_polar_i>* node)
	: Pattern_polar_r0(N, node)
	{
	}

public:
	Pattern_polar_SC_r0() : Pattern_polar_r0() {}

	virtual ~Pattern_polar_SC_r0() {}

	virtual Pattern_polar_i* alloc(const int &N, const Binary_node<Pattern_polar_i>* node) const
	{
		return new Pattern_polar_SC_r0(N, node);
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
		return "";
	}
};

#endif /* PATTERN_POLAR_SC_RATE_0_HPP_ */
