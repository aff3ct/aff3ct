#ifndef SC_MODULATOR_HPP_
#define SC_MODULATOR_HPP_

#ifdef SYSTEMC
#include <vector>
#include <string>
#include <cassert>
#include <systemc>
#include <tlm>
#include <tlm_utils/simple_target_socket.h>
#include <tlm_utils/simple_initiator_socket.h>

#include "../Tools/Frame_trace/Frame_trace.hpp"
#include "../Tools/MIPP/mipp.h"

template <typename B, typename R>
class SC_Modulator : public sc_core::sc_module, public Modulator_interface<B,R>
{
	SC_HAS_PROCESS(SC_Modulator);

public:
	tlm_utils::simple_target_socket   <SC_Modulator> socket_in_modulate;
	tlm_utils::simple_target_socket   <SC_Modulator> socket_in_demodulate;
	tlm_utils::simple_initiator_socket<SC_Modulator> socket_out;

private:
	mipp::vector<B> X_N1;
	mipp::vector<R> X_N2;
	mipp::vector<R> Y_N1, Y_N2;

public:
	SC_Modulator(const int N, const int N_mod, const int n_frames = 1, const sc_core::sc_module_name name = "SC_Modulator")
	: sc_module(name),
	  Modulator_interface<B,R>(N, N_mod, n_frames),
	  socket_in_modulate  ("socket_in_modulate_SC_Modulator"),
	  socket_in_demodulate("socket_in_demodulate_SC_Modulator"),
	  socket_out          ("socket_out_SC_Modulator"),
	  X_N1(N     * n_frames),
	  X_N2(N_mod * n_frames),
	  Y_N1(N_mod * n_frames),
	  Y_N2(N     * n_frames)
	{
		socket_in_modulate  .register_b_transport(this, &SC_Modulator::b_transport_modulate);
		socket_in_demodulate.register_b_transport(this, &SC_Modulator::b_transport_demodulate);
	};

	virtual ~SC_Modulator() {};

	virtual void   modulate(const mipp::vector<B>& X_N1, mipp::vector<R>& X_N2) const = 0;
	virtual void demodulate(const mipp::vector<R>& Y_N1, mipp::vector<R>& Y_N2) const = 0;

	virtual void set_n_frames(const int n_frames)
	{
		assert(n_frames > 0);
		this->n_frames = n_frames;

		if ((int)X_N1.size() != this->N     * this->n_frames) this->X_N1.resize(this->N     * this->n_frames);
		if ((int)X_N2.size() != this->N_mod * this->n_frames) this->X_N2.resize(this->N_mod * this->n_frames);
		if ((int)Y_N1.size() != this->N_mod * this->n_frames) this->Y_N1.resize(this->N_mod * this->n_frames);
		if ((int)Y_N2.size() != this->N     * this->n_frames) this->Y_N2.resize(this->N     * this->n_frames);
	}

private:
	void b_transport_modulate(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		assert((trans.get_data_length() / sizeof(B)) == X_N1.size());

		const B* buffer_in = (B*)trans.get_data_ptr();
		std::copy(buffer_in, buffer_in + X_N1.size(), X_N1.begin());

		this->modulate(X_N1, X_N2);

		tlm::tlm_generic_payload payload;
		payload.set_data_ptr((unsigned char*)X_N2.data());
		payload.set_data_length(X_N2.size() * sizeof(R));

		sc_core::sc_time zero_time(sc_core::SC_ZERO_TIME);
		socket_out->b_transport(payload, zero_time);
	}

	void b_transport_demodulate(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		assert((trans.get_data_length() / sizeof(R)) == Y_N1.size());

		const R* buffer_in = (R*)trans.get_data_ptr();
		std::copy(buffer_in, buffer_in + Y_N1.size(), Y_N1.begin());

		this->demodulate(Y_N1, Y_N2);

		tlm::tlm_generic_payload payload;
		payload.set_data_ptr((unsigned char*)Y_N2.data());
		payload.set_data_length(Y_N2.size() * sizeof(R));

		sc_core::sc_time zero_time(sc_core::SC_ZERO_TIME);
		socket_out->b_transport(payload, zero_time);
	}
};

template <typename B, typename R>
using Modulator = SC_Modulator<B,R>;
#else
template <typename B, typename R>
using Modulator = Modulator_interface<B,R>;
#endif

#endif /* SC_MODULATOR_HPP_ */