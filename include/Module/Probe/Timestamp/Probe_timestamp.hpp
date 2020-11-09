/*!
 * \file
 * \brief Class module::Probe_timestamp.
 */
#ifndef PROBE_TIMESTAMP_HPP_
#define PROBE_TIMESTAMP_HPP_

#include <string>
#include <typeindex>
#include <typeindex>

#include "Module/Probe/Probe.hpp"

namespace aff3ct
{
namespace module
{
template <typename T>
class Probe_timestamp : public Probe<T>
{
public:
	Probe_timestamp(const int size, const std::string &col_name, tools::Reporter_probe& reporter, const int n_frames = 1);

	virtual ~Probe_timestamp() = default;

	virtual std::type_index get_datatype() const;

protected:
	virtual void _probe(const T *in, const size_t frame_id);
};
}
}

#endif /* PROBE_TIMESTAMP_HPP_ */
