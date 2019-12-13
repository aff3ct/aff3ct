/*!
 * \file
 * \brief Class tools::Generic_tree_metric.
 */
#ifndef GENERIC_TREE_METRIC_HPP_
#define GENERIC_TREE_METRIC_HPP_

#include "Tools/Algo/Tree/Generic/Generic_tree.hpp"

namespace aff3ct
{
namespace tools
{
template <typename T = float, typename R = int>
class Generic_tree_metric : public Generic_tree<T>
{
protected:
	R path_metric; /*!< Metric of the tree. */

public:
	explicit Generic_tree_metric(const int depth, const int base, R path_metric);

	explicit Generic_tree_metric(const std::vector<uint32_t> &sequence, R path_metric);

	virtual ~Generic_tree_metric();

	R get_path_metric() const;

	void set_path_metric(R path_metric);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Tools/Algo/Tree/Generic/Generic_tree_metric.hxx"
#endif

#endif /* GENERIC_TREE_METRIC_HPP_ */

