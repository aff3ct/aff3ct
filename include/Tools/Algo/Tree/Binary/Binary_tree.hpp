/*!
 * \file
 * \brief Class tools::Binary_tree.
 */
#ifndef BINARY_TREE_HPP_
#define BINARY_TREE_HPP_

#include <vector>

#include "Tools/Algo/Tree/Binary/Binary_node.hpp"

namespace aff3ct
{
namespace tools
{
/*!
 * \class Binary_tree
 *
 * \brief Binary tree structure.
 *
 * \tparam T: the type of the contents in each tree nodes.
 */
template <typename T = float>
class Binary_tree
{
private:
	int             depth;               /*!< Depth of the tree. */
	Binary_node<T>* root;                /*!< Pointer to the root node (first node at the top in the tree). */
	std::vector<Binary_node<T>*> leaves; /*!< Vector of the tree leave pointers. */

public:
	/*!
	 * \brief Constructor.
	 *
	 * Builds a tree and allocates the nodes.
	 *
	 * \param depth: the binary tree depth.
	 */
	explicit Binary_tree(int depth);

	Binary_tree(const Binary_tree<T> &bt);

	Binary_tree<T>& operator=(const Binary_tree<T> &bt);

	/*!
	 * \brief Destructor.
	 *
	 * Releases all the nodes in the tree.
	 */
	virtual ~Binary_tree();

	/*!
	 * \brief Get the root node.
	 *
	 * \return a pointer to the root node.
	 */
	Binary_node<T>* get_root() const;

	/*!
	 * \brief Get a list of all the leaves in the tree.
	 *
	 * \return a vector of node pointers.
	 */
	const std::vector<Binary_node<T>*>& get_leaves() const;

private:
	void init                (int depth);
	void recursive_copy      (const Binary_node<T> *nref, Binary_node<T> *nclone              );
	void create_nodes        (Binary_node<T>* cur_node, int cur_depth, std::vector<int>& lanes);
	void delete_nodes        (Binary_node<T>* cur_node                                        ) const;
	void recursive_get_leaves(Binary_node<T>* cur_node                                        );
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Tools/Algo/Tree/Binary/Binary_tree.hxx"
#endif

#endif /* BINARY_TREE_HPP_ */
