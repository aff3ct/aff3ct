#include <cmath>
#include <cassert>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>

#include "Pattern_parser_polar.hpp"

template <typename B>
Pattern_parser_polar<B>
::Pattern_parser_polar(const int& N,
                       const mipp::vector<B>& frozen_bits,
                       const std::vector<Pattern_SC_interface*> patterns,
                       const Pattern_SC_interface *pattern_rate0,
                       const Pattern_SC_interface *pattern_rate1)
: N(N),
  m((int)std::log2(N)),
  frozen_bits(frozen_bits),
  patterns(patterns),
  pattern_rate0(pattern_rate0),
  pattern_rate1(pattern_rate1),
  polar_tree(new Binary_tree<Pattern_SC_interface>(m +1)),
  pattern_types(),
  pattern_types_sums(),
  leaves_depth(N),
  leaves_rev_depth(N),
  leaf_to_node_id(),
  leaf_to_node_id_sums(N, -1),
  leaf_to_g_node_id()
{
	this->recursive_allocate_nodes_patterns(this->polar_tree->get_root());
	int node_index = 0;
	this->generate_nodes_indexes(this->polar_tree->get_root(), node_index);
	int leaf_index = 0;
	this->recursive_compute_leaves_depth(this->polar_tree->get_root(), leaf_index);
	leaf_index = 0;
	this->generate_nodes_indexes_sums(this->polar_tree->get_root(), leaf_index);
}

template <typename B>
Pattern_parser_polar<B>
::Pattern_parser_polar(const int& N,
                       const mipp::vector<B>& frozen_bits,
                       const std::vector<Pattern_SC_interface*> patterns,
                       const int pattern_rate0_id,
                       const int pattern_rate1_id)
: N(N),
  m((int)std::log2(N)),
  frozen_bits(frozen_bits),
  patterns(patterns),
  pattern_rate0(patterns[pattern_rate0_id]),
  pattern_rate1(patterns[pattern_rate1_id]),
  polar_tree(new Binary_tree<Pattern_SC_interface>(m +1)),
  pattern_types(),
  pattern_types_sums(),
  leaves_depth(N),
  leaves_rev_depth(N),
  leaf_to_node_id(),
  leaf_to_node_id_sums(N, -1),
  leaf_to_g_node_id()
{
	this->recursive_allocate_nodes_patterns(this->polar_tree->get_root());
	int node_index = 0;
	this->generate_nodes_indexes(this->polar_tree->get_root(), node_index);
	int leaf_index = 0;
	this->recursive_compute_leaves_depth(this->polar_tree->get_root(), leaf_index);
	leaf_index = 0;
	this->generate_nodes_indexes_sums(this->polar_tree->get_root(), leaf_index);
}

template <typename B>
Pattern_parser_polar<B>
::~Pattern_parser_polar()
{
	this->recursive_deallocate_nodes_patterns(this->polar_tree->get_root());
	delete polar_tree;
}

template <typename B>
void Pattern_parser_polar<B>
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
			node_curr->set_contents(pattern_rate0->alloc(this->N, node_curr));
		else
			node_curr->set_contents(pattern_rate1->alloc(this->N, node_curr));
	}
}

template <typename B>
void Pattern_parser_polar<B>
::generate_nodes_indexes(const Binary_node<Pattern_SC_interface>* node_curr, int& node_index)
{
	node_curr->get_c()->set_id(pattern_types.size());
	pattern_types.push_back((unsigned char)node_curr->get_c()->type());

	if (!node_curr->is_leaf()) // stop condition
	{
		this->generate_nodes_indexes(node_curr->get_left(),  node_index); // recursive call
		this->generate_nodes_indexes(node_curr->get_right(), node_index); // recursive call
	}
	else
	{
		leaf_to_node_id.push_back(node_curr->get_c()->get_id());
		leaf_to_g_node_id.push_back(this->recursive_find_g_node_id(node_curr));
	}
}

