/*!
 * \file
 * \brief Class tools::Generic_node.
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

template <typename T = float>
class Generic_node
{
	friend Generic_tree<T>;

private:
	Generic_node<T>* father; /*!< Pointer to the father node      (nullptr if this node is the root). */
	std::vector<Generic_node<T>*> children;

	T* contents; /*!< Pointer to the node contents, could be anything. */

	const int depth;   /*!< Depth   of this node (vertical   indexing). */
	const int lane_id; /*!< Lane id of this node (horizontal indexing). */

	int child_id;

public:
	Generic_node(Generic_node<T>* father_node,
	             std::vector<Generic_node<T>*> children,
	             T* contents,
	             int depth,
	             int lane_id,
	             int child_id = -1);

	virtual ~Generic_node();

	inline bool is_root() const;

	inline bool is_leaf() const;

	inline bool is_empty() const;

	inline int get_pos() const;

	inline Generic_node<T>* get_father() const;

	inline const std::vector<Generic_node<T>*>& get_children() const;

	inline T* get_contents() const;

	inline T* get_c() const;

	inline void set_contents(T* contents);

	inline int get_depth() const;

	inline int get_lane_id() const;

	inline int get_child_id() const;

	inline bool cut_child(const size_t pos);

	inline bool cut_children();

	inline void add_child(Generic_node<T>* child);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Tools/Algo/Tree/Generic/Generic_node.hxx"
#endif

#endif /* GENERIC_NODE_HPP_ */
