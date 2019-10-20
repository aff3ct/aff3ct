#include <string>
#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Module/Quantizer/Quantizer.hpp"

namespace aff3ct
{
namespace module
{

template <typename R, typename Q>
Quantizer<R,Q>::
Quantizer(const int N, const int n_frames)
: Module(n_frames), N(N)
{
	const std::string name = "Quantizer";
	this->set_name(name);
	this->set_short_name(name);

	if (N <= 0)
	{
		std::stringstream message;
		message << "'N' has to be greater than 0 ('N' = " << N << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	auto &p = this->create_task("process");
	auto &ps_Y_N1 = this->template create_socket_in <R>(p, "Y_N1", this->N * this->n_frames);
	auto &ps_Y_N2 = this->template create_socket_out<Q>(p, "Y_N2", this->N * this->n_frames);
	this->create_codelet(p, [this, &ps_Y_N1, &ps_Y_N2]() -> int
	{
		this->process(static_cast<R*>(ps_Y_N1.get_dataptr()),
		              static_cast<Q*>(ps_Y_N2.get_dataptr()));

		return 0;
	});

}

template <typename R, typename Q>
int Quantizer<R,Q>::
get_N() const
{
	return N;
}

template <typename R, typename Q>
template <class AR, class AQ>
void Quantizer<R,Q>::
process(const std::vector<R,AR>& Y_N1, std::vector<Q,AQ>& Y_N2, const int frame_id)
{
	if (this->N * this->n_frames != (int)Y_N1.size())
	{
		std::stringstream message;
		message << "'Y_N1.size()' has to be equal to 'N' * 'n_frames' ('Y_N1.size()' = " << Y_N1.size()
		        << ", 'N' = " << this->N << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (this->N * this->n_frames != (int)Y_N2.size())
	{
		std::stringstream message;
		message << "'Y_N2.size()' has to be equal to 'N' * 'n_frames' ('Y_N2.size()' = " << Y_N2.size()
		        << ", 'N' = " << this->N << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (frame_id != -1 && frame_id >= this->n_frames)
	{
		std::stringstream message;
		message << "'frame_id' has to be equal to '-1' or to be smaller than 'n_frames' ('frame_id' = "
		        << frame_id << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	this->process(Y_N1.data(), Y_N2.data(), frame_id);
}

template <typename R, typename Q>
void Quantizer<R,Q>::
process(const R *Y_N1, Q *Y_N2, const int frame_id)
{
	const auto f_start = (frame_id < 0) ? 0 : frame_id % this->n_frames;
	const auto f_stop  = (frame_id < 0) ? this->n_frames : f_start +1;

	for (auto f = f_start; f < f_stop; f++)
		this->_process(Y_N1 + f * this->N,
		               Y_N2 + f * this->N,
		               f);
}

template <typename R, typename Q>
void Quantizer<R,Q>::
_process(const R *Y_N1, Q *Y_N2, const int frame_id)
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

}
}
