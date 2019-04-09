/*!
 * \file
 * \brief Binary tree structure.
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef GENERIC_TREE_HPP_
#define GENERIC_TREE_HPP_

#include <vector>
#include <cstdint>

#include "Generic_node.hpp"

namespace aff3ct
{
namespace tools
{
/*!
 * \class Generic_tree
 *
 * \brief Binary tree structure.
 *
 * \tparam T: the type of the contents in each tree nodes.
 */
template <typename T = float>
class Generic_tree
{
private:
	int                           depth;  /*!< Depth of the tree. */
	Generic_node<T>*              root;   /*!< Pointer to the root node (first node at the top in the tree). */
	std::vector<Generic_node<T>*> leaves; /*!< Vector of the tree leave pointers. */

public:
	/*!
	 * \brief Constructor.
	 *
	 * Builds a tree and allocates the nodes.
	 *
	 * \param depth: the binary tree depth.
	 */
	explicit Generic_tree(const int depth, const int base);

	explicit Generic_tree(const std::vector<uint32_t> &sequence);

	/*!
	 * \brief Destructor.
	 *
	 * Releases all the nodes in the tree.
	 */
	virtual ~Generic_tree();

	/*!
	 * \brief Get the root node.
	 *
	 * \return a pointer to the root node.
	 */
	Generic_node<T>* get_root() const;

	/*!
	 * \brief Get a list of all the leaves in the tree.
	 *
	 * \return a vector of node pointers.
	 */
	std::vector<Generic_node<T>*> get_leaves() const;

private:
	void create_nodes        (Generic_node<T>* cur_node, int cur_depth, std::vector<int>& lanes, const std::vector<uint32_t> &sequence);
	void delete_nodes        (Generic_node<T>* cur_node                                                                               );
	void recursive_get_leaves(Generic_node<T>* cur_node                                                                               );
};
}
}

#include "Generic_tree.hxx"

#endif /* GENERIC_TREE_HPP_ */
