#include <string>
#include <sstream>
#include <vector>
#include <cmath>

#include "Tools/Display/rang_format/rang_format.h"

#include "Tools/general_utils.h"

#include "nodes_parser.h"

template <class R0, class R0L, class R1, class REP, class REPL, class SPC, class STD>
std::vector<aff3ct::tools::Pattern_polar_i*> aff3ct::tools
::nodes_parser(const std::string &str_polar, int &idx_r0, int &idx_r1)
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

	polar_patterns.push_back(new STD);

	auto v_polar = split(str_polar_bis, ',');
	for (auto i = 0; i < (int)v_polar.size(); i++)
	{
		auto v_str1 = split(v_polar[i], '_');

		if (v_str1.size() >= 1)
		{
			if (v_str1[0] == "R0L")
			{
				if (v_str1.size() == 1)
					polar_patterns.push_back(new R0L);
				else
				{
					auto v_str2 = split(v_str1[1], '-');

					if (v_str2.size() > 1)
					{
						auto min = (int)std::log2(std::stoi(v_str2[0]));
						auto max = (int)std::log2(std::stoi(v_str2[1]));

						polar_patterns.push_back(new R0L(min, max));
					}
					else
					{
						bool plus = v_str2[0].find("+") != std::string::npos;

						auto min = (int)std::log2(std::stoi(v_str2[0]));

						if (plus) polar_patterns.push_back(new R0L(min     ));
						else      polar_patterns.push_back(new R0L(min, min));
					}
				}
			}
			else if (v_str1[0] == "R0")
			{
				idx_r0 = (int)polar_patterns.size();

				if (v_str1.size() == 1)
					polar_patterns.push_back(new R0);
				else
				{
					auto v_str2 = split(v_str1[1], '-');

					if (v_str2.size() > 1)
					{
						auto min = (int)std::log2(std::stoi(v_str2[0]));
						auto max = (int)std::log2(std::stoi(v_str2[1]));

						polar_patterns.push_back(new R0(min, max));
					}
					else
					{
						bool plus = v_str2[0].find("+") != std::string::npos;

						auto min = (int)std::log2(std::stoi(v_str2[0]));

						if (plus) polar_patterns.push_back(new R0(min     ));
						else      polar_patterns.push_back(new R0(min, min));
					}
				}
			}
			else if (v_str1[0] == "R1")
			{
				idx_r1 = (int)polar_patterns.size();

				if (v_str1.size() == 1)
					polar_patterns.push_back(new R1);
				else
				{
					auto v_str2 = split(v_str1[1], '-');

					if (v_str2.size() > 1)
					{
						auto min = (int)std::log2(std::stoi(v_str2[0]));
						auto max = (int)std::log2(std::stoi(v_str2[1]));

						polar_patterns.push_back(new R1(min, max));
					}
					else
					{
						bool plus = v_str2[0].find("+") != std::string::npos;

						auto min = (int)std::log2(std::stoi(v_str2[0]));

						if (plus) polar_patterns.push_back(new R1(min     ));
						else      polar_patterns.push_back(new R1(min, min));
					}
				}
			}
			else if (v_str1[0] == "REPL")
			{
				if (v_str1.size() == 1)
					polar_patterns.push_back(new REPL);
				else
				{
					auto v_str2 = split(v_str1[1], '-');

					if (v_str2.size() > 1)
					{
						auto min = (int)std::log2(std::stoi(v_str2[0]));
						auto max = (int)std::log2(std::stoi(v_str2[1]));

						polar_patterns.push_back(new REPL(min, max));
					}
					else
					{
						bool plus = v_str2[0].find("+") != std::string::npos;

						auto min = (int)std::log2(std::stoi(v_str2[0]));

						if (plus) polar_patterns.push_back(new REPL(min     ));
						else      polar_patterns.push_back(new REPL(min, min));
					}
				}
			}
			else if (v_str1[0] == "REP")
			{
				if (v_str1.size() == 1)
					polar_patterns.push_back(new REP);
				else
				{
					auto v_str2 = split(v_str1[1], '-');

					if (v_str2.size() > 1)
					{
						auto min = (int)std::log2(std::stoi(v_str2[0]));
						auto max = (int)std::log2(std::stoi(v_str2[1]));

						polar_patterns.push_back(new REP(min, max));
					}
					else
					{
						bool plus = v_str2[0].find("+") != std::string::npos;

						auto min = (int)std::log2(std::stoi(v_str2[0]));

						if (plus) polar_patterns.push_back(new REP(min     ));
						else      polar_patterns.push_back(new REP(min, min));
					}
				}
			}
			else if (v_str1[0] == "SPC")
			{
				if (v_str1.size() == 1)
					polar_patterns.push_back(new SPC);
				else
				{
					auto v_str2 = split(v_str1[1], '-');

					if (v_str2.size() > 1)
					{
						auto min = (int)std::log2(std::stoi(v_str2[0]));
						auto max = (int)std::log2(std::stoi(v_str2[1]));

						polar_patterns.push_back(new SPC(min, max));
					}
					else
					{
						bool plus = v_str2[0].find("+") != std::string::npos;

						auto min = (int)std::log2(std::stoi(v_str2[0]));

						if (plus) polar_patterns.push_back(new SPC(min     ));
						else      polar_patterns.push_back(new SPC(min, min));
					}
				}
			}
			else
			{
				std::clog << rang::format::warning << "Unrecognized Polar node type (" << v_polar[i] << ")."
				          << rang::format::reset << std::endl;
			}
		}
	}

	if (idx_r0 == -1)
	{
		idx_r0 = (int)polar_patterns.size();
		polar_patterns.push_back(new R0(0,0));
	}

	if (idx_r1 == -1)
	{
		idx_r1 = (int)polar_patterns.size();
		polar_patterns.push_back(new R1(0,0));
	}

	return polar_patterns;
}
