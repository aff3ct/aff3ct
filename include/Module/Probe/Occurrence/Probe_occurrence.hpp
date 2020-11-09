/*!
 * \file
 * \brief Class module::Probe_occurrence.
 */
#ifndef PROBE_OCCURRENCE_HPP_
#define PROBE_OCCURRENCE_HPP_

#include <string>
#include <vector>
#include <cstdint>
#include <typeindex>

#include "Module/Probe/Probe.hpp"

namespace aff3ct
{
namespace module
{
template <typename T>
class Probe_occurrence : public Probe<T>
{
protected:
	int64_t occurrences;

public:
	Probe_occurrence(const int size, const std::string &col_name, tools::Reporter_probe& reporter, const int n_frames = 1);

	virtual ~Probe_occurrence() = default;

	virtual std::type_index get_datatype() const;

	virtual void reset();

	int64_t get_occurrences() const;

protected:
	virtual void _probe(const T *in, const size_t frame_id);
};
}
}

#endif /* PROBE_OCCURRENCE_HPP_ */
