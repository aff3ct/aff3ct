#include "bash_tools.h"

bool aff3ct::tools::enable_bash_tools = true;

std::string aff3ct::tools::bold(std::string str)
{
#ifndef ENABLE_COOL_BASH
	return str;
#else
	if (enable_bash_tools)
		return "\e[1m" + str + "\e[0m";
	else
		return str;
#endif
}

std::string aff3ct::tools::italic(std::string str)
{
#ifndef ENABLE_COOL_BASH
	return str;
#else
	if (enable_bash_tools)
		return "\e[3m" + str + "\e[0m";
	else
		return str;
#endif
}

std::string aff3ct::tools::bold_italic(std::string str)
{
#ifndef ENABLE_COOL_BASH
	return str;
#else
	if (enable_bash_tools)
		return "\e[1m\e[3m" + str + "\e[0m";
	else
		return str;
#endif
}

std::string aff3ct::tools::underlined(std::string str)
{
#ifndef ENABLE_COOL_BASH
	return str;
#else
	if (enable_bash_tools)
		return "\e[4m" + str + "\e[0m";
	else
		return str;
#endif
}

std::string aff3ct::tools::bold_underlined(std::string str)
{
#ifndef ENABLE_COOL_BASH
	return str;
#else
	if (enable_bash_tools)
		return "\e[1m\e[4m" + str + "\e[0m";
	else
		return str;
#endif
}

std::string aff3ct::tools::inverted(std::string str)
{
#ifndef ENABLE_COOL_BASH
	return str;
#else
	if (enable_bash_tools)
		return "\e[7m" + str + "\e[0m";
	else
		return str;
#endif
}

std::string aff3ct::tools::red(std::string str)
{
#ifndef ENABLE_COOL_BASH
	return str;
#else
	if (enable_bash_tools)
		return "\e[31m" + str + "\e[39m";
	else
		return str;
#endif
}

std::string aff3ct::tools::bold_red(std::string str)
{
#ifndef ENABLE_COOL_BASH
	return str;
#else
	if (enable_bash_tools)
		return "\e[1m\e[31m" + str + "\e[39m\e[0m";
	else
		return str;
#endif
}

std::string aff3ct::tools::green(std::string str)
{
#ifndef ENABLE_COOL_BASH
	return str;
#else
	if (enable_bash_tools)
		return "\e[32m" + str + "\e[39m";
	else
		return str;
#endif
}

std::string aff3ct::tools::bold_green(std::string str)
{
#ifndef ENABLE_COOL_BASH
	return str;
#else
	if (enable_bash_tools)
		return "\e[1m\e[32m" + str + "\e[39m\e[0m";
	else
		return str;
#endif
}

std::string aff3ct::tools::yellow(std::string str)
{
#ifndef ENABLE_COOL_BASH
	return str;
#else
	if (enable_bash_tools)
		return "\e[33m" + str + "\e[39m";
	else
		return str;
#endif
}

std::string aff3ct::tools::bold_yellow(std::string str)
{
#ifndef ENABLE_COOL_BASH
	return str;
#else
	if (enable_bash_tools)
		return "\e[1m\e[33m" + str + "\e[39m\e[0m";
	else
		return str;
#endif
}

std::string aff3ct::tools::blue(std::string str)
{
#ifndef ENABLE_COOL_BASH
	return str;
#else
	if (enable_bash_tools)
		return "\e[94m" + str + "\e[39m";
	else
		return str;
#endif
}

std::string aff3ct::tools::bold_blue(std::string str)
{
#ifndef ENABLE_COOL_BASH
	return str;
#else
	if (enable_bash_tools)
		return "\e[1m\e[94m" + str + "\e[39m\e[0m";
	else
		return str;
#endif
}

std::string aff3ct::tools::orange(std::string str)
{
#ifndef ENABLE_COOL_BASH
	return str;
#else
	if (enable_bash_tools)
		return "\e[38;5;208m" + str + "\e[39m";
	else
		return str;
#endif
}

std::string aff3ct::tools::bold_orange(std::string str)
{
#ifndef ENABLE_COOL_BASH
	return str;
#else
	if (enable_bash_tools)
		return "\e[1m\e[38;5;208m" + str + "\e[39m\e[0m";
	else
		return str;
#endif
}
