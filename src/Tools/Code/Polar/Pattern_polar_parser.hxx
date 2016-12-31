#include <cmath>
#include <cassert>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>

#include "Pattern_polar_parser.hpp"

template <typename B>
Pattern_polar_parser<B>
::Pattern_polar_parser(const int& N,
                       const mipp::vector<B>& frozen_bits,
                       const std::vector<Pattern_polar_i*> patterns,
                       const Pattern_polar_i *pattern_rate0,
                       const Pattern_polar_i *pattern_rate1)
: N(N),
  m((int)std::log2(N)),
  frozen_bits(frozen_bits),
  patterns(patterns),
  pattern_rate0(pattern_rate0),
  pattern_rate1(pattern_rate1),
  polar_tree(new Binary_tree<Pattern_polar_i>(m +1)),
  pattern_types()
{
	this->recursive_allocate_nodes_patterns(this->polar_tree->get_root());
	this->generate_nodes_indexes           (this->polar_tree->get_root());
}

template <typename B>
Pattern_polar_parser<B>
::Pattern_polar_parser(const int& N,
                       const mipp::vector<B>& frozen_bits,
                       const std::vector<Pattern_polar_i*> patterns,
                       const int pattern_rate0_id,
                       const int pattern_rate1_id)
: N(N),
  m((int)std::log2(N)),
  frozen_bits(frozen_bits),
  patterns(patterns),
  pattern_rate0(patterns[pattern_rate0_id]),
  pattern_rate1(patterns[pattern_rate1_id]),
  polar_tree(new Binary_tree<Pattern_polar_i>(m +1)),
  pattern_types()
{
	this->recursive_allocate_nodes_patterns(this->polar_tree->get_root());
	this->generate_nodes_indexes           (this->polar_tree->get_root());
}

template <typename B>
Pattern_polar_parser<B>
::~Pattern_polar_parser()
{
	this->recursive_deallocate_nodes_patterns(this->polar_tree->get_root());
	delete polar_tree;
}

template <typename B>
void Pattern_polar_parser<B>
::recursive_allocate_nodes_patterns(Binary_node<Pattern_polar_i>* node_curr)
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
				matching_id = (polar_node_t)i;
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
			node_curr->set_contents(pattern_rate0->alloc(this->N, node_curr));
		else
			node_curr->set_contents(pattern_rate1->alloc(this->N, node_curr));
	}
}

template <typename B>
void Pattern_polar_parser<B>
::generate_nodes_indexes(const Binary_node<Pattern_polar_i>* node_curr)
{
	node_curr->get_c()->set_id(pattern_types.size());
	pattern_types.push_back((unsigned char)node_curr->get_c()->type());

	if (!node_curr->is_leaf()) // stop condition
	{
		this->generate_nodes_indexes(node_curr->get_left() ); // recursive call
		this->generate_nodes_indexes(node_curr->get_right()); // recursive call
	}
}

template <typename B>
void Pattern_polar_parser<B>
::recursive_deallocate_nodes_patterns(Binary_node<Pattern_polar_i>* node_curr)
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

template <typename B>
std::vector<unsigned char> Pattern_polar_parser<B>
::get_pattern_types() const
{
	return pattern_types;
}

template <typename B>
polar_node_t Pattern_polar_parser<B>
::get_node_type(const int node_id) const
{
	return (polar_node_t)pattern_types[node_id];
}

template <typename B>
void Pattern_polar_parser<B>
::release_patterns() const
{
	for (auto i = 0; i < (int)patterns.size(); i++)
		if (patterns[i] != pattern_rate0 && patterns[i] != pattern_rate1)
			delete patterns[i];

	delete pattern_rate0;
	delete pattern_rate1;
}

template <typename B>
const Binary_tree<Pattern_polar_i>* Pattern_polar_parser<B>
::get_polar_tree() const
{
	return polar_tree;
}
