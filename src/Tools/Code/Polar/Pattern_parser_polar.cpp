#include <cmath>
#include <cassert>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>

#include "Pattern_parser_polar.hpp"

Pattern_parser_polar
::Pattern_parser_polar(const int& N,
                       const mipp::vector<int>& frozen_bits,
                       const std::vector<Pattern_SC_interface*> &patterns,
                       const Pattern_SC_interface &pattern_rate0,
                       const Pattern_SC_interface &pattern_rate1)
: N(N),
  m((int)std::log2(N)),
  frozen_bits(frozen_bits),
  patterns(patterns),
  pattern_rate0(pattern_rate0),
  pattern_rate1(pattern_rate1),
  polar_tree(new Binary_tree<Pattern_SC_interface>(m +1)),
  pattern_types_per_id(2 * N)
{
	this->recursive_allocate_nodes_patterns(this->polar_tree->get_root());
	int node_index = 0;
	this->generate_nodes_indexes(this->polar_tree->get_root(), node_index);
}

Pattern_parser_polar
::~Pattern_parser_polar()
{
	this->recursive_deallocate_nodes_patterns(this->polar_tree->get_root());
	delete polar_tree;
}

void Pattern_parser_polar
::recursive_allocate_nodes_patterns(Binary_node<Pattern_SC_interface>* node_curr)
{
	if (!node_curr->is_leaf())
	{
		this->recursive_allocate_nodes_patterns(node_curr->get_left ()); // recursive call
		this->recursive_allocate_nodes_patterns(node_curr->get_right()); // recursive call

		// pattern matching
		int reverse_graph_depth = this->m - node_curr->get_depth();
#ifdef _MSC_VER
		int matching_vals[32];
#else
		int matching_vals[patterns.size()];
#endif
		for (unsigned i = 0; i < patterns.size(); i++)
			matching_vals[i] = patterns[i]->match(reverse_graph_depth, node_curr);

		// find the position/id of the biggest matching value
		int matching_id = 0;
		auto best_matching_val = 0;
		for (unsigned i = 0; i < patterns.size(); i++)
		{
			if (matching_vals[i] > best_matching_val)
			{
				best_matching_val = matching_vals[i];
				matching_id = (pattern_SC_type)i;
			}
		}

		// allocate the right pattern for this node
		node_curr->set_contents(patterns[matching_id]->alloc(this->N, node_curr));

		// cutting edges under the current node if the selected pattern is a terminal one
		if (patterns[matching_id]->is_terminal())
		{
			this->recursive_deallocate_nodes_patterns(node_curr->get_left());  node_curr->cut_left();
			this->recursive_deallocate_nodes_patterns(node_curr->get_right()); node_curr->cut_right();
		}
	}
	else
	{
		if (frozen_bits[node_curr->get_lane_id()])
			node_curr->set_contents(pattern_rate0.alloc(this->N, node_curr));
		else
			node_curr->set_contents(pattern_rate1.alloc(this->N, node_curr));
	}
}

void Pattern_parser_polar
::generate_nodes_indexes(const Binary_node<Pattern_SC_interface>* node_curr, int& node_index)
{
	node_curr->get_c()->set_id(node_index++);
	pattern_types_per_id[node_curr->get_c()->get_id()] = (char)node_curr->get_c()->type();

	if (!node_curr->is_leaf()) // stop condition
	{
		this->generate_nodes_indexes(node_curr->get_left(),  node_index); // recursive call
		this->generate_nodes_indexes(node_curr->get_right(), node_index); // recursive call
	}

}

void Pattern_parser_polar
::recursive_deallocate_nodes_patterns(Binary_node<Pattern_SC_interface>* node_curr)
{
	if (node_curr != nullptr)
	{
		this->recursive_deallocate_nodes_patterns(node_curr->get_left()); // recursive call
		this->recursive_deallocate_nodes_patterns(node_curr->get_right()); // recursive call

		auto *contents = node_curr->get_contents();
		delete contents;
		node_curr->set_contents(nullptr);
	}
}

std::vector<char> Pattern_parser_polar
::get_pattern_types_per_id() const
{
	return pattern_types_per_id;
}

const Binary_tree<Pattern_SC_interface>* Pattern_parser_polar
::get_polar_tree() const
{
	return polar_tree;
}
