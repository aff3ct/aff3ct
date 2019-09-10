/*!
 * \file
 * \brief Generic tree structure with a metric.
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef GENERIC_TREE_METRIC_HPP_
#define GENERIC_TREE_METRIC_HPP_

#include "Tools/Algo/Tree/Generic/Generic_tree.hpp"

namespace aff3ct
{
namespace tools
{
/*!
 * \class Generic_tree_metric
 *
 * \brief Generic tree structure with a metric.
 *
 * \tparam T: the type of the contents in each tree nodes.
 * \tparam R: the type of the tree metric.
 */
template <typename T = float, typename R = int>
class Generic_tree_metric : public Generic_tree<T>
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
	explicit Generic_tree_metric(const int depth, const int base, R path_metric);

	explicit Generic_tree_metric(const std::vector<uint32_t> &sequence, R path_metric);

	/*!
	 * \brief Destructor.
	 *
	 * Releases all the nodes in the tree.
	 */
	virtual ~Generic_tree_metric();

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

#include "Tools/Algo/Tree/Generic/Generic_tree_metric.hxx"

#endif /* GENERIC_TREE_METRIC_HPP_ */

