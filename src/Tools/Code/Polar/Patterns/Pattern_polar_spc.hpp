#ifndef PATTERN_POLAR_SPC_HPP_
#define PATTERN_POLAR_SPC_HPP_

#include <cassert>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <typeinfo>

#include "Pattern_polar_i.hpp"

#include "Pattern_polar_r0.hpp"
#include "Pattern_polar_r1.hpp"
#include "Pattern_polar_rep.hpp"

constexpr int spc_level = 2;

class Pattern_polar_spc : public Pattern_polar_i
{
private:
	int level;

protected:
	Pattern_polar_spc(const int &N, const Binary_node<Pattern_polar_i>* node) : Pattern_polar_i(N, node), level(spc_level)
	{
	}

public:
	Pattern_polar_spc() : Pattern_polar_i(), level(spc_level) { }

	virtual Pattern_polar_i* alloc(const int &N, const Binary_node<Pattern_polar_i>* node) const
	{
		return new Pattern_polar_spc(N, node);
	}

	virtual ~Pattern_polar_spc() {}

	virtual polar_node_t type()       const { return polar_node_t::SPC;              }
	virtual std::string  name()       const { return "SPC " + std::to_string(level); }
	virtual std::string  short_name() const { return "s";                            }
	virtual std::string  fill_color() const { return "#2F3F60";                      }
	virtual std::string  font_color() const { return "#FFFFFF";                      }

	virtual std::string f() const { return "";    }
	virtual std::string g() const { return "";    }
	virtual std::string h() const { return "spc"; }

	bool recursive_check_spc(const int reverse_graph_depth, const Binary_node<Pattern_polar_i>* node_curr) const
	{
		if (!node_curr->is_leaf())
		{
			const Pattern_polar_i *pattern_left  = node_curr->get_left ()->get_contents();
			const Pattern_polar_i *pattern_right = node_curr->get_right()->get_contents();

			if (typeid(*pattern_right) != typeid(Pattern_polar_r1))
			{
				return false;
			}
			else
			{
				if (typeid(*pattern_left) == typeid(Pattern_polar_rep) && reverse_graph_depth == 2)
					return true;
				else
					return this->recursive_check_spc(reverse_graph_depth -1, node_curr->get_left());
			}
		}
		else
		{
			return false;
		}
	}

	virtual int match(const int &reverse_graph_depth, const Binary_node<Pattern_polar_i>* node_curr) const
	{
		assert(reverse_graph_depth > 0);

		const Pattern_polar_i *pattern_left  = node_curr->get_left ()->get_contents();
		const Pattern_polar_i *pattern_right = node_curr->get_right()->get_contents();

		assert(pattern_left  != nullptr);
		assert(pattern_right != nullptr);

		int match_val = 0;

		if (reverse_graph_depth >= spc_level)
		{
			if (reverse_graph_depth == 2)
			{
//				if (typeid(*pattern_left)  == typeid(Pattern_polar_rep) &&
//				    typeid(*pattern_right) == typeid(Pattern_polar_r1 ))
				if (pattern_left ->type() == polar_node_t::REP &&
				    pattern_right->type() == polar_node_t::RATE_1)
				{
					match_val = 49;
				}
			}
//			else if (typeid(*pattern_left)  == typeid(Pattern_polar_spc) &&
//			         typeid(*pattern_right) == typeid(Pattern_polar_r1 ))
			else if (pattern_left ->type() == polar_node_t::SPC &&
			         pattern_right->type() == polar_node_t::RATE_1)
			{
				match_val = 49;
			}
			else if (this->recursive_check_spc(reverse_graph_depth, node_curr))
			{
				match_val = 49;
			}
		}

		return match_val;
	}

	virtual bool is_terminal() const { return true; }
};

#endif /* PATTERN_POLAR_SPC_HPP_ */
