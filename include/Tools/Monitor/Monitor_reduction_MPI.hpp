/*!
 * \file
 * \brief Class module::Monitor_reduction_MPI.
 */
#ifdef AFF3CT_MPI

#ifndef MONITOR_REDUCTION_MPI_HPP_
#define MONITOR_REDUCTION_MPI_HPP_

#include <vector>
#include <memory>
#include <mpi.h>

#include "Tools/Monitor/Monitor_reduction.hpp"

namespace aff3ct
{
namespace tools
{
template <class M> // M is the monitor on which must be applied the reduction
class Monitor_reduction_MPI : public Monitor_reduction<M>
{
protected:
	using Attributes = typename M::Attributes;

private:
	MPI_Datatype MPI_monitor_vals;
	MPI_Op       MPI_Op_reduce_monitors;

public:
	explicit Monitor_reduction_MPI(const std::vector<M*> &monitors);
	explicit Monitor_reduction_MPI(const std::vector<std::unique_ptr<M>> &monitors);
	explicit Monitor_reduction_MPI(const std::vector<std::shared_ptr<M>> &monitors);
	virtual ~Monitor_reduction_MPI();

	virtual bool is_done();

	virtual void reduce(bool fully = false);

protected:
	virtual bool _is_done();

private:
	static void MPI_reduce_monitors(void *in, void *inout, int *len, MPI_Datatype *datatype);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Tools/Monitor/Monitor_reduction_MPI.hxx"
#endif

#endif /* MONITOR_REDUCTION_MPI_HPP_ */

#endif
