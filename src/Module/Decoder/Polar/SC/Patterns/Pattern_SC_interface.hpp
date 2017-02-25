#ifndef PATTERN_SC_INTERFACE_HPP_
#define PATTERN_SC_INTERFACE_HPP_

#include <cmath>
#include <cassert>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <typeinfo>

#include "Tools/Algo/Tree/Binary_tree.hpp"
#include "Tools/Algo/Tree/Binary_node.hpp"

namespace aff3ct
{
namespace module
{
enum pattern_SC_type
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

class Pattern_SC_interface
{
protected:
	const int N;
	const int size;
	const int si_2;
	const tools::Binary_node<Pattern_SC_interface>* node;
	const int off_l;  // offset for lambda buffer (reals)
	const int off_s;  // offset for s buffer (bits)
	const int n_dig;  // number of digits in n
	const int n2_dig; // number of digits in n / 2

	unsigned id;
	std::string key;

	Pattern_SC_interface(const int &N,
	                     const tools::Binary_node<Pattern_SC_interface>* node)
	: N(N),
	  size(N),
	  si_2(0),
	  node(node),
	  off_l(0),
	  off_s(0),
	  n_dig((int) (1 + std::log10(2 * N))),
	  n2_dig((int) (1 + std::log10(N >> 1))),
	  id(0),
	  key("")
	{
		assert(N > 0);
		assert(size > 0);
		assert(node != nullptr);

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
	Pattern_SC_interface()
	 : N(0), size(0), si_2(0), node(nullptr), off_l(0), off_s(0), n_dig(0), n2_dig(0), id(0), key("")
	{
	}

	virtual ~Pattern_SC_interface() {}

	void         set_id   (unsigned id    ) { this->id = id;    }
	unsigned     get_id   (               ) { return this->id;  }
	void         set_key  (std::string key) { this->key = key;  }
	std::string  get_key  (               ) { return this->key; }
	int          get_size (               ) { return size;      }
	int          get_si_2 (               ) { return si_2;      }
	int          get_off_l(               ) { return off_l;     }
	int          get_off_s(               ) { return off_s;     }

	virtual Pattern_SC_interface* alloc(const int &n, const tools::Binary_node<Pattern_SC_interface>* node) const = 0;

	virtual pattern_SC_type type()   const = 0;
	virtual std::string name()       const = 0;
	virtual std::string short_name() const = 0;
	virtual std::string fill_color() const = 0;
	virtual std::string font_color() const = 0;

	virtual std::string f() const = 0;
	virtual std::string g() const = 0;
	virtual std::string h() const = 0;

	virtual std::string apply_f(std::string str_off_l = "", std::string str_off_s = "") const = 0;
	virtual std::string apply_g(std::string str_off_l = "", std::string str_off_s = "") const = 0;
	virtual std::string apply_h(std::string str_off_l = "", std::string str_off_s = "") const = 0;

	virtual int match(const int &reverse_graph_depth,
	                  const tools::Binary_node<Pattern_SC_interface>* node_curr) const = 0;

	virtual bool is_terminal() const = 0;
};
}
}

#endif /* PATTERN_SC_INTERFACE_HPP_ */
