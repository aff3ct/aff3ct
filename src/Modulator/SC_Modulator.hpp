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

#include "../Tools/MIPP/mipp.h"

template <typename B, typename R>
class SC_Modulator;

template <typename B, typename R>
class SC_Modulator_module : public sc_core::sc_module
{
	SC_HAS_PROCESS(SC_Modulator_module);

public:
	tlm_utils::simple_target_socket   <SC_Modulator_module> s_in_mod;
	tlm_utils::simple_target_socket   <SC_Modulator_module> s_in_demod;
	tlm_utils::simple_initiator_socket<SC_Modulator_module> s_out_mod;
	tlm_utils::simple_initiator_socket<SC_Modulator_module> s_out_demod;

private:
	SC_Modulator<B,R> &modulator;
	mipp::vector<B> X_N1;
	mipp::vector<R> X_N2;
	mipp::vector<R> Y_N1, Y_N2;

public:
	SC_Modulator_module(SC_Modulator<B,R> &modulator, const sc_core::sc_module_name name = "SC_Modulator_module")
	: sc_module(name), 
	  s_in_mod ("s_in_mod"),  s_in_demod ("s_in_demod"), 
	  s_out_mod("s_out_mod"), s_out_demod("s_out_demod"),
	  modulator(modulator),
	  X_N1(modulator.N     * modulator.n_frames),
	  X_N2(modulator.N_mod * modulator.n_frames),
	  Y_N1(modulator.N_mod * modulator.n_frames),
	  Y_N2(modulator.N     * modulator.n_frames)
	{
		s_in_mod  .register_b_transport(this, &SC_Modulator_module::b_transport_modulate);
		s_in_demod.register_b_transport(this, &SC_Modulator_module::b_transport_demodulate);
	}

	void resize_buffers()
	{
		if ((int)X_N1.size() != modulator.N     * modulator.n_frames) X_N1.resize(modulator.N     * modulator.n_frames);
		if ((int)X_N2.size() != modulator.N_mod * modulator.n_frames) X_N2.resize(modulator.N_mod * modulator.n_frames);
		if ((int)Y_N1.size() != modulator.N_mod * modulator.n_frames) Y_N1.resize(modulator.N_mod * modulator.n_frames);
		if ((int)Y_N2.size() != modulator.N     * modulator.n_frames) Y_N2.resize(modulator.N     * modulator.n_frames);
	}

private:
	void b_transport_modulate(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		assert((trans.get_data_length() / sizeof(B)) == X_N1.size());

		const B* buffer_in = (B*)trans.get_data_ptr();
		std::copy(buffer_in, buffer_in + X_N1.size(), X_N1.begin());

		modulator.modulate(X_N1, X_N2);

		tlm::tlm_generic_payload payload;
		payload.set_data_ptr((unsigned char*)X_N2.data());
		payload.set_data_length(X_N2.size() * sizeof(R));

		sc_core::sc_time zero_time(sc_core::SC_ZERO_TIME);
		s_out_mod->b_transport(payload, zero_time);
	}

	void b_transport_demodulate(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		assert((trans.get_data_length() / sizeof(R)) == Y_N1.size());

		const R* buffer_in = (R*)trans.get_data_ptr();
		std::copy(buffer_in, buffer_in + Y_N1.size(), Y_N1.begin());

		modulator.demodulate(Y_N1, Y_N2);

		tlm::tlm_generic_payload payload;
		payload.set_data_ptr((unsigned char*)Y_N2.data());
		payload.set_data_length(Y_N2.size() * sizeof(R));

		sc_core::sc_time zero_time(sc_core::SC_ZERO_TIME);
		s_out_demod->b_transport(payload, zero_time);
	}
};

template <typename B, typename R>
class SC_Modulator : public Modulator_interface<B,R>
{
	friend SC_Modulator_module<B,R>;

private:
	std::string name;

public:
	SC_Modulator_module<B,R> *module;

public:
	SC_Modulator(const int N, const int N_mod, const int n_frames = 1, const std::string name = "SC_Modulator")
	: Modulator_interface<B,R>(N, N_mod, n_frames, name), name(name), module(nullptr) {}

	virtual ~SC_Modulator() { if (module != nullptr) { delete module; module = nullptr; } }

	virtual void   modulate(const mipp::vector<B>& X_N1, mipp::vector<R>& X_N2) const = 0;
	virtual void demodulate(const mipp::vector<R>& Y_N1, mipp::vector<R>& Y_N2) const = 0;

	virtual void set_n_frames(const int n_frames)
	{
		Modulator_interface<B,R>::set_n_frames(n_frames);

		if (module != nullptr)
			module->resize_buffers();
	}

	void create_sc_module()
	{
		this->module = new SC_Modulator_module<B,R>(*this, name.c_str());
	}
};

template <typename B, typename R>
using Modulator = SC_Modulator<B,R>;
#else
template <typename B, typename R>
using Modulator = Modulator_interface<B,R>;
#endif

#endif /* SC_MODULATOR_HPP_ */