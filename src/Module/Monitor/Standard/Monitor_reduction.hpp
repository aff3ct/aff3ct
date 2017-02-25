#ifndef MONITOR_REDUCTION_HPP_
#define MONITOR_REDUCTION_HPP_

#include <vector>

#include "Monitor_std.hpp"

namespace aff3ct
{
namespace module
{
template <typename B>
class Monitor_reduction : public Monitor_std<B>
{
private:
	std::vector<Monitor<B>*>& error_analyzers;

public:
	Monitor_reduction(const int& K, const int& N, const int& max_fe,
	                  std::vector<Monitor<B>*>& error_analyzers, const int& n_frames = 1,
	                  const std::string name = "Monitor_reduction");
	virtual ~Monitor_reduction();

	unsigned long long get_n_analyzed_fra() const;
	int get_n_fe() const;
	int get_n_be() const;
};
}
}

#endif /* MONITOR_REDUCTION_HPP_ */
