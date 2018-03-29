#ifdef ENABLE_MPI

#include <stddef.h>
#include <sstream>

#include "Tools/Exception/exception.hpp"

#include "Monitor_BFER_reduction_mpi.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

struct monitor_vals
{
	unsigned long long n_be;
	unsigned long long n_fe;
	unsigned long long n_fra;
	float              MI_sum;
};

void MPI_SUM_monitor_vals_func(void *in, void *inout, int *len, MPI_Datatype *datatype)
{
	auto    in_cvt = static_cast<monitor_vals*>(in   );
	auto inout_cvt = static_cast<monitor_vals*>(inout);

	for (auto i = 0; i < *len; i++)
	{
		inout_cvt[i].n_be  += in_cvt[i].n_be;
		inout_cvt[i].n_fe  += in_cvt[i].n_fe;

		inout_cvt[i].n_fra += in_cvt[i].n_fra;
		inout_cvt[i].MI_sum+= inout_cvt[i].MI_sum;
	}
}

template <typename B, typename R>
Monitor_BFER_reduction_mpi<B,R>
::Monitor_BFER_reduction_mpi(const std::vector<Monitor_BFER<B,R>*> &monitors,
                             const std::thread::id master_thread_id,
                             const std::chrono::nanoseconds d_mpi_comm_frequency)
: Monitor_BFER_reduction<B,R>(monitors),
  master_thread_id(master_thread_id),
  is_fe_limit_achieved(false),
  t_last_mpi_comm(std::chrono::steady_clock::now()),
  d_mpi_comm_frequency(d_mpi_comm_frequency)
{
	const std::string name = "Monitor_BFER_reduction_mpi";
	this->set_name(name);

	int blen[4];
	MPI_Aint displacements[4];
	MPI_Datatype oldtypes[4];

	blen[0] = 1; displacements[0] = offsetof(monitor_vals, n_be  ); oldtypes[0] = MPI_UNSIGNED_LONG_LONG;
	blen[1] = 1; displacements[1] = offsetof(monitor_vals, n_fe  ); oldtypes[1] = MPI_UNSIGNED_LONG_LONG;
	blen[2] = 1; displacements[2] = offsetof(monitor_vals, n_fra ); oldtypes[2] = MPI_UNSIGNED_LONG_LONG;
	blen[3] = 1; displacements[3] = offsetof(monitor_vals, MI_sum); oldtypes[3] = MPI_FLOAT;

	if (auto ret = MPI_Type_create_struct(4, blen, displacements, oldtypes, &MPI_monitor_vals))
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

	if (auto ret = MPI_Op_create(MPI_SUM_monitor_vals_func, true, &MPI_SUM_monitor_vals))
	{
		std::stringstream message;
		message << "'MPI_Op_create' returned '" << ret << "' error code.";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename B, typename R>
Monitor_BFER_reduction_mpi<B,R>
::~Monitor_BFER_reduction_mpi()
{
}

template <typename B, typename R>
bool Monitor_BFER_reduction_mpi<B,R>
::fe_limit_achieved()
{
	// only the master thread can do this
	if (std::this_thread::get_id() == this->master_thread_id &&
	    ((std::chrono::steady_clock::now() - t_last_mpi_comm) >= d_mpi_comm_frequency))
	{
		monitor_vals mvals_recv;
		monitor_vals mvals_send = { this->get_n_be()           - this->n_bit_errors,
		                            this->get_n_fe()           - this->n_frame_errors,
		                            this->get_n_analyzed_fra() - this->n_analyzed_frames,
		                            this->get_MI_sum()         - this->MI_sum };

		MPI_Allreduce(&mvals_send, &mvals_recv, 1, MPI_monitor_vals, MPI_SUM_monitor_vals, MPI_COMM_WORLD);

		this->n_bit_errors      = mvals_recv.n_be   - mvals_send.n_be;
		this->n_frame_errors    = mvals_recv.n_fe   - mvals_send.n_fe;
		this->n_analyzed_frames = mvals_recv.n_fra  - mvals_send.n_fra;
		this->MI_sum            = mvals_recv.MI_sum - mvals_send.MI_sum;

		t_last_mpi_comm = std::chrono::steady_clock::now();

		is_fe_limit_achieved = mvals_recv.n_fe >= this->get_fe_limit();
	}

	return is_fe_limit_achieved;
}

template <typename B, typename R>
void Monitor_BFER_reduction_mpi<B,R>
::reset()
{
	Monitor_BFER_reduction<B,R>::reset();
	is_fe_limit_achieved = false;
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Monitor_BFER_reduction_mpi<B_8, Q_8>;
template class aff3ct::module::Monitor_BFER_reduction_mpi<B_16,Q_16>;
template class aff3ct::module::Monitor_BFER_reduction_mpi<B_32,Q_32>;
template class aff3ct::module::Monitor_BFER_reduction_mpi<B_64,Q_64>;
#else
template class aff3ct::module::Monitor_BFER_reduction_mpi<B,Q>;
#endif
// ==================================================================================== explicit template instantiation

#endif
