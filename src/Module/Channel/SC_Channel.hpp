#ifndef SC_CHANNELS_HPP_
#define SC_CHANNELS_HPP_

#ifdef SYSTEMC_MODULE
#include <vector>
#include <string>
#include <sstream>
#include <systemc>
#include <tlm>
#include <tlm_utils/simple_target_socket.h>
#include <tlm_utils/simple_initiator_socket.h>
#include <mipp.h>

#include "Tools/Exception/exception.hpp"

namespace aff3ct
{
namespace module
{
template <typename R>
class SC_Channel;

template <typename R = float>
class SC_Channel_module : public sc_core::sc_module
{
public:
	tlm_utils::simple_target_socket   <SC_Channel_module> s_in;
	tlm_utils::simple_initiator_socket<SC_Channel_module> s_out;

private:
	SC_Channel<R> &channel;
	mipp::vector<R> Y_N;

public:
	SC_Channel_module(SC_Channel<R> &channel, const sc_core::sc_module_name name = "SC_Channel_module")
	: sc_module(name), s_in("s_in"), s_out("s_out"),
	  channel(channel),
	  Y_N(channel.get_N() * channel.get_n_frames())
	{
		s_in.register_b_transport(this, &SC_Channel_module::b_transport);
	}

	const mipp::vector<R>& get_Y_N()
	{
		return Y_N;
	}

private:
	void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		if (channel.get_N() * channel.get_n_frames() != (int)(trans.get_data_length() / sizeof(R)))
		{
			std::stringstream message;
			message << "'channel.get_N()' * 'channel.get_n_frames()' has to be equal to 'trans.get_data_length()' / "
			           "'sizeof(R)' ('channel.get_N()' = " << channel.get_N()
			        << ", 'channel.get_n_frames()' = " << channel.get_n_frames() << ", 'trans.get_data_length()' = "
			        << trans.get_data_length() << ", 'sizeof(R)' = " << sizeof(R) << ").";
			throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
		}

		const auto X_N = (R*)trans.get_data_ptr();

		channel.add_noise(X_N, Y_N.data());

		tlm::tlm_generic_payload payload;
		payload.set_data_ptr((unsigned char*)Y_N.data());
		payload.set_data_length(Y_N.size() * sizeof(R));

		sc_core::sc_time zero_time(sc_core::SC_ZERO_TIME);
		s_out->b_transport(payload, zero_time);
	}
};

template <typename R>
class SC_Channel_module_wg : public sc_core::sc_module
{
	SC_HAS_PROCESS(SC_Channel_module_wg);

public:
	tlm_utils::simple_target_socket   <SC_Channel_module_wg> s_in;
	tlm_utils::simple_initiator_socket<SC_Channel_module_wg> s_out1;
	tlm_utils::simple_initiator_socket<SC_Channel_module_wg> s_out2;

private:
	SC_Channel<R> &channel;
	mipp::vector<R> Y_N;
	mipp::vector<R> H_N;

public:
	SC_Channel_module_wg(SC_Channel<R> &channel,
	                     const sc_core::sc_module_name name = "SC_Channel_module_wg")
	: sc_module(name), s_in("s_in"), s_out1("s_out1"), s_out2("s_out2"),
	  channel(channel),
	  Y_N(channel.get_N() * channel.get_n_frames()),
	  H_N(channel.get_N() * channel.get_n_frames())
	{
		s_in.register_b_transport(this, &SC_Channel_module_wg::b_transport);
	}

	const mipp::vector<R>& get_Y_N()
	{
		return Y_N;
	}

	const mipp::vector<R>& get_H_N()
	{
		return H_N;
	}

private:
	void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		if (channel.get_N() * channel.get_n_frames() != (int)(trans.get_data_length() / sizeof(R)))
		{
			std::stringstream message;
			message << "'channel.get_N()' * 'channel.get_n_frames()' has to be equal to 'trans.get_data_length()' / "
			           "'sizeof(R)' ('channel.get_N()' = " << channel.get_N()
			        << ", 'channel.get_n_frames()' = " << channel.get_n_frames() << ", 'trans.get_data_length()' = "
			        << trans.get_data_length() << ", 'sizeof(R)' = " << sizeof(R) << ").";
			throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
		}

		const auto X_N = (R*)trans.get_data_ptr();

		channel.add_noise(X_N, Y_N.data(), H_N.data());

		tlm::tlm_generic_payload payload1;
		payload1.set_data_ptr((unsigned char*)H_N.data());
		payload1.set_data_length(H_N.size() * sizeof(R));

		sc_core::sc_time zero_time(sc_core::SC_ZERO_TIME);
		s_out1->b_transport(payload1, zero_time);

		tlm::tlm_generic_payload payload2;
		payload2.set_data_ptr((unsigned char*)Y_N.data());
		payload2.set_data_length(Y_N.size() * sizeof(R));

		s_out2->b_transport(payload2, zero_time);
	}
};

template <typename R>
class SC_Channel : public Channel_i<R>
{
public:
	SC_Channel_module   <R> *sc_module;
	SC_Channel_module_wg<R> *sc_module_wg;

public:
	SC_Channel(const int N, const R sigma, const int n_frames = 1, const std::string name = "SC_Channel")
	: Channel_i<R>(N, sigma, n_frames, name), sc_module(nullptr), sc_module_wg(nullptr) {}

	virtual ~SC_Channel()
	{
		if (sc_module    != nullptr) { delete sc_module;    sc_module    = nullptr; }
		if (sc_module_wg != nullptr) { delete sc_module_wg; sc_module_wg = nullptr; }
	};

	void create_sc_module()
	{
		if (sc_module != nullptr) { delete sc_module; sc_module = nullptr; }
		this->sc_module = new SC_Channel_module<R>(*this, this->name.c_str());
	}

	void create_sc_module_wg()
	{
		if (sc_module_wg != nullptr) { delete sc_module_wg; sc_module_wg = nullptr; }
		const std::string new_name = this->name + "_wg";
		this->sc_module_wg = new SC_Channel_module_wg<R>(*this, new_name.c_str());
	}
};

template <typename R = float>
using Channel = SC_Channel<R>;
}
}
#else
#include "SPU_Channel.hpp"
#endif

#endif /* SC_CHANNELS_HPP_ */
