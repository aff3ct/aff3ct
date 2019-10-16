/*!
 * \file
 * \brief Binary tree structure with a metric.
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef BINARY_TREE_METRIC_HPP_
#define BINARY_TREE_METRIC_HPP_

#include "Tools/Algo/Tree/Binary_tree.hpp"

namespace aff3ct
{
namespace tools
{
/*!
 * \class Binary_tree_metric
 *
 * \brief Binary tree structure with a metric.
 *
 * \tparam T: the type of the contents in each tree nodes.
 * \tparam R: the type of the tree metric.
 */
template <typename T = float, typename R = int>
class Binary_tree_metric : public Binary_tree<T>
{
protected:
	R path_metric; /*!< Metric of the tree. */

public:
	/*!
	 * \brief Constructor.
	 *
	 * Builds a tree and allocates the nodes.
	 *
	 * \param depth:       the binary tree depth.
	 * \param path_metric: internal tree metric.
	 */
	Binary_tree_metric(int depth, R path_metric);

	/*!
	 * \brief Destructor.
	 *
	 * Releases all the nodes in the tree.
	 */
	virtual ~Binary_tree_metric();

	/*!
	 * \brief Returns the tree metric.
	 *
	 * \return the tree metric.
	 */
	R get_path_metric() const;

	/*!
	 * \brief Sets the tree metric.
	 *
	 * \param path_metric: a tree metric.
	 */
	void set_path_metric(R path_metric);
};
}
}

#include "Tools/Algo/Tree/Binary_tree_metric.hxx"

#endif /* BINARY_TREE_METRIC_HPP_ */

