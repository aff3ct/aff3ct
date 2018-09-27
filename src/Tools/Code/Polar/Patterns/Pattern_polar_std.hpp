#ifndef PATTERN_POLAR_STANDARD_HPP_
#define PATTERN_POLAR_STANDARD_HPP_

#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <typeinfo>
#include <algorithm>

#include "Pattern_polar_i.hpp"

namespace aff3ct
{
namespace tools
{
class Pattern_polar_std : public Pattern_polar_i
{
protected:
	Pattern_polar_std(const int &N, const Binary_node<Pattern_polar_i>* node) : Pattern_polar_i(N, node)
	{
	}

public:
	Pattern_polar_std() : Pattern_polar_i() { }

	virtual Pattern_polar_i* alloc(const int &N, const Binary_node<Pattern_polar_i>* node) const
	{
		return new Pattern_polar_std(N, node);
	}

	virtual ~Pattern_polar_std() = default;

	virtual polar_node_t type()       const { return polar_node_t::STANDARD; }
	virtual std::string  name()       const { return "Default";              }
	virtual std::string  short_name() const { return "d";                    }
	virtual std::string  fill_color() const { return "#7F7F7F";              }
	virtual std::string  font_color() const { return "#FFFFFF";              }

	virtual std::string f() const { return "f";  }
	virtual std::string g() const { return "g";  }
	virtual std::string h() const { return "xo"; }

	virtual int _match(const int &reverse_graph_depth, const Binary_node<Pattern_polar_i>* node_curr) const
	{
		int match_val = 1;

		return match_val;
	}

	virtual bool is_terminal() const { return false; }
};
}
}

#endif /* PATTERN_POLAR_STANDARD_HPP_ */
