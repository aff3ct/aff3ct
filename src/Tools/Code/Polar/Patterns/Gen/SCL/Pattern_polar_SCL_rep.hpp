#ifndef PATTERN_POLAR_SCL_REP_HPP_
#define PATTERN_POLAR_SCL_REP_HPP_

#include "../../Pattern_polar_rep.hpp"

namespace aff3ct
{
namespace tools
{
class Pattern_polar_SCL_rep : public Pattern_polar_rep
{
protected:
	Pattern_polar_SCL_rep(const int &N, const Binary_node<Pattern_polar_i>* node,
	                      const int min_level = 1, const int max_level = -1)
	: Pattern_polar_rep(N, node, min_level, max_level)
	{
	}

public:
	Pattern_polar_SCL_rep(const int min_level = 1, const int max_level = -1)
	: Pattern_polar_rep(min_level, max_level) {}

	virtual ~Pattern_polar_SCL_rep() {}

	virtual Pattern_polar_i* alloc(const int &N, const Binary_node<Pattern_polar_i>* node) const
	{
		return new Pattern_polar_SCL_rep(N, node, this->min_level, this->max_level);
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
		stream << si << "this->template update_paths_rep<" << this->rev_depth << ", " << this->size << ">("
		             << str_off_l << ", "
		             << str_off_s << ");"
		       << std::endl;
		stream << si << "normalize_scl_metrics<R>(this->metrics, this->L);" << std::endl;

		return stream.str();
	}
};
}
}

#endif /* PATTERN_POLAR_SCL_REP_HPP_ */
