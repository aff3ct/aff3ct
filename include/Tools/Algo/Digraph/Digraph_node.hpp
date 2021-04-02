/*!
 * \file
 * \brief Class tools::Digraph_node.
 */
#ifndef DIGRAPH_NODE_HPP_
#define DIGRAPH_NODE_HPP_

#include <vector>
#include <cstddef>

namespace aff3ct
{
namespace tools
{

template <typename T = float>
class Digraph_node
{
private:
	std::vector<Digraph_node<T>*> fathers;
	std::vector<Digraph_node<T>*> children;

	T* contents; /*!< Pointer to the node contents, could be anything. */

	size_t depth; /*!< Depth of this node (vertical indexing). */

public:
	Digraph_node(std::vector<Digraph_node<T>*> fathers,
	             std::vector<Digraph_node<T>*> children,
	             T* contents,
	             const size_t depth);

	virtual ~Digraph_node();

	inline bool is_no_father() const;

	inline bool is_no_child() const;

	inline bool is_empty() const;

	inline const std::vector<Digraph_node<T>*>& get_fathers() const;

	inline const std::vector<Digraph_node<T>*>& get_children() const;

	inline T* get_contents() const;

	inline T* get_c() const;

	inline void set_contents(T* contents);

	inline void set_depth(const size_t depth);

	inline size_t get_depth() const;

	inline int get_child_pos (const Digraph_node<T>& father) const;

	inline int get_father_pos(const Digraph_node<T>& child ) const;

	inline bool cut_child(const size_t pos);

	inline bool cut_father(const size_t pos);

	inline bool cut_children();

	inline void add_child(Digraph_node<T>* child, const int pos = -1);

	inline void add_father(Digraph_node<T>* father, const int pos = -1);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Tools/Algo/Digraph/Digraph_node.hxx"
#endif

#endif /* DIGRAPH_NODE_HPP_ */
