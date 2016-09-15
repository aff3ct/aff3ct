#ifndef BINARY_TREE_HPP_
#define BINARY_TREE_HPP_

#include "Binary_node.hpp"

template <typename T = float>
class Binary_tree
{
private:
	int             depth;
	Binary_node<T>* root;
	std::vector<Binary_node<T>*> leaves;

public:
	Binary_tree(int depth);
	virtual ~Binary_tree();

	Binary_node<T>*              get_root()   const;
	std::vector<Binary_node<T>*> get_leaves() const;

private:
	void create_nodes        (Binary_node<T>* cur_node, int cur_depth, std::vector<int>& lanes);
	void delete_nodes        (Binary_node<T>* cur_node                                        );
	void recursive_get_leaves(Binary_node<T>* cur_node                                        );
};

#include "Binary_tree.hxx"

#endif /* BINARY_TREE_HPP_ */
