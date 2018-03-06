#ifndef MONITOR_REDUCTION_HPP_
#define MONITOR_REDUCTION_HPP_

#include <string>
#include <vector>

#include "Monitor_BFER.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int>
class Monitor_BFER_reduction : public Monitor_BFER<B>
{
private:
	unsigned long long n_analyzed_frames_historic;
	std::vector<Monitor_BFER<B>*> monitors;

public:
	Monitor_BFER_reduction(const std::vector<Monitor_BFER<B>*> &monitors);
	virtual ~Monitor_BFER_reduction();

	unsigned long long get_n_analyzed_fra_historic() const;
	unsigned long long get_n_analyzed_fra         () const;
	unsigned long long get_n_fe                   () const;
	unsigned long long get_n_be                   () const;

	virtual void reset();
	virtual void clear_callbacks();
};
}
}

#endif /* MONITOR_REDUCTION_HPP_ */
