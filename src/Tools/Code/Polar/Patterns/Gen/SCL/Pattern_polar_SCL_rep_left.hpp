#ifndef PATTERN_POLAR_SCL_REP_LEFT_HPP_
#define PATTERN_POLAR_SCL_REP_LEFT_HPP_

#include "../../Pattern_polar_rep_left.hpp"

class Pattern_polar_SCL_rep_left : public Pattern_polar_rep_left
{
protected:
	Pattern_polar_SCL_rep_left(const int &N, const Binary_node<Pattern_polar_i>* node)
	: Pattern_polar_rep_left(N, node)
	{
	}

public:
	Pattern_polar_SCL_rep_left() : Pattern_polar_rep_left() { }

	virtual ~Pattern_polar_SCL_rep_left() {}

	virtual Pattern_polar_i* alloc(const int &N, const Binary_node<Pattern_polar_i>* node) const
	{
		return new Pattern_polar_SCL_rep_left(N, node);
	}

	virtual std::string apply_f(std::string si = "", std::string str_off_l = "", std::string str_off_s = "") const
	{
		if (str_off_l.empty()) str_off_l = std::to_string(this->off_l - this->N);
		if (str_off_s.empty()) str_off_s = std::to_string(this->off_s          );

		std::stringstream stream;
		if (node->get_depth() == 0) // root node
		{
			stream << si << "API_polar::template " << this->f() << "<" << this->si_2 << ">("
			             << "y.data(), "
			             << "y.data() + " << this->si_2 << ", "
			             << "l[0].data(), "
			             << this->si_2 << ");" << std::endl;
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
			                          << "y.data(), "
			                          << "y.data() + " << this->si_2 << ", "
			                          << "s[path].data() + " << str_off_s << ", "
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
			                          << "s[path].data() + " << str_off_s << ", "
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
		                          << str_off_s << ", "
		                          << str_off_s << " + " << this->si_2 << ", "
		                          << str_off_s << ", "
		                          << this->si_2 << ");" << std::endl
		       << si << "}" << std::endl;

		return stream.str();
	}
};

#endif /* PATTERN_POLAR_SCL_REP_LEFT_HPP_ */
