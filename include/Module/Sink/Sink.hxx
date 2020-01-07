#include <sstream>
#include <string>

#include "Tools/Exception/exception.hpp"
#include "Module/Sink/Sink.hpp"

namespace aff3ct
{
namespace module
{

template <typename B>
Task& Sink<B>
::operator[](const snk::tsk t)
{
	return Module::operator[]((size_t)t);
}

template <typename B>
Socket& Sink<B>
::operator[](const snk::sck::send s)
{
	return Module::operator[]((size_t)snk::tsk::send)[(size_t)s];
}

template <typename B>
Sink<B>
::Sink(const int N, const int n_frames)
: Module(n_frames), N(N)
{
	const std::string name = "Sink";
	this->set_name(name);
	this->set_short_name(name);

	if (N <= 0)
	{
		std::stringstream message;
		message << "'N' has to be greater than 0 ('N' = " << N << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	auto &p1 = this->create_task("send");
	auto p1s_V = this->template create_socket_in <B>(p1, "V", N);
	this->create_codelet(p1, [p1s_V](Module& m, Task& t) -> int
	{
		static_cast<Sink<B>&>(m).send(static_cast<B*>(t[p1s_V].get_dataptr()));
		return 0;
	});
}

template <typename B>
template <class A>
void Sink<B>
::send(const std::vector<B,A>& V, const int frame_id)
{
	if (this->N * this->n_frames != (int)V.size())
	{
		std::stringstream message;
		message << "'V.size()' has to be equal to 'N' * 'n_frames' ('V.size()' = " << V.size()
		        << ", 'N' = " << this->N << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	this->send(V.data(), frame_id);
}

template <typename B>
void Sink<B>
::send(const B *V, const int frame_id)
{
	const auto f_start = (frame_id < 0) ? 0 : frame_id % this->n_frames;
	const auto f_stop  = (frame_id < 0) ? this->n_frames : f_start +1;

	for (auto f = f_start; f < f_stop; f++)
		this->_send(V + f * this->N, f);
}

template <typename B>
void Sink<B>
::_send(const B *V, const int frame_id)
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

}
}
