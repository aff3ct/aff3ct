#include "Module/Router/Router.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

size_t Router
::route(const int8_t *in, const int frame_id)
{
	const auto f_start = (frame_id < 0) ? 0 : frame_id % this->n_frames;
	const auto f_stop  = (frame_id < 0) ? this->n_frames : f_start +1;

	auto ret_val = this->_route(in + f_start * this->n_bytes_in, f_start);
	for (auto f = f_start+1; f < f_stop; f++)
		ret_val = this->select_route_inter(ret_val, this->_route(in + f * this->n_bytes_in, f));
	return ret_val;
}

size_t Router
::_route(const int8_t *in, const int frame_id)
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

size_t Router
::select_route_inter(const size_t a, const size_t b)
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

void Router
::reset()
{
}