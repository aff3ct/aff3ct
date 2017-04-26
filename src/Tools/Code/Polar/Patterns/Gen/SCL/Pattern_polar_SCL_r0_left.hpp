#ifndef PATTERN_POLAR_SCL_RATE_0_LEFT_HPP_
#define PATTERN_POLAR_SCL_RATE_0_LEFT_HPP_

#include "../../Pattern_polar_r0_left.hpp"

namespace aff3ct
{
namespace tools
{
class Pattern_polar_SCL_r0_left : public Pattern_polar_r0_left
{
protected:
	Pattern_polar_SCL_r0_left(const int &N, const Binary_node<Pattern_polar_i>* node,
	                          const int min_level = 1, const int max_level = -1)
	: Pattern_polar_r0_left(N, node, min_level, max_level)
	{
	}

public:
	Pattern_polar_SCL_r0_left(const int min_level = 1, const int max_level = -1)
	: Pattern_polar_r0_left(min_level, max_level) {}

	virtual ~Pattern_polar_SCL_r0_left() {}

	virtual Pattern_polar_i* alloc(const int &N, const Binary_node<Pattern_polar_i>* node) const
	{
		return new Pattern_polar_SCL_r0_left(N, node, this->min_level, this->max_level);
	}

	virtual std::string apply_f(std::string si = "", std::string str_off_l = "", std::string str_off_s = "") const
	{
		if (str_off_l.empty()) str_off_l = std::to_string(this->off_l - this->N);
		if (str_off_s.empty()) str_off_s = std::to_string(this->off_s          );

		std::stringstream stream;
		if (node->get_depth() == 0) // root node
		{
			return "";
		}
		else
		{
			stream << si << "for (auto i = 0; i < this->n_active_paths; i++) " << std::endl
			       << si << "{" << std::endl
			       << si << this->tab << "const auto path   = this->paths[i];" << std::endl
			       << si << this->tab << "const auto parent = l[this->path_2_array    [path][" << this->rev_depth << "   ]].data();" << std::endl
			       << si << this->tab << "const auto child  = l[this->up_ref_array_idx(path, " << this->rev_depth << " -1)].data();" << std::endl
			       << si << this->tab << "API_polar::template " << this->f() << "<" << this->si_2 << ">("
			                          << "parent + " << str_off_l << ", "
			                          << "parent + " << str_off_l << " + " << this->si_2 << ", "
			                          << "child + "  << str_off_l << " + " << this->size << ", "
			                          << this->si_2 << ");" << std::endl
			       << si << "}" << std::endl;
		}

		return stream.str();
	}

	virtual std::string apply_g(std::string si = "", std::string str_off_l = "", std::string str_off_s = "") const
	{
		if (str_off_l.empty()) str_off_l = std::to_string(this->off_l - this->N);
		if (str_off_s.empty()) str_off_s = std::to_string(this->off_s          );

		std::stringstream stream;
		if (node->get_depth() == 0) // root node
		{
			stream << si << "for (auto i = 0; i < this->n_active_paths; i++) " << std::endl
			       << si << "{" << std::endl
			       << si << this->tab << "const auto path  = this->paths[i];" << std::endl
			       << si << this->tab << "const auto child = l[this->up_ref_array_idx(path, " << this->rev_depth << " -1)].data();" << std::endl
			       << si << this->tab << "API_polar::template " << this->g() << "<" << this->si_2 << ">("
			                          << "y, "
			                          << "y + " << this->si_2 << ", "
			                          << "child, "
			                          << this->si_2 << ");" << std::endl
			       << si << "}" << std::endl;
		}
		else
		{
			stream << si << "for (auto i = 0; i < this->n_active_paths; i++) " << std::endl
			       << si << "{" << std::endl
			       << si << this->tab << "const auto path   = this->paths[i];" << std::endl
			       << si << this->tab << "const auto parent = l[this->path_2_array    [path][" << this->rev_depth << "   ]].data();" << std::endl
			       << si << this->tab << "const auto child  = l[this->up_ref_array_idx(path, " << this->rev_depth << " -1)].data();" << std::endl
			       << si << this->tab << "API_polar::template " << this->g() << "<" << this->si_2 << ">("
			                          << "parent + " << str_off_l << ", "
			                          << "parent + " << str_off_l << " + " << this->si_2 << ", "
			                          << "child + " << str_off_l << " + " << this->size << ", "
			                          << this->si_2 << ");" << std::endl
			       << si << "}" << std::endl;
		}

		return stream.str();
	}

	virtual std::string apply_h(std::string si = "", std::string str_off_l = "", std::string str_off_s = "") const
	{
		if (str_off_l.empty()) str_off_l = std::to_string(this->off_l - this->N);
		if (str_off_s.empty()) str_off_s = std::to_string(this->off_s          );

		std::stringstream stream;
		stream << si << "for (auto i = 0; i < this->n_active_paths; i++) " << std::endl
		       << si << "{" << std::endl
		       << si << this->tab << "API_polar::template " << this->h() << "<" << this->si_2 << ">("
		                          << "s[this->paths[i]], "
		                          << str_off_s << " + " << this->si_2 << ", "
		                          << str_off_s << ", "
		                          << this->si_2 << ");" << std::endl
		       << si << "}" << std::endl;

		return stream.str();
	}
};
}
}

#endif /* PATTERN_POLAR_SCL_RATE_0_LEFT_HPP_ */
