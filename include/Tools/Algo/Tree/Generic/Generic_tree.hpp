/*!
 * \file
 * \brief Class tools::Generic_tree.
 */
#ifndef GENERIC_TREE_HPP_
#define GENERIC_TREE_HPP_

#include <vector>
#include <cstdint>

#include "Tools/Algo/Tree/Generic/Generic_node.hpp"

namespace aff3ct
{
namespace tools
{
template <typename T = float>
class Generic_tree
{
private:
	const std::vector<uint32_t>   sequence;
	int                           depth;  /*!< Depth of the tree. */
	Generic_node<T>*              root;   /*!< Pointer to the root node (first node at the top in the tree). */
	std::vector<Generic_node<T>*> leaves; /*!< Vector of the tree leave pointers. */

public:
	explicit Generic_tree(const int depth, const int base);

	explicit Generic_tree(const std::vector<uint32_t> &sequence);

	Generic_tree(const Generic_tree<T> &bt);

	Generic_tree<T>& operator=(const Generic_tree<T> &bt);

	virtual ~Generic_tree();

	Generic_node<T>* get_root() const;

	std::vector<Generic_node<T>*> get_leaves() const;

	int get_depth() const;

private:
	void init                ();
	void recursive_copy      (const Generic_node<T> *nref, Generic_node<T> *nclone                                                    );
	void create_nodes        (Generic_node<T>* cur_node, int cur_depth, std::vector<int>& lanes, const std::vector<uint32_t> &sequence);
	void delete_nodes        (Generic_node<T>* cur_node                                                                               );
	void recursive_get_leaves(Generic_node<T>* cur_node                                                                               );
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Tools/Algo/Tree/Generic/Generic_tree.hxx"
#endif

#endif /* GENERIC_TREE_HPP_ */
