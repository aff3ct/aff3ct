#include <string>
#include <sstream>
#include <vector>
#include <cmath>

#include "Tools/Code/Polar/Patterns/Pattern_polar_r0.hpp"
#include "Tools/Code/Polar/Patterns/Pattern_polar_r0_left.hpp"
#include "Tools/Code/Polar/Patterns/Pattern_polar_r1.hpp"
#include "Tools/Code/Polar/Patterns/Pattern_polar_rep.hpp"
#include "Tools/Code/Polar/Patterns/Pattern_polar_rep_left.hpp"
#include "Tools/Code/Polar/Patterns/Pattern_polar_spc.hpp"
#include "Tools/Code/Polar/Patterns/Pattern_polar_std.hpp"

#include "Tools/Code/Polar/Patterns/Gen/SC/Pattern_polar_SC_r0.hpp"
#include "Tools/Code/Polar/Patterns/Gen/SC/Pattern_polar_SC_r0_left.hpp"
#include "Tools/Code/Polar/Patterns/Gen/SC/Pattern_polar_SC_r1.hpp"
#include "Tools/Code/Polar/Patterns/Gen/SC/Pattern_polar_SC_rep.hpp"
#include "Tools/Code/Polar/Patterns/Gen/SC/Pattern_polar_SC_rep_left.hpp"
#include "Tools/Code/Polar/Patterns/Gen/SC/Pattern_polar_SC_spc.hpp"
#include "Tools/Code/Polar/Patterns/Gen/SC/Pattern_polar_SC_std.hpp"

#include "Tools/Code/Polar/Patterns/Gen/SCL/Pattern_polar_SCL_r0.hpp"
#include "Tools/Code/Polar/Patterns/Gen/SCL/Pattern_polar_SCL_r0_left.hpp"
#include "Tools/Code/Polar/Patterns/Gen/SCL/Pattern_polar_SCL_r1.hpp"
#include "Tools/Code/Polar/Patterns/Gen/SCL/Pattern_polar_SCL_rep.hpp"
#include "Tools/Code/Polar/Patterns/Gen/SCL/Pattern_polar_SCL_rep_left.hpp"
#include "Tools/Code/Polar/Patterns/Gen/SCL/Pattern_polar_SCL_spc.hpp"
#include "Tools/Code/Polar/Patterns/Gen/SCL/Pattern_polar_SCL_std.hpp"

#include "Tools/Display/bash_tools.h"

#include "Tools/general_utils.h"

#include "nodes_parser.h"

using namespace aff3ct::tools;

