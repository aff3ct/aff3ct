#include <sstream>
#include <string>

#include "Tools/Exception/exception.hpp"
#include "Tools/Noise/Sigma.hpp"
#include "Module/Channel/Channel.hpp"

namespace aff3ct
{
namespace module
{

template <typename R>
Task& Channel<R>
::operator[](const chn::tsk t)
{
	return Module::operator[]((size_t)t);
}

template <typename R>
Socket& Channel<R>
::operator[](const chn::sck::add_noise s)
{
	return Module::operator[]((size_t)chn::tsk::add_noise)[(size_t)s];
}

template <typename R>
Socket& Channel<R>
::operator[](const chn::sck::add_noise_wg s)
{
	return Module::operator[]((size_t)chn::tsk::add_noise_wg)[(size_t)s];
}

template <typename R>
Channel<R>
::Channel(const int N)
: Module(), N(N), noised_data(this->N * this->n_frames, 0)
{
	const std::string name = "Channel";
	this->set_name(name);
	this->set_short_name(name);

	if (N <= 0)
	{
		std::stringstream message;
		message << "'N' has to be greater than 0 ('N' = " << N << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	auto &p1 = this->create_task("add_noise");
	auto p1s_CP  = this->template create_socket_in <float>(p1, "CP",        1);
	auto p1s_X_N = this->template create_socket_in <R    >(p1, "X_N", this->N);
	auto p1s_Y_N = this->template create_socket_out<R    >(p1, "Y_N", this->N);
	this->create_codelet(p1, [p1s_CP, p1s_X_N, p1s_Y_N](Module &m, Task &t, const size_t frame_id) -> int
	{
		auto &chn = static_cast<Channel<R>&>(m);

		chn._add_noise(static_cast<float*>(t[p1s_CP ].get_dataptr()),
		               static_cast<R    *>(t[p1s_X_N].get_dataptr()),
		               static_cast<R    *>(t[p1s_Y_N].get_dataptr()),
		               frame_id);

		return status_t::SUCCESS;
	});

	auto &p2 = this->create_task("add_noise_wg");
	auto p2s_CP  = this->template create_socket_in <float>(p2, "CP",        1);
	auto p2s_X_N = this->template create_socket_in <R    >(p2, "X_N", this->N);
	auto p2s_H_N = this->template create_socket_out<R    >(p2, "H_N", this->N);
	auto p2s_Y_N = this->template create_socket_out<R    >(p2, "Y_N", this->N);
	this->create_codelet(p2, [p2s_CP, p2s_X_N, p2s_H_N, p2s_Y_N](Module &m, Task &t, const size_t frame_id) -> int
	{
		auto &chn = static_cast<Channel<R>&>(m);

		chn._add_noise_wg(static_cast<float*>(t[p2s_CP ].get_dataptr()),
		                  static_cast<R    *>(t[p2s_X_N].get_dataptr()),
		                  static_cast<R    *>(t[p2s_H_N].get_dataptr()),
		                  static_cast<R    *>(t[p2s_Y_N].get_dataptr()),
		                  frame_id);

		return status_t::SUCCESS;
	});
}

template <typename R>
Channel<R>* Channel<R>
::clone() const
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename R>
int Channel<R>
::get_N() const
{
	return this->N;
}

template <typename R>
const std::vector<R>& Channel<R>
::get_noised_data() const
{
	return this->noised_data;
}

template <typename R>
void Channel<R>
::set_seed(const int seed)
{
	// do nothing in the general case, this method has to be overrided
}

template <typename R>
template <class A>
void Channel<R>
::add_noise(const std::vector<float,A>& CP, const std::vector<R,A>& X_N, std::vector<R,A>& Y_N, const int frame_id,
            const bool managed_memory)
{
	(*this)[chn::sck::add_noise::CP ].bind(CP );
	(*this)[chn::sck::add_noise::X_N].bind(X_N);
	(*this)[chn::sck::add_noise::Y_N].bind(Y_N);
	(*this)[chn::tsk::add_noise].exec(frame_id, managed_memory);
}

template <typename R>
void Channel<R>
::add_noise(const float *CP, const R *X_N, R *Y_N, const int frame_id, const bool managed_memory)
{
	(*this)[chn::sck::add_noise::CP ].bind(CP );
	(*this)[chn::sck::add_noise::X_N].bind(X_N);
	(*this)[chn::sck::add_noise::Y_N].bind(Y_N);
	(*this)[chn::tsk::add_noise].exec(frame_id, managed_memory);
}

template <typename R>
template <class A>
void Channel<R>
::add_noise_wg(const std::vector<float,A>& CP, const std::vector<R,A>& X_N, std::vector<R,A>& H_N,
               std::vector<R,A>& Y_N, const int frame_id, const bool managed_memory)
{
	(*this)[chn::sck::add_noise_wg::CP ].bind(CP );
	(*this)[chn::sck::add_noise_wg::X_N].bind(X_N);
	(*this)[chn::sck::add_noise_wg::H_N].bind(H_N);
	(*this)[chn::sck::add_noise_wg::Y_N].bind(Y_N);
	(*this)[chn::tsk::add_noise_wg].exec(frame_id, managed_memory);
}

template <typename R>
void Channel<R>
::add_noise_wg(const float *CP, const R *X_N, R *H_N, R *Y_N, const int frame_id, const bool managed_memory)
{
	(*this)[chn::sck::add_noise_wg::CP ].bind(CP );
	(*this)[chn::sck::add_noise_wg::X_N].bind(X_N);
	(*this)[chn::sck::add_noise_wg::H_N].bind(H_N);
	(*this)[chn::sck::add_noise_wg::Y_N].bind(Y_N);
	(*this)[chn::tsk::add_noise_wg].exec(frame_id, managed_memory);
}

template <typename R>
void Channel<R>
::_add_noise(const float *CP, const R *X_N, R *Y_N, const size_t frame_id)
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename R>
void Channel<R>
::_add_noise_wg(const float *CP, const R *X_N, R *H_N, R *Y_N, const size_t frame_id)
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template<typename R>
void Channel<R>
::set_n_frames(const size_t n_frames)
{
	const auto old_n_frames = this->get_n_frames();
	if (old_n_frames != n_frames)
	{
		Module::set_n_frames(n_frames);

		const auto old_noised_data_size = this->noised_data.size();
		const auto new_noised_data_size = (old_noised_data_size / old_n_frames) * n_frames;
		this->noised_data.resize(new_noised_data_size);
	}
}

}
}
