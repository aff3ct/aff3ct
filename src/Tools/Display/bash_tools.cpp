#include "bash_tools.h"

std::string bold(std::string str)
{
#ifndef ENABLE_COOL_BASH
	return str;
#else
	return "\e[1m" + str + "\e[0m";
#endif
}

std::string italic(std::string str)
{
#ifndef ENABLE_COOL_BASH
	return str;
#else
	return "\e[3m" + str + "\e[0m";
#endif
}

std::string bold_italic(std::string str)
{
#ifndef ENABLE_COOL_BASH
	return str;
#else
	return "\e[1m\e[3m" + str + "\e[0m";
#endif
}

std::string underlined(std::string str)
{
#ifndef ENABLE_COOL_BASH
	return str;
#else
	return "\e[4m" + str + "\e[0m";
#endif
}

std::string bold_underlined(std::string str)
{
#ifndef ENABLE_COOL_BASH
	return str;
#else
	return "\e[1m\e[4m" + str + "\e[0m";
#endif
}

std::string inverted(std::string str)
{
#ifndef ENABLE_COOL_BASH
	return str;
#else
	return "\e[7m" + str + "\e[0m";
#endif
}

std::string red(std::string str)
{
#ifndef ENABLE_COOL_BASH
	return str;
#else
	return "\e[31m" + str + "\e[39m";
#endif
}

std::string bold_red(std::string str)
{
#ifndef ENABLE_COOL_BASH
	return str;
#else
	return "\e[1m\e[31m" + str + "\e[39m\e[0m";
#endif
}

std::string green(std::string str)
{
#ifndef ENABLE_COOL_BASH
	return str;
#else
	return "\e[32m" + str + "\e[39m";
#endif
}

std::string bold_green(std::string str)
{
#ifndef ENABLE_COOL_BASH
	return str;
#else
	return "\e[1m\e[32m" + str + "\e[39m\e[0m";
#endif
}

std::string yellow(std::string str)
{
#ifndef ENABLE_COOL_BASH
	return str;
#else
	return "\e[33m" + str + "\e[39m";
#endif
}

std::string bold_yellow(std::string str)
{
#ifndef ENABLE_COOL_BASH
	return str;
#else
	return "\e[1m\e[33m" + str + "\e[39m\e[0m";
#endif
}

std::string blue(std::string str)
{
#ifndef ENABLE_COOL_BASH
	return str;
#else
	return "\e[94m" + str + "\e[39m";
#endif
}

std::string bold_blue(std::string str)
{
#ifndef ENABLE_COOL_BASH
	return str;
#else
	return "\e[1m\e[94m" + str + "\e[39m\e[0m";
#endif
}

std::string orange(std::string str)
{
#ifndef ENABLE_COOL_BASH
	return str;
#else
	return "\e[38;5;208m" + str + "\e[39m";
#endif
}

std::string bold_orange(std::string str)
{
#ifndef ENABLE_COOL_BASH
	return str;
#else
	return "\e[1m\e[38;5;208m" + str + "\e[39m\e[0m";
#endif
}