#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <algorithm>
#include <iostream>
#include <cmath>

#include "Tools/Display/rang_format/rang_format.h"
#include "Tools/general_utils.h"
#include "Tools/Code/Polar/Nodes_parser.hpp"

namespace aff3ct
{
namespace tools
{

template <class R0, class R0L, class R1, class REP, class REPL, class SPC, class STD>
void Nodes_parser<R0,R0L,R1,REP,REPL,SPC,STD>
::push_back_polar_pattern(std::vector<Pattern_polar_i*> *polar_patterns_ptr,
                          std::vector<std::unique_ptr<Pattern_polar_i>> *polar_patterns_uptr,
                          Pattern_polar_i* polar_pattern)
{
	if (polar_patterns_ptr != nullptr)
		polar_patterns_ptr->push_back(polar_pattern);
	if (polar_patterns_uptr != nullptr)
		polar_patterns_uptr->push_back(std::unique_ptr<Pattern_polar_i>(polar_pattern));

	if (polar_patterns_ptr == nullptr && polar_patterns_uptr == nullptr)
		delete polar_pattern;
}

template <class R0, class R0L, class R1, class REP, class REPL, class SPC, class STD>
void Nodes_parser<R0,R0L,R1,REP,REPL,SPC,STD>
::parse(const std::string &str_polar, int &idx_r0, int &idx_r1,
        std::vector<Pattern_polar_i*> *polar_patterns_ptr,
        std::vector<std::unique_ptr<Pattern_polar_i>> *polar_patterns_uptr)
{
	using namespace aff3ct::tools;
	idx_r0 = -1;
	idx_r1 = -1;

	auto str_polar_bis = str_polar;

	str_polar_bis.erase(std::remove(str_polar_bis.begin(), str_polar_bis.end(), ' '), str_polar_bis.end());
	str_polar_bis.erase(std::remove(str_polar_bis.begin(), str_polar_bis.end(), '{'), str_polar_bis.end());
	str_polar_bis.erase(std::remove(str_polar_bis.begin(), str_polar_bis.end(), '}'), str_polar_bis.end());
	str_polar_bis.erase(std::remove(str_polar_bis.begin(), str_polar_bis.end(), '('), str_polar_bis.end());
	str_polar_bis.erase(std::remove(str_polar_bis.begin(), str_polar_bis.end(), ')'), str_polar_bis.end());

	push_back_polar_pattern(polar_patterns_ptr, polar_patterns_uptr, new STD);

	auto v_polar = split(str_polar_bis, ',');
	for (auto i = 0; i < (int)v_polar.size(); i++)
	{
		auto v_str1 = split(v_polar[i], '_');

		if (v_str1.size() >= 1)
		{
			if (v_str1[0] == "R0L")
			{
				if (v_str1.size() == 1)
					push_back_polar_pattern(polar_patterns_ptr, polar_patterns_uptr, new R0L);
				else
				{
					auto v_str2 = split(v_str1[1], '-');

					if (v_str2.size() > 1)
					{
						auto min = (int)std::log2(std::stoi(v_str2[0]));
						auto max = (int)std::log2(std::stoi(v_str2[1]));

						push_back_polar_pattern(polar_patterns_ptr, polar_patterns_uptr, new R0L(min, max));
					}
					else
					{
						bool plus = v_str2[0].find("+") != std::string::npos;

						auto min = (int)std::log2(std::stoi(v_str2[0]));

						if (plus) push_back_polar_pattern(polar_patterns_ptr, polar_patterns_uptr, new R0L(min     ));
						else      push_back_polar_pattern(polar_patterns_ptr, polar_patterns_uptr, new R0L(min, min));
					}
				}
			}
			else if (v_str1[0] == "R0")
			{
				if      (polar_patterns_uptr != nullptr) idx_r0 = (int)polar_patterns_uptr->size();
				else if (polar_patterns_ptr  != nullptr) idx_r0 = (int)polar_patterns_ptr ->size();

				if (v_str1.size() == 1)
					push_back_polar_pattern(polar_patterns_ptr, polar_patterns_uptr, new R0);
				else
				{
					auto v_str2 = split(v_str1[1], '-');

					if (v_str2.size() > 1)
					{
						auto min = (int)std::log2(std::stoi(v_str2[0]));
						auto max = (int)std::log2(std::stoi(v_str2[1]));

						push_back_polar_pattern(polar_patterns_ptr, polar_patterns_uptr, new R0(min, max));
					}
					else
					{
						bool plus = v_str2[0].find("+") != std::string::npos;

						auto min = (int)std::log2(std::stoi(v_str2[0]));

						if (plus) push_back_polar_pattern(polar_patterns_ptr, polar_patterns_uptr, new R0(min     ));
						else      push_back_polar_pattern(polar_patterns_ptr, polar_patterns_uptr, new R0(min, min));
					}
				}
			}
			else if (v_str1[0] == "R1")
			{
				if      (polar_patterns_uptr != nullptr) idx_r1 = (int)polar_patterns_uptr->size();
				else if (polar_patterns_ptr  != nullptr) idx_r1 = (int)polar_patterns_ptr ->size();

				if (v_str1.size() == 1)
					push_back_polar_pattern(polar_patterns_ptr, polar_patterns_uptr, new R1);
				else
				{
					auto v_str2 = split(v_str1[1], '-');

					if (v_str2.size() > 1)
					{
						auto min = (int)std::log2(std::stoi(v_str2[0]));
						auto max = (int)std::log2(std::stoi(v_str2[1]));

						push_back_polar_pattern(polar_patterns_ptr, polar_patterns_uptr, new R1(min, max));
					}
					else
					{
						bool plus = v_str2[0].find("+") != std::string::npos;

						auto min = (int)std::log2(std::stoi(v_str2[0]));

						if (plus) push_back_polar_pattern(polar_patterns_ptr, polar_patterns_uptr, new R1(min     ));
						else      push_back_polar_pattern(polar_patterns_ptr, polar_patterns_uptr, new R1(min, min));
					}
				}
			}
			else if (v_str1[0] == "REPL")
			{
				if (v_str1.size() == 1)
					push_back_polar_pattern(polar_patterns_ptr, polar_patterns_uptr, new REPL);
				else
				{
					auto v_str2 = split(v_str1[1], '-');

					if (v_str2.size() > 1)
					{
						auto min = (int)std::log2(std::stoi(v_str2[0]));
						auto max = (int)std::log2(std::stoi(v_str2[1]));

						push_back_polar_pattern(polar_patterns_ptr, polar_patterns_uptr, new REPL(min, max));
					}
					else
					{
						bool plus = v_str2[0].find("+") != std::string::npos;

						auto min = (int)std::log2(std::stoi(v_str2[0]));

						if (plus) push_back_polar_pattern(polar_patterns_ptr, polar_patterns_uptr, new REPL(min     ));
						else      push_back_polar_pattern(polar_patterns_ptr, polar_patterns_uptr, new REPL(min, min));
					}
				}
			}
			else if (v_str1[0] == "REP")
			{
				if (v_str1.size() == 1)
					push_back_polar_pattern(polar_patterns_ptr, polar_patterns_uptr, new REP);
				else
				{
					auto v_str2 = split(v_str1[1], '-');

					if (v_str2.size() > 1)
					{
						auto min = (int)std::log2(std::stoi(v_str2[0]));
						auto max = (int)std::log2(std::stoi(v_str2[1]));

						push_back_polar_pattern(polar_patterns_ptr, polar_patterns_uptr, new REP(min, max));
					}
					else
					{
						bool plus = v_str2[0].find("+") != std::string::npos;

						auto min = (int)std::log2(std::stoi(v_str2[0]));

						if (plus) push_back_polar_pattern(polar_patterns_ptr, polar_patterns_uptr, new REP(min     ));
						else      push_back_polar_pattern(polar_patterns_ptr, polar_patterns_uptr, new REP(min, min));
					}
				}
			}
			else if (v_str1[0] == "SPC")
			{
				if (v_str1.size() == 1)
					push_back_polar_pattern(polar_patterns_ptr, polar_patterns_uptr, new SPC);
				else
				{
					auto v_str2 = split(v_str1[1], '-');

					if (v_str2.size() > 1)
					{
						auto min = (int)std::log2(std::stoi(v_str2[0]));
						auto max = (int)std::log2(std::stoi(v_str2[1]));

						push_back_polar_pattern(polar_patterns_ptr, polar_patterns_uptr, new SPC(min, max));
					}
					else
					{
						bool plus = v_str2[0].find("+") != std::string::npos;

						auto min = (int)std::log2(std::stoi(v_str2[0]));

						if (plus) push_back_polar_pattern(polar_patterns_ptr, polar_patterns_uptr, new SPC(min     ));
						else      push_back_polar_pattern(polar_patterns_ptr, polar_patterns_uptr, new SPC(min, min));
					}
				}
			}
			else
			{
				std::clog << rang::tag::warning << "Unrecognized Polar node type (" << v_polar[i] << ")." << std::endl;
			}
		}
	}

	if (idx_r0 == -1)
	{
		if      (polar_patterns_uptr != nullptr) idx_r0 = (int)polar_patterns_uptr->size();
		else if (polar_patterns_ptr  != nullptr) idx_r0 = (int)polar_patterns_ptr ->size();

		push_back_polar_pattern(polar_patterns_ptr, polar_patterns_uptr, new R0(0,0));
	}

	if (idx_r1 == -1)
	{
		if      (polar_patterns_uptr != nullptr) idx_r1 = (int)polar_patterns_uptr->size();
		else if (polar_patterns_ptr  != nullptr) idx_r1 = (int)polar_patterns_ptr ->size();

		push_back_polar_pattern(polar_patterns_ptr, polar_patterns_uptr, new R1(0,0));
	}
}


template <class R0, class R0L, class R1, class REP, class REPL, class SPC, class STD>
std::vector<std::unique_ptr<Pattern_polar_i>> Nodes_parser<R0,R0L,R1,REP,REPL,SPC,STD>
::parse_uptr(const std::string &str_polar, int &idx_r0, int &idx_r1)
{
	std::vector<std::unique_ptr<Pattern_polar_i>> polar_patterns_uptr;
	Nodes_parser::parse(str_polar, idx_r0, idx_r1, nullptr, &polar_patterns_uptr);
	return polar_patterns_uptr;
}

template <class R0, class R0L, class R1, class REP, class REPL, class SPC, class STD>
std::vector<Pattern_polar_i*> Nodes_parser<R0,R0L,R1,REP,REPL,SPC,STD>
::parse_ptr(const std::string &str_polar, int &idx_r0, int &idx_r1)
{
	std::vector<Pattern_polar_i*> polar_patterns_ptr;
	Nodes_parser::parse(str_polar, idx_r0, idx_r1, &polar_patterns_ptr, nullptr);
	return polar_patterns_ptr;
}

}
}