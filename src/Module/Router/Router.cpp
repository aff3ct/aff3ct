#include "Module/Router/Router.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

size_t Router
::compute_bytes(const size_t n_elmts, const std::type_index type)
{
	     if (type == typeid(int8_t )) return n_elmts * sizeof(int8_t );
	else if (type == typeid(int16_t)) return n_elmts * sizeof(int16_t);
	else if (type == typeid(int32_t)) return n_elmts * sizeof(int32_t);
	else if (type == typeid(int64_t)) return n_elmts * sizeof(int64_t);
	else if (type == typeid(float  )) return n_elmts * sizeof(float  );
	else if (type == typeid(double )) return n_elmts * sizeof(double );
	else
	{
		std::stringstream message;
		message << "This should never happen.";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}
}

size_t Router
::route(const int8_t *in, const int frame_id, const bool managed_memory)
{
	(*this)[rtr::sck::route::in].bind(in);
	return (size_t)(*this)[rtr::tsk::route].exec(frame_id, managed_memory);
}

size_t Router
::_route(const int8_t *in, const int frame_id)
{
	auto ret_val = this->__route(in + 0 * this->n_bytes_in, 0);
	for (auto f = 1; f < this->get_n_frames(); f++)
		ret_val = this->select_route_inter(ret_val, this->__route(in + f * this->n_bytes_in, f));
	return ret_val;
}

size_t Router
::__route(const int8_t *in, const int frame_id)
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