#include <sstream>
#include <stdexcept>
#include <cmath>
#include <algorithm>

#include "Tools/Exception/exception.hpp"
#include "Tools/Code/Polar/Pattern_polar_parser.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

Pattern_polar_parser
::Pattern_polar_parser(const std::vector<bool> &frozen_bits,
                       const std::vector<tools::Pattern_polar_i*> &patterns,
                       const size_t pattern_rate0_id,
                       const size_t pattern_rate1_id,
                       const bool delete_input_patterns)
: N(frozen_bits.size()),
  m((int)std::log2(N)),
  frozen_bits(frozen_bits),
  pattern_rate0_id(pattern_rate0_id),
  pattern_rate1_id(pattern_rate1_id),
  polar_tree(m +1),
  pattern_types(),
  leaves_pattern_types()
{
	if (pattern_rate0_id >= patterns.size())
	{
		std::stringstream message;
		message << "'pattern_rate0_id' has to be smaller than 'patterns.size()' ('pattern_rate0_id' = "
		        << pattern_rate0_id << ", 'patterns.size()' = " << patterns.size() << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (pattern_rate1_id >= patterns.size())
	{
		std::stringstream message;
		message << "'pattern_rate1_id' has to be smaller than 'patterns.size()' ('pattern_rate1_id' = "
		        << pattern_rate1_id << ", 'patterns.size()' = " << patterns.size() << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (pattern_rate0_id == pattern_rate1_id)
	{
		std::stringstream message;
		message << "'pattern_rate0_id' can't be equal to 'pattern_rate1_id' ('pattern_rate0_id' = "
		        << pattern_rate0_id << ", 'pattern_rate1_id' = " << pattern_rate1_id << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	for (auto p : patterns)
	{
		this->patterns.push_back(std::shared_ptr<tools::Pattern_polar_i>(p->alloc(0, nullptr)));
		if (delete_input_patterns)
			delete p;
	}

	//this->recursive_allocate_nodes_patterns(this->polar_tree.get_root());
	//this->generate_nodes_indexes           (this->polar_tree.get_root());
	this->set_frozen_bits(frozen_bits);
}

Pattern_polar_parser
::Pattern_polar_parser(const Pattern_polar_parser &ppp)
: N(ppp.N),
  m(ppp.m),
  frozen_bits(ppp.frozen_bits),
  patterns(),
  pattern_rate0_id(ppp.pattern_rate0_id),
  pattern_rate1_id(ppp.pattern_rate1_id),
  polar_tree(ppp.polar_tree),
  pattern_types(ppp.pattern_types),
  leaves_pattern_types(ppp.leaves_pattern_types)
{
	this->deep_copy(*this);
	for (auto &p : ppp.patterns)
		this->patterns.push_back(std::shared_ptr<tools::Pattern_polar_i>(p->alloc(0, nullptr)));
}

Pattern_polar_parser
::~Pattern_polar_parser()
{
	this->recursive_deallocate_nodes_patterns(this->polar_tree.get_root());
}

void Pattern_polar_parser
::deep_copy(const Pattern_polar_parser& t)
{
	this->recursive_deep_copy(t.polar_tree.get_root(), this->polar_tree.get_root());
}

void Pattern_polar_parser
::recursive_deep_copy(const tools::Binary_node<Pattern_polar_i> *nref,
                            tools::Binary_node<Pattern_polar_i> *nclone)
{
	auto cref = nref->get_contents();
	nclone->set_contents((cref != nullptr) ?
	                     cref->alloc(cref->get_size(), cref->get_size() ? nclone : nullptr) :
	                     nullptr);

	if (!nref->is_leaf() && !nclone->is_leaf())
	{
		if (nref->get_left() != nullptr)
			this->recursive_deep_copy(nref->get_left(), nclone->get_left());

		if (nref->get_right() != nullptr)
			this->recursive_deep_copy(nref->get_right(), nclone->get_right());
	}
}
void Pattern_polar_parser
::set_frozen_bits(const std::vector<bool>& fb)
{
	std::copy(fb.begin(), fb.end(), this->frozen_bits.begin());
	this->recursive_deallocate_nodes_patterns(this->polar_tree.get_root());
	this->pattern_types.clear();
	this->leaves_pattern_types.clear();

	this->polar_tree = Binary_tree<Pattern_polar_i>(m +1);
	this->recursive_allocate_nodes_patterns(this->polar_tree.get_root());
	this->generate_nodes_indexes           (this->polar_tree.get_root());

}

const std::vector<bool>& Pattern_polar_parser
::get_frozen_bits() const
{
	return this->frozen_bits;
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
			node_curr->set_contents(patterns[pattern_rate0_id]->alloc(this->N, node_curr));
		else
			node_curr->set_contents(patterns[pattern_rate1_id]->alloc(this->N, node_curr));
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
	return polar_tree;
}