#ifndef SC_CHANNELS_HPP_
#define SC_CHANNELS_HPP_

#ifdef SYSTEMC
#include <vector>
#include <string>
#include <cassert>
#include <systemc>
#include <tlm>
#include <tlm_utils/simple_target_socket.h>
#include <tlm_utils/simple_initiator_socket.h>

#include "Tools/MIPP/mipp.h"

template <typename R>
class SC_Channel;

template <typename R>
class SC_Channel_module : public sc_core::sc_module
{
	SC_HAS_PROCESS(SC_Channel_module);

public:
	tlm_utils::simple_target_socket   <SC_Channel_module> s_in;
	tlm_utils::simple_initiator_socket<SC_Channel_module> s_out;

private:
	SC_Channel<R> &channel;
	mipp::vector<R> X_N;
	mipp::vector<R> Y_N;

public:
	SC_Channel_module(SC_Channel<R> &channel, const sc_core::sc_module_name name = "SC_Channel_module")
	: sc_module(name), s_in("s_in"), s_out("s_out"),
	  channel(channel),
	  X_N(channel.N * channel.n_frames),
	  Y_N(channel.N * channel.n_frames)
	{
		s_in.register_b_transport(this, &SC_Channel_module::b_transport);
	}

	void resize_buffers()
	{
		if ((int)X_N.size() != channel.N * channel.n_frames) X_N.resize(channel.N * channel.n_frames);
		if ((int)Y_N.size() != channel.N * channel.n_frames) Y_N.resize(channel.N * channel.n_frames);
	}

private:
	void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		assert((trans.get_data_length() / sizeof(R)) == X_N.size());

		const R* buffer_in = (R*)trans.get_data_ptr();
		std::copy(buffer_in, buffer_in + X_N.size(), X_N.begin());

		channel.add_noise(X_N, Y_N);

		tlm::tlm_generic_payload payload;
		payload.set_data_ptr((unsigned char*)Y_N.data());
		payload.set_data_length(Y_N.size() * sizeof(R));

		sc_core::sc_time zero_time(sc_core::SC_ZERO_TIME);
		s_out->b_transport(payload, zero_time);
	}
};

template <typename R>
class SC_Channel : public Channel_interface<R>
{
	friend SC_Channel_module<R>;

public:
	SC_Channel_module<R> *module;

public:
	SC_Channel(const int N, const int n_frames = 1, const std::string name = "SC_Channel")
	: Channel_interface<R>(N, n_frames, name), module(nullptr) {}

	virtual ~SC_Channel() { if (module != nullptr) { delete module; module = nullptr; } };

	virtual void add_noise(const mipp::vector<R>& X_N, mipp::vector<R>& Y_N) = 0;

	virtual void set_n_frames(const int n_frames)
	{
		Channel_interface<R>::set_n_frames(n_frames);

		if (module != nullptr)
			module->resize_buffers();
	}

	void create_sc_module()
	{
		this->module = new SC_Channel_module<R>(*this, this->name.c_str());
	}
};

template <typename R>
using Channel = SC_Channel<R>;
#else
template <typename R>
using Channel = Channel_interface<R>;
#endif

#endif /* SC_CHANNELS_HPP_ */
