#include <string>
#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Module/Router/Router.hpp"

namespace aff3ct
{
namespace module
{

template <typename IN, typename OUT>
Task& Router<IN,OUT>
::operator[](const rtr::tsk t)
{
	return Module::operator[]((size_t)t);
}

template <typename IN, typename OUT>
Socket& Router<IN,OUT>
::operator[](const rtr::sck::route s)
{
	return Module::operator[]((size_t)rtr::tsk::route)[(size_t)s];
}

template <typename IN, typename OUT>
Router<IN,OUT>
::Router(const size_t n_elmts_in, const size_t n_elmts_out, const size_t n_outputs, const int n_frames)
: Module(n_frames),
  n_elmts_in(n_elmts_in),
  n_elmts_out(n_elmts_out),
  n_outputs(n_outputs)
{
	const std::string name = "Router";
	this->set_name(name);
	this->set_short_name(name);

	if (n_elmts_in == 0)
	{
		std::stringstream message;
		message << "'n_elmts_in' has to be greater than 0 ('n_elmts_in' = " << n_elmts_in << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (n_elmts_out == 0)
	{
		std::stringstream message;
		message << "'n_elmts_out' has to be greater than 0 ('n_elmts_out' = " << n_elmts_out << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (n_outputs == 0)
	{
		std::stringstream message;
		message << "'n_outputs' has to be greater than 0 ('n_outputs' = " << n_outputs << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	auto &p = this->create_task("route");
	auto ps_in = this->template create_socket_in<IN>(p, "in", this->n_elmts_in);
	auto ps_out = this->template create_socket_in<OUT>(p, "out", this->n_elmts_out);
	for (size_t o = 0; o < n_outputs; o++)
		this->template create_socket_out<OUT>(p, "out" + std::to_string(o), this->n_elmts_out);

	this->create_codelet(p, [ps_in, ps_out](Module &m, Task &t) -> int
	{
		auto sout_dataptr = t.sockets[ps_out]->get_dataptr();
		for (size_t sout = 2; sout < t.sockets.size(); sout++)
			t.sockets[sout]->bind(sout_dataptr);

		return (int)static_cast<Router<IN,OUT>&>(m).route(static_cast<IN*>(t[ps_in].get_dataptr()));
	});
}

template <typename IN, typename OUT>
size_t Router<IN,OUT>
::get_n_elmts_in() const
{
	return this->n_elmts_in;
}

template <typename IN, typename OUT>
size_t Router<IN,OUT>
::get_n_elmts_out() const
{
	return this->n_elmts_out;
}

template <typename IN, typename OUT>
size_t Router<IN,OUT>
::get_n_outputs() const
{
	return this->n_outputs;
}

template <typename IN, typename OUT>
template <class A>
size_t Router<IN,OUT>
::route(const std::vector<IN,A>& in, const int frame_id)
{
	if (this->n_elmts_in * this->n_frames != (int)in.size())
	{
		std::stringstream message;
		message << "'in.size()' has to be equal to 'n_elmts_in' * 'n_frames' ('in.size()' = " << in.size()
		        << ", 'n_elmts_in' = " << this->n_elmts_in << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	this->route(in.data(), frame_id);
}

template <typename IN, typename OUT>
size_t Router<IN,OUT>
::route(const IN *in, const int frame_id)
{
	const auto f_start = (frame_id < 0) ? 0 : frame_id % this->n_frames;
	const auto f_stop  = (frame_id < 0) ? this->n_frames : f_start +1;

	auto ret_val = this->_route(in + f_start * this->n_elmts_in, f_start);
	for (auto f = f_start+1; f < f_stop; f++)
		ret_val = this->select_route_inter(ret_val, this->_route(in + f * this->n_elmts_in, f));
	return ret_val;
}

template <typename IN, typename OUT>
size_t Router<IN,OUT>
::_route(const IN *in, const int frame_id)
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename IN, typename OUT>
size_t Router<IN,OUT>
::select_route_inter(const size_t a, const size_t b)
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

}
}