std::vector<aff3ct::tools::Pattern_polar_i*> aff3ct::tools::nodes_parser(const std::string &str_polar,
                                                                               int         &idx_r0,
                                                                               int         &idx_r1,
                                                                         const std::string  type)
{
	idx_r0 = -1;
	idx_r1 = -1;

	auto str_polar_bis = str_polar;

	str_polar_bis.erase(std::remove(str_polar_bis.begin(), str_polar_bis.end(), ' '), str_polar_bis.end());
	str_polar_bis.erase(std::remove(str_polar_bis.begin(), str_polar_bis.end(), '{'), str_polar_bis.end());
	str_polar_bis.erase(std::remove(str_polar_bis.begin(), str_polar_bis.end(), '}'), str_polar_bis.end());
	str_polar_bis.erase(std::remove(str_polar_bis.begin(), str_polar_bis.end(), '('), str_polar_bis.end());
	str_polar_bis.erase(std::remove(str_polar_bis.begin(), str_polar_bis.end(), ')'), str_polar_bis.end());

	std::vector<aff3ct::tools::Pattern_polar_i*> polar_patterns;

	if      (type == "SC" ) polar_patterns.push_back(new Pattern_polar_SC_std );
	else if (type == "SCL") polar_patterns.push_back(new Pattern_polar_SCL_std);
	else                    polar_patterns.push_back(new Pattern_polar_std    );

	auto v_polar = string_split(str_polar_bis, ',');
	for (auto i = 0; i < (int)v_polar.size(); i++)
	{
		auto v_str1 = string_split(v_polar[i], '_');

		if (v_str1.size() >= 1)
		{
			if (v_str1[0] == "R0L")
			{
				if (v_str1.size() == 1)
				{
					if      (type == "SC" ) polar_patterns.push_back(new Pattern_polar_SC_r0_left );
					else if (type == "SCL") polar_patterns.push_back(new Pattern_polar_SCL_r0_left);
					else                    polar_patterns.push_back(new Pattern_polar_r0_left    );
				}
				else
				{
					auto v_str2 = string_split(v_str1[1], '-');

					if (v_str2.size() > 1)
					{
						auto min = (int)std::log2(std::stoi(v_str2[0]));
						auto max = (int)std::log2(std::stoi(v_str2[1]));

						if      (type == "SC" ) polar_patterns.push_back(new Pattern_polar_SC_r0_left (min, max));
						else if (type == "SCL") polar_patterns.push_back(new Pattern_polar_SCL_r0_left(min, max));
						else                    polar_patterns.push_back(new Pattern_polar_r0_left    (min, max));

					}
					else
					{
						bool plus = v_str2[0].find("+") != std::string::npos;

						auto min = (int)std::log2(std::stoi(v_str2[0]));

						if (plus)
						{
							if      (type == "SC" ) polar_patterns.push_back(new Pattern_polar_SC_r0_left (min));
							else if (type == "SCL") polar_patterns.push_back(new Pattern_polar_SCL_r0_left(min));
							else                    polar_patterns.push_back(new Pattern_polar_r0_left    (min));
						}
						else
						{

							if      (type == "SC" ) polar_patterns.push_back(new Pattern_polar_SC_r0_left (min, min));
							else if (type == "SCL") polar_patterns.push_back(new Pattern_polar_SCL_r0_left(min, min));
							else                    polar_patterns.push_back(new Pattern_polar_r0_left    (min, min));
						}
					}
				}
			}
			else if (v_str1[0] == "R0")
			{
				idx_r0 = (int)polar_patterns.size();

				if (v_str1.size() == 1)
				{
					if      (type == "SC" ) polar_patterns.push_back(new Pattern_polar_SC_r0 );
					else if (type == "SCL") polar_patterns.push_back(new Pattern_polar_SCL_r0);
					else                    polar_patterns.push_back(new Pattern_polar_r0    );
				}
				else
				{
					auto v_str2 = string_split(v_str1[1], '-');

					if (v_str2.size() > 1)
					{
						auto min = (int)std::log2(std::stoi(v_str2[0]));
						auto max = (int)std::log2(std::stoi(v_str2[1]));

						if      (type == "SC" ) polar_patterns.push_back(new Pattern_polar_SC_r0 (min, max));
						else if (type == "SCL") polar_patterns.push_back(new Pattern_polar_SCL_r0(min, max));
						else                    polar_patterns.push_back(new Pattern_polar_r0    (min, max));
					}
					else
					{
						bool plus = v_str2[0].find("+") != std::string::npos;

						auto min = (int)std::log2(std::stoi(v_str2[0]));

						if (plus)
						{
							if      (type == "SC" ) polar_patterns.push_back(new Pattern_polar_SC_r0 (min));
							else if (type == "SCL") polar_patterns.push_back(new Pattern_polar_SCL_r0(min));
							else                    polar_patterns.push_back(new Pattern_polar_r0    (min));
						}
						else
						{
							if      (type == "SC" ) polar_patterns.push_back(new Pattern_polar_SC_r0 (min, min));
							else if (type == "SCL") polar_patterns.push_back(new Pattern_polar_SCL_r0(min, min));
							else                    polar_patterns.push_back(new Pattern_polar_r0    (min, min));
						}
					}
				}
			}
			else if (v_str1[0] == "R1")
			{
				idx_r1 = (int)polar_patterns.size();

				if (v_str1.size() == 1)
				{
					if      (type == "SC" ) polar_patterns.push_back(new Pattern_polar_SC_r1 );
					else if (type == "SCL") polar_patterns.push_back(new Pattern_polar_SCL_r1);
					else                    polar_patterns.push_back(new Pattern_polar_r1    );
				}
				else
				{
					auto v_str2 = string_split(v_str1[1], '-');

					if (v_str2.size() > 1)
					{
						auto min = (int)std::log2(std::stoi(v_str2[0]));
						auto max = (int)std::log2(std::stoi(v_str2[1]));

						if      (type == "SC" ) polar_patterns.push_back(new Pattern_polar_SC_r1 (min, max));
						else if (type == "SCL") polar_patterns.push_back(new Pattern_polar_SCL_r1(min, max));
						else                    polar_patterns.push_back(new Pattern_polar_r1    (min, max));
					}
					else
					{
						bool plus = v_str2[0].find("+") != std::string::npos;

						auto min = (int)std::log2(std::stoi(v_str2[0]));

						if (plus)
						{
							if      (type == "SC" ) polar_patterns.push_back(new Pattern_polar_SC_r1 (min));
							else if (type == "SCL") polar_patterns.push_back(new Pattern_polar_SCL_r1(min));
							else                    polar_patterns.push_back(new Pattern_polar_r1    (min));
						}
						else
						{
							if      (type == "SC" ) polar_patterns.push_back(new Pattern_polar_SC_r1 (min, min));
							else if (type == "SCL") polar_patterns.push_back(new Pattern_polar_SCL_r1(min, min));
							else                    polar_patterns.push_back(new Pattern_polar_r1    (min, min));
						}
					}
				}
			}
			else if (v_str1[0] == "REPL")
			{
				if (v_str1.size() == 1)
				{
					if      (type == "SC" ) polar_patterns.push_back(new Pattern_polar_SC_rep_left );
					else if (type == "SCL") polar_patterns.push_back(new Pattern_polar_SCL_rep_left);
					else                    polar_patterns.push_back(new Pattern_polar_rep_left    );
				}
				else
				{
					auto v_str2 = string_split(v_str1[1], '-');

					if (v_str2.size() > 1)
					{
						auto min = (int)std::log2(std::stoi(v_str2[0]));
						auto max = (int)std::log2(std::stoi(v_str2[1]));

						if      (type == "SC" ) polar_patterns.push_back(new Pattern_polar_SC_rep_left (min, max));
						else if (type == "SCL") polar_patterns.push_back(new Pattern_polar_SCL_rep_left(min, max));
						else                    polar_patterns.push_back(new Pattern_polar_rep_left    (min, max));
					}
					else
					{
						bool plus = v_str2[0].find("+") != std::string::npos;

						auto min = (int)std::log2(std::stoi(v_str2[0]));

						if (plus)
						{
							if      (type == "SC" ) polar_patterns.push_back(new Pattern_polar_SC_rep_left (min));
							else if (type == "SCL") polar_patterns.push_back(new Pattern_polar_SCL_rep_left(min));
							else                    polar_patterns.push_back(new Pattern_polar_rep_left    (min));
						}
						else
						{
							if      (type == "SC" ) polar_patterns.push_back(new Pattern_polar_SC_rep_left (min, min));
							else if (type == "SCL") polar_patterns.push_back(new Pattern_polar_SCL_rep_left(min, min));
							else                    polar_patterns.push_back(new Pattern_polar_rep_left    (min, min));
						}
					}
				}
			}
			else if (v_str1[0] == "REP")
			{
				if (v_str1.size() == 1)
				{
					if      (type == "SC" ) polar_patterns.push_back(new Pattern_polar_SC_rep );
					else if (type == "SCL") polar_patterns.push_back(new Pattern_polar_SCL_rep);
					else                    polar_patterns.push_back(new Pattern_polar_rep    );
				}
				else
				{
					auto v_str2 = string_split(v_str1[1], '-');

					if (v_str2.size() > 1)
					{
						auto min = (int)std::log2(std::stoi(v_str2[0]));
						auto max = (int)std::log2(std::stoi(v_str2[1]));

						if      (type == "SC" ) polar_patterns.push_back(new Pattern_polar_SC_rep (min, max));
						else if (type == "SCL") polar_patterns.push_back(new Pattern_polar_SCL_rep(min, max));
						else                    polar_patterns.push_back(new Pattern_polar_rep    (min, max));
					}
					else
					{
						bool plus = v_str2[0].find("+") != std::string::npos;

						auto min = (int)std::log2(std::stoi(v_str2[0]));

						if (plus)
						{
							if      (type == "SC" ) polar_patterns.push_back(new Pattern_polar_SC_rep (min));
							else if (type == "SCL") polar_patterns.push_back(new Pattern_polar_SCL_rep(min));
							else                    polar_patterns.push_back(new Pattern_polar_rep    (min));
						}
						else
						{
							if      (type == "SC" ) polar_patterns.push_back(new Pattern_polar_SC_rep (min, min));
							else if (type == "SCL") polar_patterns.push_back(new Pattern_polar_SCL_rep(min, min));
							else                    polar_patterns.push_back(new Pattern_polar_rep    (min, min));
						}
					}
				}
			}
			else if (v_str1[0] == "SPC")
			{
				if (v_str1.size() == 1)
				{
					if      (type == "SC" ) polar_patterns.push_back(new Pattern_polar_SC_spc );
					else if (type == "SCL") polar_patterns.push_back(new Pattern_polar_SCL_spc);
					else                    polar_patterns.push_back(new Pattern_polar_spc    );
				}
				else
				{
					auto v_str2 = string_split(v_str1[1], '-');

					if (v_str2.size() > 1)
					{
						auto min = (int)std::log2(std::stoi(v_str2[0]));
						auto max = (int)std::log2(std::stoi(v_str2[1]));

						if      (type == "SC" ) polar_patterns.push_back(new Pattern_polar_SC_spc (min, max));
						else if (type == "SCL") polar_patterns.push_back(new Pattern_polar_SCL_spc(min, max));
						else                    polar_patterns.push_back(new Pattern_polar_spc    (min, max));
					}
					else
					{
						bool plus = v_str2[0].find("+") != std::string::npos;

						auto min = (int)std::log2(std::stoi(v_str2[0]));

						if (plus)
						{
							if      (type == "SC" ) polar_patterns.push_back(new Pattern_polar_SC_spc (min));
							else if (type == "SCL") polar_patterns.push_back(new Pattern_polar_SCL_spc(min));
							else                    polar_patterns.push_back(new Pattern_polar_spc    (min));
						}
						else
						{
							if      (type == "SC" ) polar_patterns.push_back(new Pattern_polar_SC_spc (min, min));
							else if (type == "SCL") polar_patterns.push_back(new Pattern_polar_SCL_spc(min, min));
							else                    polar_patterns.push_back(new Pattern_polar_spc    (min, min));
						}
					}
				}
			}
			else
			{
				std::clog << aff3ct::tools::bold_yellow("(WW) Unrecognized Polar node type (")
				          << aff3ct::tools::bold_yellow(v_polar[i])
				          << aff3ct::tools::bold_yellow(").")
				          << std::endl;
			}
		}
	}

	if (idx_r0 == -1)
	{
		idx_r0 = (int)polar_patterns.size();
		if      (type == "SC" ) polar_patterns.push_back(new Pattern_polar_SC_r0 (0,0));
		else if (type == "SCL") polar_patterns.push_back(new Pattern_polar_SCL_r0(0,0));
		else                    polar_patterns.push_back(new Pattern_polar_r0    (0,0));
	}

	if (idx_r1 == -1)
	{
		idx_r1 = (int)polar_patterns.size();
		if      (type == "SC" ) polar_patterns.push_back(new Pattern_polar_SC_r1 (0,0));
		else if (type == "SCL") polar_patterns.push_back(new Pattern_polar_SCL_r1(0,0));
		else                    polar_patterns.push_back(new Pattern_polar_r1    (0,0));
	}

	return polar_patterns;
}
