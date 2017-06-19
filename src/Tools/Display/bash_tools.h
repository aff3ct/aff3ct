#ifndef BASH_TOOLS_H_
#define BASH_TOOLS_H_

#include <string>
#include <numeric>

namespace aff3ct
{
namespace tools
{
extern bool enable_bash_tools;


/*
 * Format type contains all the details to display correctly the text.
 * binary use is : 32 unused bits, 12 flags for the style,
 *                 2 bits for BG intesity code, 8 bits for BG color code
 *                 2 bits for FG intesity code, 8 bits for FG color code
 * The syntax is such as : Format f = Style::BOLD | Style::ITALIC | FG::BLUE | BG::ORANGE
 */
using Format = uint32_t;

enum Style
{
	BASIC      = (Format)0,
	BLINK      = (Format)1 << 20, // does not work with all terminals (only tty and Xterm)
	BOLD       = BLINK     << 1,
	DIM        = BOLD      << 1,
	HIDDEN     = DIM       << 1,
	INVERTED   = HIDDEN    << 1,
	ITALIC     = INVERTED  << 1,
	UNDERLINED = ITALIC    << 1,
};

namespace FG // foreground
{
	enum Color
	{
		DEFAULT =  (Format)0,
		BLACK   = ((Format)1 <<  0),
		BLUE    = (BLACK   + BLACK),
		CYAN    = (BLUE    + BLACK),
		GRAY    = (CYAN    + BLACK),
		GREEN   = (GRAY    + BLACK),
		MAGENTA = (GREEN   + BLACK),
		ORANGE  = (MAGENTA + BLACK),
		RED     = (ORANGE  + BLACK),
		WHITE   = (RED     + BLACK),
		YELLOW  = (WHITE   + BLACK)
	};

	enum Intensity
	{
		NORMAL  =  (Format)0,
		INTENSE = ((Format)1 << 8)
	};
}

namespace BG // background
{
	enum Color
	{
		DEFAULT =  (Format)0,
		BLACK   = ((Format)1 << 10),
		BLUE    = (BLACK   + BLACK),
		CYAN    = (BLUE    + BLACK),
		GRAY    = (CYAN    + BLACK),
		GREEN   = (GRAY    + BLACK),
		MAGENTA = (GREEN   + BLACK),
		ORANGE  = (MAGENTA + BLACK),
		RED     = (ORANGE  + BLACK),
		WHITE   = (RED     + BLACK),
		YELLOW  = (WHITE   + BLACK)
	};

	enum Intensity
	{
		NORMAL  =  (Format)0,
		INTENSE = ((Format)1 << 18)
	};
}

std::string format  (std::string str, Format f = 0);
std::string style   (std::string str, Style s);
std::string fg_color(std::string str, FG::Color c, FG::Intensity i = FG::Intensity::NORMAL);
std::string bg_color(std::string str, BG::Color c, BG::Intensity i = BG::Intensity::NORMAL);

std::string default_style(std::string str);

std::string format_error           (std::string str);
std::string format_critical_error  (std::string str);
std::string format_warning         (std::string str);
std::string format_critical_warning(std::string str);
std::string format_info            (std::string str);
std::string format_critical_info   (std::string str);
std::string format_positive_info   (std::string str);

using format_function = std::add_pointer<std::string(std::string)>::type;
std::string apply_on_each_line(const std::string& str, format_function fptr);
}
}

#endif /* BASH_TOOLS_H_ */
