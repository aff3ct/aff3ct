#ifndef NODES_PARSER_HPP
#define NODES_PARSER_HPP

#include <string>
#include <sstream>
#include <vector>

#include "Tools/Code/Polar/Patterns/Pattern_polar_r0.hpp"
#include "Tools/Code/Polar/Patterns/Pattern_polar_r0_left.hpp"
#include "Tools/Code/Polar/Patterns/Pattern_polar_r1.hpp"
#include "Tools/Code/Polar/Patterns/Pattern_polar_rep.hpp"
#include "Tools/Code/Polar/Patterns/Pattern_polar_rep_left.hpp"
#include "Tools/Code/Polar/Patterns/Pattern_polar_spc.hpp"
#include "Tools/Code/Polar/Patterns/Pattern_polar_std.hpp"

#include "Tools/Display/bash_tools.h"

std::vector<std::string> string_split(const std::string &s, char delim)
{
	std::stringstream ss(s);
	std::string item;
	std::vector<std::string> elems;
	while (std::getline(ss, item, delim))
		elems.push_back(std::move(item)); // if C++11 (based on comment from @mchiasson)

	return elems;
}

std::string string_replace(std::string subject, const std::string& search, const std::string& replace)
{
	if (search.empty()) return subject;

	size_t pos = 0;
	while((pos = subject.find(search, pos)) != std::string::npos)
	{
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}

	return subject;
}

std::vector<Pattern_polar_i*> nodes_parser(const std::string &str_polar, int &idx_r0, int &idx_r1)
{
	idx_r0 = -1;
	idx_r1 = -1;

	auto str_polar_bis = str_polar;

	str_polar_bis.erase(std::remove(str_polar_bis.begin(), str_polar_bis.end(), ' '), str_polar_bis.end());
	str_polar_bis.erase(std::remove(str_polar_bis.begin(), str_polar_bis.end(), '{'), str_polar_bis.end());
	str_polar_bis.erase(std::remove(str_polar_bis.begin(), str_polar_bis.end(), '}'), str_polar_bis.end());
	str_polar_bis.erase(std::remove(str_polar_bis.begin(), str_polar_bis.end(), '('), str_polar_bis.end());
	str_polar_bis.erase(std::remove(str_polar_bis.begin(), str_polar_bis.end(), ')'), str_polar_bis.end());

	std::vector<Pattern_polar_i*> polar_patterns;
	polar_patterns.push_back(new Pattern_polar_std);

	auto v_polar = string_split(str_polar_bis, ',');
	for (auto i = 0; i < (int)v_polar.size(); i++)
	{
		if (v_polar[i].find("R0L") != std::string::npos)
		{
			polar_patterns.push_back(new Pattern_polar_r0_left);
		}
		else if (v_polar[i].find("R0") != std::string::npos)
		{
			idx_r0 = (int)polar_patterns.size();

			auto search = "R0";
			auto replace = "";
			auto str = string_replace(v_polar[i], search, replace);

			if (str.empty())
			{
				polar_patterns.push_back(new Pattern_polar_r0);
			}
			else
			{
				auto v_str = string_split(str, '-');

				if (v_str.size() > 1)
				{
					auto min = (int)std::log2(std::stoi(v_str[0]));
					auto max = (int)std::log2(std::stoi(v_str[1]));

					polar_patterns.push_back(new Pattern_polar_r0(min, max));
				}
				else
				{
					bool plus = v_str[0].find("+") != std::string::npos;

					auto min = (int)std::log2(std::stoi(v_str[0]));

					if (plus)
						polar_patterns.push_back(new Pattern_polar_r0(min));
					else
						polar_patterns.push_back(new Pattern_polar_r0(min, min));
				}
			}
		}
		else if (v_polar[i].find("R1") != std::string::npos)
		{
			idx_r1 = (int)polar_patterns.size();

			auto search = "R1";
			auto replace = "";
			auto str = string_replace(v_polar[i], search, replace);

			if (str.empty())
			{
				polar_patterns.push_back(new Pattern_polar_r1);
			}
			else
			{
				auto v_str = string_split(str, '-');

				if (v_str.size() > 1)
				{
					auto min = (int)std::log2(std::stoi(v_str[0]));
					auto max = (int)std::log2(std::stoi(v_str[1]));

					polar_patterns.push_back(new Pattern_polar_r1(min, max));
				}
				else
				{
					bool plus = v_str[0].find("+") != std::string::npos;

					auto min = (int)std::log2(std::stoi(v_str[0]));

					if (plus)
						polar_patterns.push_back(new Pattern_polar_r1(min));
					else
						polar_patterns.push_back(new Pattern_polar_r1(min, min));
				}
			}
		}
		else if (v_polar[i].find("REPL") != std::string::npos)
		{
			polar_patterns.push_back(new Pattern_polar_rep_left);
		}
		else if (v_polar[i].find("REP") != std::string::npos)
		{
			auto search = "REP";
			auto replace = "";
			auto str = string_replace(v_polar[i], search, replace);

			if (str.empty())
			{
				polar_patterns.push_back(new Pattern_polar_rep);
			}
			else
			{
				auto v_str = string_split(str, '-');

				if (v_str.size() > 1)
				{
					auto min = (int)std::log2(std::stoi(v_str[0]));
					auto max = (int)std::log2(std::stoi(v_str[1]));

					polar_patterns.push_back(new Pattern_polar_rep(min, max));
				}
				else
				{
					bool plus = v_str[0].find("+") != std::string::npos;

					auto min = (int)std::log2(std::stoi(v_str[0]));

					if (plus)
						polar_patterns.push_back(new Pattern_polar_rep(min));
					else
						polar_patterns.push_back(new Pattern_polar_rep(min, min));
				}
			}
		}
		else if (v_polar[i].find("SPC") != std::string::npos)
		{
			auto search = "SPC";
			auto replace = "";
			auto str = string_replace(v_polar[i], search, replace);

			if (str.empty())
			{
				polar_patterns.push_back(new Pattern_polar_spc);
			}
			else
			{
				auto v_str = string_split(str, '-');

				if (v_str.size() > 1)
				{
					auto min = (int)std::log2(std::stoi(v_str[0]));
					auto max = (int)std::log2(std::stoi(v_str[1]));

					polar_patterns.push_back(new Pattern_polar_spc(min, max));
				}
				else
				{
					bool plus = v_str[0].find("+") != std::string::npos;

					auto min = (int)std::log2(std::stoi(v_str[0]));

					if (plus)
						polar_patterns.push_back(new Pattern_polar_spc(min));
					else
						polar_patterns.push_back(new Pattern_polar_spc(min, min));
				}
			}
		}
		else
		{
			std::clog << bold_yellow("(WW) Unrecognized Polar node type (")
			          << bold_yellow(v_polar[i])
			          << bold_yellow(")")
			          << std::endl;
		}
	}

	if (idx_r0 == -1)
	{
		idx_r0 = (int)polar_patterns.size();
		polar_patterns.push_back(new Pattern_polar_r0(0,0));
	}

	if (idx_r1 == -1)
	{
		idx_r1 = (int)polar_patterns.size();
		polar_patterns.push_back(new Pattern_polar_r1(0,0));
	}

	return polar_patterns;
}

#endif /* PATTERN_POLAR_PARSER_HPP */
