/*!
 * \file
 * \brief Binary node structure.
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef GENERIC_NODE_HPP_
#define GENERIC_NODE_HPP_

#include <vector>
#include <cstddef>

namespace aff3ct
{
namespace tools
{
template <typename R>
class Generic_tree;

/*!
 * \class Generic_node
 *
 * \brief Binary node structure.
 *
 * \tparam T: the type of the contents in each tree nodes.
 */
template <typename T = float>
class Generic_node
{
	friend Generic_tree<T>;

private:
	Generic_node<T>* father; /*!< Pointer to the father node      (nullptr if this node is the root). */
	// Generic_node<T>* left;   /*!< Pointer to the left child node  (nullptr if this node is a leaf).   */
	// Generic_node<T>* right;  /*!< Pointer to the right child node (nullptr if this node is a leaf).   */
	std::vector<Generic_node<T>*> children;

	T* contents; /*!< Pointer to the node contents, could be anything. */

	const int depth;   /*!< Depth   of this node (vertical   indexing). */
	const int lane_id; /*!< Lane id of this node (horizontal indexing). */

	int child_id;

public:
	/*!
	 * \brief Constructor.
	 *
	 * \param father_node: a pointer to the father node.
	 * \param left_node:   a pointer to the left child node.
	 * \param right_node:  a pointer to the right child node.
	 * \param contents:    a pointer to generic data.
	 * \param depth:       the depth of this node (vertical indexing).
	 * \param lane_id:     the lane id of this node (horizontal indexing).
	 */
	Generic_node(Generic_node<T>* father_node,
	             std::vector<Generic_node<T>*> children,
	             T* contents,
	             int depth,
	             int lane_id,
	             int child_id = -1);

	/*!
	 * \brief Destructor.
	 */
	virtual ~Generic_node();

	/*!
	 * \brief Returns true if this node is the root of the tree.
	 *
	 * \return true if this node is the root of the tree.
	 */
	inline bool is_root() const;

	/*!
	 * \brief Returns true if this node is a leaf.
	 *
	 * \return true if this node is a leaf.
	 */
	inline bool is_leaf() const;

	/*!
	 * \brief Returns true if there is no contents associated to this node.
	 *
	 * \return true if there is no contents associated to this node.
	 */
	inline bool is_empty() const;

	inline int get_pos() const;

	/*!
	 * \brief Returns a pointer to the father node.
	 *
	 * \return a pointer to the father node.
	 */
	inline Generic_node<T>* get_father() const;

	/*!
	 * \brief Returns a pointer to the left child node.
	 *
	 * \return a pointer to the left child node.
	 */
	inline const std::vector<Generic_node<T>*>& get_children() const;

	/*!
	 * \brief Returns a pointer to the contents of this node (could be nullptr if there is no contents).
	 *
	 * \return a pointer to the contents of this node (could be nullptr if there is no contents).
	 */
	inline T* get_contents() const;

	/*!
	 * \brief Alias of the Generic_node::get_contents method.
	 *
	 * \copydoc Generic_node::get_contents
	 */
	inline T* get_c() const
	{
		return this->get_contents();
	}

	/*!
	 * \brief Set the contents of this node.
	 *
	 * \param contents: a pointer to the contents to set.
	 */
	inline void set_contents(T* contents);

	/*!
	 * \brief Returns the depth of this node (vertical indexing).
	 *
	 * \return the depth of this node (vertical indexing).
	 */
	inline int get_depth() const;

	/*!
	 * \brief Returns the lane id of this node (horizontal indexing).
	 *
	 * \return Returns the lane id of this node (horizontal indexing).
	 */
	inline int get_lane_id() const;

	inline int get_child_id() const;

	inline bool cut_child(const size_t pos);

	inline bool cut_children();
};
}
}

#include "Tools/Algo/Tree/Generic/Generic_node.hxx"

#endif /* GENERIC_NODE_HPP_ */