template <typename B>
void Pattern_parser_polar<B>
::generate_nodes_indexes_sums(const Binary_node<Pattern_SC_interface>* node_curr, int& leaf_index)
{
	if (!node_curr->is_leaf()) // stop condition
	{
		this->generate_nodes_indexes_sums(node_curr->get_left(),  leaf_index); // recursive call
		leaf_to_node_id_sums[leaf_index] = pattern_types_sums.size();
		this->generate_nodes_indexes_sums(node_curr->get_right(), leaf_index); // recursive call
		leaf_to_node_id_sums[leaf_index] = pattern_types_sums.size();
		pattern_types_sums.push_back((unsigned char)node_curr->get_c()->type());
	}
	else
		leaf_index++;
}

template <typename B>
int Pattern_parser_polar<B>
::recursive_find_g_node_id(const Binary_node<Pattern_SC_interface>* node_curr)
{
	if (node_curr->is_right())
		return node_curr->get_father()->get_c()->get_id();
	else if (node_curr->is_left())
		return recursive_find_g_node_id(node_curr->get_father());
	else
		return -1;
}

template <typename B>
void Pattern_parser_polar<B>
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

template <typename B>
void Pattern_parser_polar<B>
::recursive_compute_leaves_depth(Binary_node<Pattern_SC_interface>* node_curr, int& leaf_index)
{
	const auto my_pattern_type = node_curr->get_c()->type();

	const bool is_terminal_pattern = (my_pattern_type == pattern_SC_type::RATE_0) ||
	                                 (my_pattern_type == pattern_SC_type::RATE_1) ||
	                                 (my_pattern_type == pattern_SC_type::REP)    ||
	                                 (my_pattern_type == pattern_SC_type::SPC);

	if (!node_curr->is_leaf() && !is_terminal_pattern)
	{
		this->recursive_compute_leaves_depth(node_curr->get_left (), leaf_index); // recursive call
		this->recursive_compute_leaves_depth(node_curr->get_right(), leaf_index); // recursive call
	}
	else
	{
		this->leaves_depth    [leaf_index] =     node_curr->get_depth();
		this->leaves_rev_depth[leaf_index] = m - node_curr->get_depth();
		leaf_index += node_curr->get_c()->get_size();
	}
}

template <typename B>
std::vector<unsigned char> Pattern_parser_polar<B>
::get_pattern_types() const
{
	return pattern_types;
}

template <typename B>
pattern_SC_type Pattern_parser_polar<B>
::get_node_type(const int node_id) const
{
	return (pattern_SC_type)pattern_types[node_id];
}

template <typename B>
pattern_SC_type Pattern_parser_polar<B>
::get_node_type_sums(const int node_id) const
{
	return (pattern_SC_type)pattern_types_sums[node_id];
}

template <typename B>
pattern_SC_type Pattern_parser_polar<B>
::get_leaf_type(const int leaf_id) const
{
	return (pattern_SC_type)pattern_types[get_leaf_to_node_id(leaf_id)];
}

template <typename B>
unsigned char Pattern_parser_polar<B>
::get_leaf_depth(const int leaf_id) const
{
	return this->leaves_depth[leaf_id];
}

template <typename B>
unsigned char Pattern_parser_polar<B>
::get_leaf_rev_depth(const int leaf_id) const
{
	return this->leaves_rev_depth[leaf_id];
}

template <typename B>
int Pattern_parser_polar<B>
::get_leaf_to_node_id(const int leaf_id) const
{
	return leaf_to_node_id[leaf_id];
}

template <typename B>
int Pattern_parser_polar<B>
::get_leaf_to_node_id_sums(const int leaf_id) const
{
	return leaf_to_node_id_sums[leaf_id];
}

template <typename B>
int Pattern_parser_polar<B>
::get_leaf_to_g_node_id(const int leaf_id) const
{
	return leaf_to_g_node_id[leaf_id];
}

template <typename B>
void Pattern_parser_polar<B>
::release_patterns() const
{
	for (auto i = 0; i < (int)patterns.size(); i++)
		if (patterns[i] != pattern_rate0 && patterns[i] != pattern_rate1)
			delete patterns[i];

	delete pattern_rate0;
	delete pattern_rate1;
}

template <typename B>
const Binary_tree<Pattern_SC_interface>* Pattern_parser_polar<B>
::get_polar_tree() const
{
	return polar_tree;
}
