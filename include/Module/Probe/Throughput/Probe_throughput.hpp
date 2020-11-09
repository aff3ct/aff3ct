/*!
 * \file
 * \brief Class module::Probe_throughput.
 */
#ifndef PROBE_THROUGHPUT_HPP_
#define PROBE_THROUGHPUT_HPP_

#include <string>
#include <vector>
#include <chrono>
#include <typeindex>

#include "Module/Probe/Probe.hpp"

namespace aff3ct
{
namespace module
{
template <typename T>
class Probe_throughput : public Probe<T>
{
protected:
	std::chrono::time_point<std::chrono::steady_clock> t_start;
	double mbps;

public:
	Probe_throughput(const int size, const std::string &col_name, tools::Reporter_probe& reporter, const int n_frames = 1);

	virtual ~Probe_throughput() = default;

	virtual std::type_index get_datatype() const;

	virtual void reset();

protected:
	virtual void _probe(const T *in, const size_t frame_id);
};
}
}

#endif /* PROBE_THROUGHPUT_HPP_ */
