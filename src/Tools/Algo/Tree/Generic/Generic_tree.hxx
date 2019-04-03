#include <vector>
#include <iostream>

#include "Tools/Exception/exception.hpp"

#include "Generic_tree.hpp"

namespace aff3ct
{
namespace tools
{
template <typename T>
Generic_tree<T>
::Generic_tree(const int depth, const int base)
: depth(depth), root(nullptr)
{
	std::vector<int> lanes(depth +1);
	for (unsigned i = 0; i < lanes.size(); i++)
		lanes[i] = 0;

	auto cur_depth = 0;
	this->root = new Generic_node<T>(nullptr, std::vector<Generic_node<T>*>(), nullptr, cur_depth, lanes[cur_depth]);
	this->create_nodes(this->root, cur_depth +1, lanes, base);
	recursive_get_leaves(this->get_root());
}

template <typename T>
Generic_tree<T>
::~Generic_tree()
{
	this->delete_nodes(this->root);
}

template <typename T>
Generic_node<T>* Generic_tree<T>
::get_root() const
{
	return this->root;
}

template <typename T>
void Generic_tree<T>
::create_nodes(Generic_node<T>* cur_node, int cur_depth, std::vector<int> &lanes, const int base)
{
	if (cur_node->children.size())
		throw runtime_error(__FILE__, __LINE__, __func__, "'cur_node->children.size()' has to be equal to 0.");

	if (cur_depth < this->depth)
	{
		for (auto c = 0; c < base; c++)
		{
			auto child = new Generic_node<T>(cur_node,
			                                 std::vector<Generic_node<T>*>(),
			                                 nullptr,
			                                 cur_depth,
			                                 lanes[cur_depth]++);
			cur_node->children.push_back(child);
			this->create_nodes(child, cur_depth +1, lanes, base);
		}
	}
}

template <typename T>
void Generic_tree<T>
::delete_nodes(Generic_node<T>* cur_node)
{
	if (cur_node != nullptr)
	{
		for (auto c : cur_node->children)
			this->delete_nodes(c);
		delete cur_node;
	}
}

template<typename T>
void Generic_tree<T>
::recursive_get_leaves(Generic_node<T>* cur_node){
	if (cur_node->is_leaf())
		leaves.push_back(cur_node);
	else
		for (auto c : cur_node->children)
			recursive_get_leaves(c);
}

template<typename T>
std::vector<Generic_node<T>*> Generic_tree<T>
::get_leaves() const{
	return this->leaves;
}
}
}
