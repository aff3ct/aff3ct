#ifdef ENABLE_MPI

#ifndef MONITOR_REDUCTION_MPI_HXX_
#define MONITOR_REDUCTION_MPI_HXX_

#include <stddef.h>
#include <sstream>

#include "Tools/Exception/exception.hpp"

#include "Monitor_reduction_mpi.hpp"

using namespace aff3ct;
using namespace aff3ct::module;


template<class M>
void MPI_SUM_monitor_vals_func(void *in, void *inout, int *len, MPI_Datatype *datatype)
{
	auto    in_cvt = static_cast<M::Values_t*>(in   );
	auto inout_cvt = static_cast<M::Values_t*>(inout);

	for (auto i = 0; i < *len; i++)
		inout_cvt[i] += in_cvt[i]
}

template <class M>
Monitor_reduction_mpi<M>
::Monitor_reduction_mpi(const std::vector<M*> &monitors,
                        const std::thread::id master_thread_id,
                        const std::chrono::nanoseconds d_mpi_comm_frequency)
: Monitor_reduction<M>(monitors),
  master_thread_id(master_thread_id),
  t_last_mpi_comm(std::chrono::steady_clock::now()),
  d_mpi_comm_frequency(d_mpi_comm_frequency)
{
	const std::string name = "Monitor_reduction_mpi<" + monitors[0]->get_name() + ">";
	this->set_name(name);

	int          blen         [M::vals::n_attributes];
	MPI_Aint     displacements[M::vals::n_attributes];
	MPI_Datatype oldtypes     [M::vals::n_attributes];

	M::vals::create_MPI_struct(blen, displacements, oldtypes);

	if (auto ret = MPI_Type_create_struct(M::vals::n_attributes, blen, displacements, oldtypes, &MPI_monitor_vals))
	{
		std::stringstream message;
		message << "'MPI_Type_create_struct' returned '" << ret << "' error code.";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (auto ret = MPI_Type_commit(&MPI_monitor_vals))
	{
		std::stringstream message;
		message << "'MPI_Type_commit' returned '" << ret << "' error code.";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (auto ret = MPI_Op_create(MPI_SUM_monitor_vals_func<M>, true, &MPI_SUM_monitor_vals))
	{
		std::stringstream message;
		message << "'MPI_Op_create' returned '" << ret << "' error code.";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}
}

template <class M>
void Monitor_reduction_mpi<M>
::reduce(bool fully)
{
	// only the master thread can do this
	if (std::this_thread::get_id() == this->master_thread_id &&
	    ((std::chrono::steady_clock::now() - t_last_mpi_comm) >= d_mpi_comm_frequency))
	{
		Monitor_reduction<M>::reduce(fully);

		M::Values_t mvals_recv;
		auto& mvals_send = this->vals;

		MPI_Allreduce(&mvals_send, &mvals_recv, 1, MPI_monitor_vals, MPI_SUM_monitor_vals, MPI_COMM_WORLD);

		this->vals = mvals_recv;

		t_last_mpi_comm = std::chrono::steady_clock::now();
	}
}

#endif // MONITOR_REDUCTION_MPI_HXX_

#endif // ENABLE_MPI
