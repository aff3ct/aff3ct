#ifdef AFF3CT_MPI

#ifndef MONITOR_REDUCTION_MPI_HPP_
#define MONITOR_REDUCTION_MPI_HPP_

#include <mpi.h>

#include "Monitor_reduction.hpp"

namespace aff3ct
{
namespace module
{
template <class M> // M is the monitor on which must be applied the reduction
class Monitor_reduction_MPI : public Monitor_reduction_M<M>
{
protected:
	using Attributes = typename M::Attributes;

private:
	MPI_Datatype MPI_monitor_vals;
	MPI_Op       MPI_Op_reduce_monitors;

public:
	explicit Monitor_reduction_MPI(const std::vector<std::unique_ptr<M>> &monitors);
	virtual ~Monitor_reduction_MPI() = default;

	virtual void reset();

protected:
	virtual void _reduce(bool fully = false);

private:
	static void MPI_reduce_monitors(void *in, void *inout, int *len, MPI_Datatype *datatype);
};
}
}
#include "Monitor_reduction_MPI.hxx"

#endif /* MONITOR_REDUCTION_MPI_HPP_ */

#endif
