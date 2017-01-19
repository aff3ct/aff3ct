#ifndef PATTERN_POLAR_SCL_RATE_1_HPP_
#define PATTERN_POLAR_SCL_RATE_1_HPP_

#include "../../Pattern_polar_r1.hpp"

class Pattern_polar_SCL_r1 : public Pattern_polar_r1
{
protected:
	Pattern_polar_SCL_r1(const int &N, const Binary_node<Pattern_polar_i>* node)
	: Pattern_polar_r1(N, node)
	{
	}

public:
	Pattern_polar_SCL_r1() : Pattern_polar_r1() {}

	virtual ~Pattern_polar_SCL_r1() {}

	virtual Pattern_polar_i* alloc(const int &N, const Binary_node<Pattern_polar_i>* node) const
	{
		return new Pattern_polar_SCL_r1(N, node);
	}

	virtual std::string apply_f(std::string start_indent = "", std::string str_off_l = "", std::string str_off_s = "") const
	{
		return "";
	}

	virtual std::string apply_g(std::string start_indent = "", std::string str_off_l = "", std::string str_off_s = "") const
	{
		return "";
	}

	virtual std::string apply_h(std::string si = "", std::string str_off_l = "", std::string str_off_s = "") const
	{
		if (str_off_l.empty()) str_off_l = std::to_string(this->off_l - this->N);
		if (str_off_s.empty()) str_off_s = std::to_string(this->off_s          );

		std::stringstream stream;
		stream << si << "this->template update_paths_r1<" << this->rev_depth << ", " << this->size << ">("
		             << str_off_l << ", "
		             << str_off_s << ");"
		       << std::endl;

		return stream.str();
	}
};

#endif /* PATTERN_POLAR_SCL_RATE_1_HPP_ */
