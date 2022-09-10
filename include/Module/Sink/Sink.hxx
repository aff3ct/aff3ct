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
Socket& Sink<B>
::operator[](const snk::sck::send_k s)
{
	return Module::operator[]((size_t)snk::tsk::send_k)[(size_t)s];
}

template <typename B>
Sink<B>
::Sink(const int K)
: Module(), K(K)
{
	const std::string name = "Sink";
	this->set_name(name);
	this->set_short_name(name);

	if (K <= 0)
	{
		std::stringstream message;
		message << "'K' has to be greater than 0 ('K' = " << K << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	auto &p1 = this->create_task("send");
	auto p1s_V = this->template create_socket_in<B>(p1, "V", K);
	this->create_codelet(p1, [p1s_V](Module& m, Task& t, const size_t frame_id) -> int
	{
		auto &snk = static_cast<Sink<B>&>(m);

		snk._send(static_cast<const B*>(t[p1s_V].get_dataptr()),
		          frame_id);

		return status_t::SUCCESS;
	});

	auto &p2 = this->create_task("send_k");
	auto p2s_V = this->template create_socket_in<B>(p2, "V", K);
	auto p2s_real_K = this->template create_socket_in<uint32_t>(p2, "real_K", 1);
	this->create_codelet(p2, [p2s_V, p2s_real_K](Module& m, Task& t, const size_t frame_id) -> int
	{
		auto &snk = static_cast<Sink<B>&>(m);

		snk._send_k(static_cast<const B*>(t[p2s_V].get_dataptr()),
			        static_cast<const uint32_t*>(t[p2s_real_K].get_dataptr()),
		            frame_id);

		return status_t::SUCCESS;
	});
}

template <typename B>
Sink<B>* Sink<B>
::clone() const
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename B>
template <class A>
void Sink<B>
::send(const std::vector<B,A>& V, const int frame_id, const bool managed_memory)
{
	(*this)[snk::sck::send::V].bind(V);
	(*this)[snk::tsk::send].exec(frame_id, managed_memory);
}

template <typename B>
void Sink<B>
::send(const B *V, const int frame_id, const bool managed_memory)
{
	(*this)[snk::sck::send::V].bind(V);
	(*this)[snk::tsk::send].exec(frame_id, managed_memory);
}

template <typename B>
template <class A>
void Sink<B>
::send_k(const std::vector<B,A>& V, const std::vector<uint32_t>& real_K, const int frame_id, const bool managed_memory)
{
	(*this)[snk::sck::send_k::V].bind(V);
	(*this)[snk::sck::send_k::real_K].bind(real_K);
	(*this)[snk::tsk::send_k].exec(frame_id, managed_memory);
}

template <typename B>
void Sink<B>
::send_k(const B *V, uint32_t* real_K, const int frame_id, const bool managed_memory)
{
	(*this)[snk::sck::send_k::V].bind(V);
	(*this)[snk::sck::send_k::real_K].bind(real_K);
	(*this)[snk::tsk::send_k].exec(frame_id, managed_memory);
}

template <typename B>
void Sink<B>
::_send(const B *V, const size_t frame_id)
{
	const uint32_t real_K = (uint32_t)this->K;
	this->_send_k(V, &real_K, frame_id);
}

template <typename B>
void Sink<B>
::_send_k(const B *V, const uint32_t* real_K, const size_t frame_id)
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename B>
void Sink<B>
::reset()
{
	// do nothing in the general case, this method has to be overrided
}

}
}
