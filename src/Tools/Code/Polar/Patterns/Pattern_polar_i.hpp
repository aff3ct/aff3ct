#ifndef PATTERN_POLAR_INTERFACE_HPP_
#define PATTERN_POLAR_INTERFACE_HPP_

#include <cmath>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <typeinfo>

#include "Tools/Algo/Tree/Binary_tree.hpp"
#include "Tools/Algo/Tree/Binary_node.hpp"

namespace aff3ct
{
namespace tools
{
enum polar_node_t
{
	STANDARD = 0,
	RATE_0_LEFT,
	RATE_0,
	RATE_1,
	REP_LEFT,
	REP,
	SPC,
	NB_PATTERNS
};

// interface
class Pattern_polar_i
{
protected:
	const int N;
	const int m;
	const int size;
	const int si_2;
	const Binary_node<Pattern_polar_i>* node;
	const int off_l;  // offset for lambda buffer (reals)
	const int off_s;  // offset for s buffer (bits)
	const int rev_depth;
	const int n_dig;  // number of digits in n
	const int n2_dig; // number of digits in n / 2
	const std::string tab;

	unsigned id;
	std::string key;

	int min_level;
	int max_level;

	Pattern_polar_i(const int &N, const Binary_node<Pattern_polar_i>* node,
	                const int min_level = 0, const int max_level = -1)
	: N(N),
	  m((int)std::log2(N)),
	  size(N),
	  si_2(0),
	  node(node),
	  off_l(0),
	  off_s(0),
	  rev_depth(m - node->get_depth()),
	  n_dig((int) (1 + std::log10(2 * N))),
	  n2_dig((int) (1 + std::log10(N >> 1))),
	  tab("\t"),
	  id(0),
	  key(""),
	  min_level(min_level),
	  max_level(max_level)
	{
		if (N <= 0)
			throw std::invalid_argument("aff3ct::module::Pattern_polar: \"N\" has to be greater than 0.");
		if (node == nullptr)
			throw std::invalid_argument("aff3ct::module::Pattern_polar: \"node\" can't be null.");
		if (min_level < 0)
			throw std::invalid_argument("aff3ct::module::Pattern_polar: \"min_level\" has to be equal or greater "
			                            "than 0.");
		if (max_level != -1 && (max_level < 0 || max_level < min_level))
			throw std::invalid_argument("aff3ct::module::Pattern_polar: \"max_level\" has to be equal or greater "
			                            "than \"min_level\".");

		const int *p_size = &size;
		for (auto i = 0; i < node->get_depth(); i++) *const_cast<int*>(p_size) >>= 1;

		const int *p_si_2 = &si_2;
		*const_cast<int*>(p_si_2) = size >> 1;

		const int *p_off_l = &off_l;
		for (auto i = N; i > (N >> node->get_depth()); i >>= 1) *const_cast<int*>(p_off_l) += i;

		const int *p_off_s = &off_s;
		*const_cast<int*>(p_off_s) = node->get_lane_id() * size;
	}

public:
	Pattern_polar_i(const int min_level = 0, const int max_level = -1)
	: N(0), m(0), size(0), si_2(0), node(nullptr), off_l(0), off_s(0), rev_depth(0), n_dig(0), n2_dig(0), tab("\t"),
	  id(0), key(""), min_level(min_level), max_level(max_level)
	{
		if (min_level < 0)
			throw std::invalid_argument("aff3ct::module::Pattern_polar: \"min_level\" has to be equal or greater "
			                            "than 0.");
		if (max_level != -1 && (max_level < 0 || max_level < min_level))
			throw std::invalid_argument("aff3ct::module::Pattern_polar: \"max_level\" has to be equal or greater "
			                            "than \"min_level\".");
	}

	virtual ~Pattern_polar_i() {}

	void        set_id   (unsigned id    ) { this->id = id;    }
	unsigned    get_id   (               ) { return this->id;  }
	void        set_key  (std::string key) { this->key = key;  }
	std::string get_key  (               ) { return this->key; }
	int         get_size (               ) { return size;      }
	int         get_si_2 (               ) { return si_2;      }
	int         get_off_l(               ) { return off_l;     }
	int         get_off_s(               ) { return off_s;     }

	inline int get_min_lvl() { return min_level; }
	inline int get_max_lvl() { return max_level; }

	virtual Pattern_polar_i* alloc(const int &n, const Binary_node<Pattern_polar_i>* node) const = 0;

	virtual polar_node_t type()       const = 0;
	virtual std::string  name()       const = 0;
	virtual std::string  short_name() const = 0;
	virtual std::string  fill_color() const = 0;
	virtual std::string  font_color() const = 0;

	virtual std::string f() const = 0;
	virtual std::string g() const = 0;
	virtual std::string h() const = 0;

	virtual std::string apply_f(std::string start_indent = "", std::string str_off_l = "", std::string str_off_s = "") const { return ""; }
	virtual std::string apply_g(std::string start_indent = "", std::string str_off_l = "", std::string str_off_s = "") const { return ""; }
	virtual std::string apply_h(std::string start_indent = "", std::string str_off_l = "", std::string str_off_s = "") const { return ""; }

	virtual std::string range() const
	{
		if (min_level == 0 && max_level == -1)
			return "";
		else if (min_level > 0 && max_level == -1)
			return " (" + std::to_string((int)std::exp2(min_level)) + "+)";
		else if (max_level == min_level)
			return " (" + std::to_string((int)std::exp2(min_level)) + ")";
		else
			return " (" + std::to_string((int)std::exp2(min_level)) + "-" + std::to_string((int)std::exp2(max_level)) + ")";
	}

	virtual int match(const int &reverse_graph_depth, const Binary_node<Pattern_polar_i>* node_curr)
	{
		if (reverse_graph_depth <= 0)
			throw std::invalid_argument("aff3ct::module::Pattern_polar: \"reverse_graph_depth\" has to be "
			                            "greater than 0.");
		if (node_curr == nullptr)
			throw std::invalid_argument("aff3ct::module::Pattern_polar: \"node_curr\" can't be null.");

		if ((reverse_graph_depth >= min_level) && (max_level == -1 || reverse_graph_depth <= max_level))
			return _match(reverse_graph_depth, node_curr);
		else
			return 0;
	}

	virtual int _match(const int &reverse_graph_depth, const Binary_node<Pattern_polar_i>* node_curr) const = 0;

	virtual bool is_terminal() const = 0;
};
}
}

#endif /* PATTERN_POLAR_INTERFACE_HPP_ */
