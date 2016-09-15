#ifndef BINARY_TREE_METRIC_HPP_
#define BINARY_TREE_METRIC_HPP_

#include "Tools/Algo/Tree/Binary_tree.hpp"

template <typename T = float, typename R = int>
class Binary_tree_metric : public Binary_tree<T>
{
protected:
	R path_metric;

public:
	Binary_tree_metric(int depth, R path_metric);
	virtual ~Binary_tree_metric();

	R    get_path_metric(             ) const;
	void set_path_metric(R path_metric);
};

#include "Binary_tree_metric.hxx"

#endif /* BINARY_TREE_METRIC_HPP_ */

