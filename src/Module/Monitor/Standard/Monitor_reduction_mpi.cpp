#ifdef ENABLE_MPI

#include <stddef.h>

#include "Tools/Display/bash_tools.h"

#include "Monitor_reduction_mpi.hpp"

struct monitor_vals
{
	int n_be;
	int n_fe;
	unsigned long long n_fra;
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
	}
}

template <typename B, typename R>
Monitor_reduction_mpi<B,R>
::Monitor_reduction_mpi(const int& K, const int& N, const int& max_fe,
                        std::vector<Monitor<B,R>*>& monitors,
                        const std::thread::id master_thread_id,
                        const std::chrono::nanoseconds d_mpi_comm_frequency,
                        const int& n_frames,
                        const std::string name)
: Monitor_reduction<B,R>(K, N, max_fe, monitors, n_frames, name),
  master_thread_id(master_thread_id),
  is_fe_limit_achieved(false),
  t_last_mpi_comm(std::chrono::steady_clock::now()),
  d_mpi_comm_frequency(d_mpi_comm_frequency)
{
	int blen[3];
	MPI_Aint displacements[3];
	MPI_Datatype oldtypes[3];

	blen[0] = 1; displacements[0] = offsetof(monitor_vals, n_be);  oldtypes[0] = MPI_INT;
	blen[1] = 1; displacements[1] = offsetof(monitor_vals, n_fe);  oldtypes[1] = MPI_INT;
	blen[2] = 1; displacements[2] = offsetof(monitor_vals, n_fra); oldtypes[2] = MPI_UNSIGNED_LONG_LONG;

	if (auto ret = MPI_Type_create_struct(3, blen, displacements, oldtypes, &MPI_monitor_vals))
	{
		std::cerr << bold_red("(EE) MPI_Type_create_struct returned \"") << bold_red(std::to_string(ret))
		          << bold_red("\", exiting.") << std::endl;
		std::exit(-1);
	}

	if (auto ret = MPI_Type_commit(&MPI_monitor_vals))
	{
		std::cerr << bold_red("(EE) MPI_Type_commit returned \"") << bold_red(std::to_string(ret))
		          << bold_red("\", exiting.") << std::endl;
		std::exit(-1);
	}

	if (auto ret = MPI_Op_create(MPI_SUM_monitor_vals_func, true, &MPI_SUM_monitor_vals))
	{
		std::cerr << bold_red("(EE) MPI_Op_create returned \"") << bold_red(std::to_string(ret))
		          << bold_red("\", exiting.") << std::endl;
		std::exit(-1);
	}
}

template <typename B, typename R>
Monitor_reduction_mpi<B,R>
::~Monitor_reduction_mpi()
{
}

template <typename B, typename R>
bool Monitor_reduction_mpi<B,R>
::fe_limit_achieved()
{
	// only the master thread can do this
	if (std::this_thread::get_id() == this->master_thread_id &&
	    ((std::chrono::steady_clock::now() - t_last_mpi_comm) >= d_mpi_comm_frequency))
	{
		monitor_vals mvals_recv;
		monitor_vals mvals_send = { this->get_n_be()           - this->n_bit_errors,
		                            this->get_n_fe()           - this->n_frame_errors,
		                            this->get_n_analyzed_fra() - this->n_analyzed_frames };

		MPI_Allreduce(&mvals_send, &mvals_recv, 1, MPI_monitor_vals, MPI_SUM_monitor_vals, MPI_COMM_WORLD);

		this->n_bit_errors      = mvals_recv.n_be  - mvals_send.n_be;
		this->n_frame_errors    = mvals_recv.n_fe  - mvals_send.n_fe;
		this->n_analyzed_frames = mvals_recv.n_fra - mvals_send.n_fra;

		t_last_mpi_comm = std::chrono::steady_clock::now();

		is_fe_limit_achieved = mvals_recv.n_fe >= this->get_fe_limit();
	}

	return is_fe_limit_achieved;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Monitor_reduction_mpi<B_8 ,R_8>;
template class Monitor_reduction_mpi<B_16,R_16>;
template class Monitor_reduction_mpi<B_32,R_32>;
template class Monitor_reduction_mpi<B_64,R_64>;
#else
template class Monitor_reduction_mpi<B,R>;
#endif
// ==================================================================================== explicit template instantiation

#endif
