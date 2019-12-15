#include "Tools/Algo/Tree/Binary/Binary_tree_metric.hpp"

namespace aff3ct
{
namespace tools
{
template <typename T, typename R>
Binary_tree_metric<T,R>
::Binary_tree_metric(int depth, R path_metric)
: Binary_tree<T>(depth),
  path_metric(path_metric)
{
}

template<typename T, typename R>
R Binary_tree_metric<T,R>
::get_path_metric() const {
	return path_metric;
}

template<typename T, typename R>
void Binary_tree_metric<T,R>
::set_path_metric(R path_metric) {
	this->path_metric = path_metric;
}

template <typename T, typename R>
Binary_tree_metric<T,R>
::~Binary_tree_metric()
{
}
}
}
