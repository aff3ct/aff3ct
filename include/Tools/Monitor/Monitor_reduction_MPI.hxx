#ifdef AFF3CT_MPI

#ifndef MONITOR_REDUCTION_MPI_HXX_
#define MONITOR_REDUCTION_MPI_HXX_

#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Tools/Monitor/Monitor_reduction_MPI.hpp"

namespace aff3ct
{
namespace tools
{

template <class M>
Monitor_reduction_MPI<M>
::Monitor_reduction_MPI(const std::vector<M*> &monitors)
: Monitor_reduction<M>(monitors)
{
	const std::string name = "Monitor_reduction_MPI<" + monitors[0]->get_name() + ">";
	this->set_name(name);

	int          blen          = sizeof(Attributes);
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

	if (auto ret = MPI_Op_create(MPI_reduce_monitors, true, &MPI_Op_reduce_monitors))
	{
		std::stringstream message;
		message << "'MPI_Op_create' returned '" << ret << "' error code.";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}
}

template <class M>
Monitor_reduction_MPI<M>
::Monitor_reduction_MPI(const std::vector<std::unique_ptr<M>> &monitors)
: Monitor_reduction_MPI(convert_to_ptr<M>(monitors))
{
}

template <class M>
Monitor_reduction_MPI<M>
::Monitor_reduction_MPI(const std::vector<std::shared_ptr<M>> &monitors)
: Monitor_reduction_MPI(convert_to_ptr<M>(monitors))
{
}

template <class M>
Monitor_reduction_MPI<M>
::~Monitor_reduction_MPI()
{
	MPI_Type_free(&MPI_monitor_vals);
	MPI_Op_free(&MPI_Op_reduce_monitors);
}

template <class M>
bool Monitor_reduction_MPI<M>
::is_done()
{
	std::stringstream message;
	message << "'is_done' method is not available in MPI, please use the static 'is_done_all' method instead.";
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__, message.str());
}

template <class M>
bool Monitor_reduction_MPI<M>
::_is_done()
{
	return M::is_done();
}

template <class M>
void Monitor_reduction_MPI<M>
::reduce(bool fully)
{
	fully = false;

	Monitor_reduction<M>::reduce(fully);

	Attributes mvals_send = M::get_attributes(), mvals_recv;
	if (auto ret = MPI_Allreduce(&mvals_send, &mvals_recv, 1, MPI_monitor_vals, MPI_Op_reduce_monitors, MPI_COMM_WORLD))
	{
		std::stringstream message;
		message << "'MPI_Allreduce' returned '" << ret << "' error code.";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	M::copy(mvals_recv);
}

template <class M>
void Monitor_reduction_MPI<M>
::MPI_reduce_monitors(void *in, void *inout, int *len, MPI_Datatype *datatype)
{
	auto    in_cvt = static_cast<Attributes*>(in   );
	auto inout_cvt = static_cast<Attributes*>(inout);

	for (auto i = 0; i < *len; i++)
		inout_cvt[i] += in_cvt[i];
}

}
}

#endif // MONITOR_REDUCTION_MPI_HXX_

#endif // AFF3CT_MPI
