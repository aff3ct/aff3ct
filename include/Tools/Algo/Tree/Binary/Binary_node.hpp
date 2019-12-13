/*!
 * \file
 * \brief Class tools::Binary_node.
 */
#ifndef BINARY_NODE_HPP_
#define BINARY_NODE_HPP_

namespace aff3ct
{
namespace tools
{
template <typename R>
class Binary_tree;

/*!
 * \class Binary_node
 *
 * \brief Binary node structure.
 *
 * \tparam T: the type of the contents in each tree nodes.
 */
template <typename T = float>
class Binary_node
{
#ifndef DOXYGEN_SHOULD_SKIP_THIS
	friend Binary_tree<T>;
#endif

private:
	Binary_node<T>* father; /*!< Pointer to the father node      (nullptr if this node is the root). */
	Binary_node<T>* left;   /*!< Pointer to the left child node  (nullptr if this node is a leaf).   */
	Binary_node<T>* right;  /*!< Pointer to the right child node (nullptr if this node is a leaf).   */

	T* contents; /*!< Pointer to the node contents, could be anything. */

	const int depth;   /*!< Depth   of this node (vertical   indexing). */
	const int lane_id; /*!< Lane id of this node (horizontal indexing). */

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
	Binary_node(Binary_node<T>* father_node,
	            Binary_node<T>* left_node,
	            Binary_node<T>* right_node,
	            T* contents,
	            int depth,
	            int lane_id);

	/*!
	 * \brief Destructor.
	 */
	virtual ~Binary_node();

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

	/*!
	 * \brief Returns true if this node is the right child of its father.
	 *
	 * \return true if this node is the right child of its father.
	 */
	inline bool is_right() const;

	/*!
	 * \brief Returns true if this node is the left child of its father.
	 *
	 * \return true if this node is the left child of its father.
	 */
	inline bool is_left() const;

	/*!
	 * \brief Returns a pointer to the father node.
	 *
	 * \return a pointer to the father node.
	 */
	inline Binary_node<T>* get_father() const;

	/*!
	 * \brief Returns a pointer to the left child node.
	 *
	 * \return a pointer to the left child node.
	 */
	inline Binary_node<T>* get_left() const;

	/*!
	 * \brief Returns a pointer to the right child node.
	 *
	 * \return a pointer to the left right node.
	 */
	inline Binary_node<T>* get_right() const;

	/*!
	 * \brief Returns a pointer to the contents of this node (could be nullptr if there is no contents).
	 *
	 * \return a pointer to the contents of this node (could be nullptr if there is no contents).
	 */
	inline T* get_contents() const;

	/*!
	 * \brief Alias of the Binary_node::get_contents method.
	 *
	 * \copydoc Binary_node::get_contents
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

	/*!
	 * \brief Cuts the left child node (delete the node but not the contents!).
	 */
	inline void cut_left();

	/*!
	 * \brief Cuts the right child node (delete the node but not the contents!).
	 */
	inline void cut_right();
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Tools/Algo/Tree/Binary/Binary_node.hxx"
#endif

#endif /* BINARY_NODE_HPP_ */
