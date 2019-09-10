#include "Tools/Algo/Tree/Generic/Generic_tree_metric.hpp"

namespace aff3ct
{
namespace tools
{
template <typename T, typename R>
Generic_tree_metric<T,R>
::Generic_tree_metric(const int depth, const int base, R path_metric)
:Generic_tree<T>(depth, base),path_metric(path_metric)
{
}

template <typename T, typename R>
Generic_tree_metric<T,R>
::Generic_tree_metric(const std::vector<uint32_t> &sequence, R path_metric)
:Generic_tree<T>(sequence),path_metric(path_metric)
{
}

template<typename T, typename R>
R Generic_tree_metric<T,R>
::get_path_metric() const {
	return path_metric;
}

template<typename T, typename R>
void Generic_tree_metric<T,R>
::set_path_metric(R path_metric) {
	this->path_metric = path_metric;
}

template <typename T, typename R>
Generic_tree_metric<T,R>
::~Generic_tree_metric()
{
}
}
}
