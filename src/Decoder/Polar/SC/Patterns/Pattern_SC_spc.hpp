#ifndef PATTERN_SC_SPC_HPP_
#define PATTERN_SC_SPC_HPP_

#include <cassert>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <typeinfo>

#include "Pattern_SC_interface.hpp"
#include "Pattern_SC_rate0.hpp"
#include "Pattern_SC_rate1.hpp"
#include "Pattern_SC_rep.hpp"

constexpr int spc_level = 2;

template <>
class Pattern_SC<pattern_SC_type::SPC> : public Pattern_SC_interface
{
private:
	int level;

protected:
	Pattern_SC(const int &N,
	           const Binary_node<Pattern_SC_interface>* node)
	 : Pattern_SC_interface(N, node), level(spc_level)
	{
	}

public:
	Pattern_SC() : Pattern_SC_interface(), level(spc_level) { }

	virtual Pattern_SC_interface* alloc(const int &N,
	                                    const Binary_node<Pattern_SC_interface>* node) const
	{
		return new Pattern_SC<pattern_SC_type::SPC>(N, node);
	}

	virtual ~Pattern_SC() {}

	virtual pattern_SC_type type()   const { return SPC;                            }
	virtual std::string name()       const { return "SPC " + std::to_string(level); }
	virtual std::string short_name() const { return "s";                            }
	virtual std::string fill_color() const { return "#2F3F60";                      }
	virtual std::string font_color() const { return "#FFFFFF";                      }

	virtual std::string f() const { return "";    }
	virtual std::string g() const { return "";    }
	virtual std::string h() const { return "spc"; }

	virtual std::string apply_f(std::string str_off_l = "", std::string str_off_s = "") const
	{
		return "";
	}

	virtual std::string apply_g(std::string str_off_l = "", std::string str_off_s = "") const
	{
		return "";
	}

	virtual std::string apply_h(std::string str_off_l = "", std::string str_off_s = "") const
	{
		if (str_off_l.empty()) str_off_l = std::to_string(this->off_l);
		if (str_off_s.empty()) str_off_s = std::to_string(this->off_s);

		auto apply_spc = h();
		std::string spaces = ""; for (auto i = 0; i < 2*this->n_dig+1; i++) spaces += " ";

		std::stringstream stream;
		stream << "API_polar::template "  << apply_spc  << "<" << std::setw(this->n2_dig) << this->size << ">("
		       << "s, "
		       << "l, "
		       << std::setw(this->n_dig ) << str_off_l  << "+" << std::setw(this->n_dig ) << 0          << ", "
		       << spaces                                                                                << "  "
		       << spaces                                                                                << "  "
		       << std::setw(this->n_dig ) << str_off_s  << "+" << std::setw(this->n_dig ) << 0          << ", "
		       << std::setw(this->n2_dig) << this->size << ");" << std::endl;

		return stream.str();
	}

	bool recursive_check_spc(const int reverse_graph_depth, const Binary_node<Pattern_SC_interface>* node_curr) const
	{
		if (!node_curr->is_leaf())
		{
			const Pattern_SC_interface *pattern_left  = node_curr->get_left ()->get_contents();
			const Pattern_SC_interface *pattern_right = node_curr->get_right()->get_contents();

			if (typeid(*pattern_right) != typeid(Pattern_SC<pattern_SC_type::RATE_1>))
			{
				return false;
			}
			else
			{
				if (typeid(*pattern_left) == typeid(Pattern_SC<pattern_SC_type::REP>) && reverse_graph_depth == 2)
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

	virtual int match(const int &reverse_graph_depth,
	                  const Binary_node<Pattern_SC_interface>* node_curr) const
	{
		assert(reverse_graph_depth > 0);

		const Pattern_SC_interface *pattern_left  = node_curr->get_left ()->get_contents();
		const Pattern_SC_interface *pattern_right = node_curr->get_right()->get_contents();

		assert(pattern_left  != nullptr);
		assert(pattern_right != nullptr);

		int match_val = 0;

		if (reverse_graph_depth >= spc_level)
		{
			if (reverse_graph_depth == 2)
			{
				if (typeid(*pattern_left)  == typeid(Pattern_SC<pattern_SC_type::REP>) &&
					typeid(*pattern_right) == typeid(Pattern_SC<pattern_SC_type::RATE_1>))
				{
					match_val = 49;
				}
			}
			else if (typeid(*pattern_left)  == typeid(Pattern_SC<pattern_SC_type::SPC>) &&
					 typeid(*pattern_right) == typeid(Pattern_SC<pattern_SC_type::RATE_1>))
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

#endif /* PATTERN_SC_SPC_FINAL_HPP_ */
