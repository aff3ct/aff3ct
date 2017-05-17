#ifndef MONITOR_REDUCTION_HPP_
#define MONITOR_REDUCTION_HPP_

#include <string>
#include <vector>

#include "Tools/Perf/MIPP/mipp.h"
#include "Monitor_std.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int>
class Monitor_reduction : public Monitor_std<B>
{
private:
	std::vector<Monitor<B>*> monitors;

public:
	Monitor_reduction(const int size, const unsigned max_fe, std::vector<Monitor<B>*> monitors, const int n_frames = 1,
	                  const std::string name = "Monitor_reduction");
	virtual ~Monitor_reduction();

	unsigned long long get_n_analyzed_fra() const;
	unsigned long long get_n_fe          () const;
	unsigned long long get_n_be          () const;

	void reset();
};
}
}

#endif /* MONITOR_REDUCTION_HPP_ */
