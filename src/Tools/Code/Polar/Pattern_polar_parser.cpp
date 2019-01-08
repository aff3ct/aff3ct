#include <stdexcept>
#include <cmath>
#include <algorithm>

#include "Pattern_polar_parser.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

std::vector<std::unique_ptr<tools::Pattern_polar_i>>
convert_vector_to_vecuniptr(const std::vector<tools::Pattern_polar_i*> patterns)
{
	std::vector<std::unique_ptr<tools::Pattern_polar_i>> v;

	for (auto& p : patterns)
		v.push_back(std::unique_ptr<tools::Pattern_polar_i>(p));

	return v;
}


Pattern_polar_parser
::Pattern_polar_parser(const int& N,
                       const std::vector<bool> &frozen_bits,
                       std::vector<std::unique_ptr<tools::Pattern_polar_i>> &&patterns,
                       const std::unique_ptr<tools::Pattern_polar_i>& pattern_rate0,
                       const std::unique_ptr<tools::Pattern_polar_i>& pattern_rate1)
: N(N),
  m((int)std::log2(N)),
  frozen_bits(frozen_bits),
  patterns(std::move(patterns)),
  pattern_rate0(pattern_rate0),
  pattern_rate1(pattern_rate1),
  polar_tree(new Binary_tree<Pattern_polar_i>(m +1)),
  pattern_types(),
  leaves_pattern_types()
{
	this->recursive_allocate_nodes_patterns(this->polar_tree->get_root());
	this->generate_nodes_indexes           (this->polar_tree->get_root());
}

Pattern_polar_parser
::Pattern_polar_parser(const int& N,
                       const std::vector<bool>& frozen_bits,
                       std::vector<std::unique_ptr<tools::Pattern_polar_i>> &&patterns,
                       const int pattern_rate0_id,
                       const int pattern_rate1_id)
: Pattern_polar_parser(N, frozen_bits, std::move(patterns), patterns[pattern_rate0_id], patterns[pattern_rate1_id])
{
}

Pattern_polar_parser
::Pattern_polar_parser(const int& N,
                       const std::vector<bool> &frozen_bits,
                       const std::vector<tools::Pattern_polar_i*> patterns,
                       const std::unique_ptr<tools::Pattern_polar_i> &pattern_rate0,
                       const std::unique_ptr<tools::Pattern_polar_i> &pattern_rate1)
: Pattern_polar_parser(N, frozen_bits, convert_vector_to_vecuniptr(patterns), pattern_rate0, pattern_rate1)
{
}

Pattern_polar_parser
::Pattern_polar_parser(const int& N,
                       const std::vector<bool>& frozen_bits,
                       const std::vector<tools::Pattern_polar_i*> patterns,
                       const int pattern_rate0_id,
                       const int pattern_rate1_id)
: Pattern_polar_parser(N, frozen_bits, convert_vector_to_vecuniptr(patterns), pattern_rate0_id, pattern_rate1_id)
{
}

Pattern_polar_parser
::~Pattern_polar_parser()
{
	this->recursive_deallocate_nodes_patterns(this->polar_tree->get_root());
}

void Pattern_polar_parser
::notify_frozenbits_update()
{
	this->recursive_deallocate_nodes_patterns(this->polar_tree->get_root());
	this->pattern_types.clear();
	this->leaves_pattern_types.clear();

	this->polar_tree.reset(new Binary_tree<Pattern_polar_i>(m +1));
	this->recursive_allocate_nodes_patterns(this->polar_tree->get_root());
	this->generate_nodes_indexes           (this->polar_tree->get_root());
}

void Pattern_polar_parser
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
				matching_id = (int)i;
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

void Pattern_polar_parser
::generate_nodes_indexes(const Binary_node<Pattern_polar_i>* node_curr)
{
	node_curr->get_c()->set_id((unsigned int)pattern_types.size());
	pattern_types.push_back((unsigned char)node_curr->get_c()->type());

	if (!node_curr->is_leaf()) // stop condition
	{
		this->generate_nodes_indexes(node_curr->get_left() ); // recursive call
		this->generate_nodes_indexes(node_curr->get_right()); // recursive call
	}
	else
		leaves_pattern_types.push_back(std::make_pair<unsigned char, int>((unsigned char)node_curr->get_c()->type(),
		                                                                  node_curr->get_c()->get_size()));
}

void Pattern_polar_parser
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

std::vector<unsigned char> Pattern_polar_parser
::get_pattern_types() const
{
	return pattern_types;
}

const std::vector<std::pair<unsigned char, int>>& Pattern_polar_parser
::get_leaves_pattern_types() const
{
	return leaves_pattern_types;
}

const Binary_tree<Pattern_polar_i>& Pattern_polar_parser
::get_polar_tree() const
{
	return *polar_tree;
}