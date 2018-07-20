#ifdef ENABLE_MPI

#ifndef MONITOR_REDUCTION_MPI_HXX_
#define MONITOR_REDUCTION_MPI_HXX_

#include <stddef.h>
#include <sstream>

#include "Tools/Exception/exception.hpp"

#include "Monitor_reduction_MPI.hpp"
#include "MI/Monitor_MI.hpp"

namespace aff3ct
{
namespace module
{

template<class M>
void MPI_SUM_monitor_vals_func(void *in, void *inout, int *len, MPI_Datatype *datatype)
{
	auto    in_cvt = static_cast<typename M::Vals_mpi*>(in   );
	auto inout_cvt = static_cast<typename M::Vals_mpi*>(inout);

	for (auto i = 0; i < *len; i++)
	{
		M m_in   (in_cvt   [i]);
		M m_inout(inout_cvt[i]);
		m_inout += m_in;
		inout_cvt[i] = m_inout.get_vals_mpi();
	}
}

template <class M>
Monitor_reduction_MPI<M>
::Monitor_reduction_MPI(const std::vector<M*> &monitors)
: Monitor_reduction_M<M>(monitors)
{
	const std::string name = "Monitor_reduction_MPI<" + monitors[0]->get_name() + ">";
	this->set_name(name);

	// int          blen         [M::n_MPI_attributes];
	// MPI_Aint     displacements[M::n_MPI_attributes];
	// MPI_Datatype oldtypes     [M::n_MPI_attributes];

	// M::create_MPI_struct(blen, displacements, oldtypes);

	// MPI_Datatype MPI_monitor_vals_tmp;

	// if (auto ret = MPI_Type_create_struct(M::n_MPI_attributes, blen, displacements, oldtypes, &MPI_monitor_vals_tmp))
	// {
	// 	std::stringstream message;
	// 	message << "'MPI_Type_create_struct' returned '" << ret << "' error code.";
	// 	throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	// }


	// MPI_Aint lb, extent;
	// MPI_Type_get_extent( MPI_monitor_vals_tmp, &lb, &extent );
	// extent = sizeof (M);
	// MPI_Type_create_resized( MPI_monitor_vals_tmp, lb, extent, &MPI_monitor_vals );

	int          blen          = sizeof(typename M::Vals_mpi);
	MPI_Aint     displacements = 0;
	MPI_Datatype oldtypes      = MPI_CHAR;

	if (auto ret = MPI_Type_create_struct(1, &blen, &displacements, &oldtypes, &MPI_monitor_vals))
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
bool Monitor_reduction_MPI<M>
::_reduce(bool fully, bool last)
{
	fully = false;

	Monitor_reduction_M<M>::_reduce(fully);

	typename M::Vals_mpi mvals_recv;
	auto mvals_send = this->get_vals_mpi();

	MPI_Allreduce(&mvals_send, &mvals_recv, 1,
	              MPI_monitor_vals, MPI_SUM_monitor_vals, MPI_COMM_WORLD);
	M::copy(mvals_recv);


	char last_recv = true;

	MPI_Allreduce(&last, &last_recv, 1,
	              MPI_CHAR, MPI_LAND, MPI_COMM_WORLD);

	return last_recv;
}

template <class M>
void Monitor_reduction_MPI<M>
::reset()
{
	Monitor_reduction_M<M>::reset();
}

}
}

#endif // MONITOR_REDUCTION_MPI_HXX_

#endif // ENABLE_MPI
