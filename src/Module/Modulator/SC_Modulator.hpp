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

#include "Tools/MIPP/mipp.h"

template <typename B, typename R, typename Q>
class SC_Modulator;

template <typename B, typename R, typename Q>
class SC_Modulator_module_modulator : public sc_core::sc_module
{
	SC_HAS_PROCESS(SC_Modulator_module_modulator);

public:
	tlm_utils::simple_target_socket   <SC_Modulator_module_modulator> s_in;
	tlm_utils::simple_initiator_socket<SC_Modulator_module_modulator> s_out;

private:
	SC_Modulator<B,R,Q> &modulator;
	mipp::vector<B> X_N1;
	mipp::vector<R> X_N2;

public:
	SC_Modulator_module_modulator(SC_Modulator<B,R,Q> &modulator, 
	                               const sc_core::sc_module_name name = "SC_Modulator_module_modulator")
	: sc_module(name), s_in("s_in"), s_out("s_out"),
	  modulator(modulator),
	  X_N1(modulator.N     * modulator.n_frames),
	  X_N2(modulator.N_mod * modulator.n_frames)
	{
		s_in.register_b_transport(this, &SC_Modulator_module_modulator::b_transport);
	}

	void resize_buffers()
	{
		if ((int)X_N1.size() != modulator.N     * modulator.n_frames) X_N1.resize(modulator.N     * modulator.n_frames);
		if ((int)X_N2.size() != modulator.N_mod * modulator.n_frames) X_N2.resize(modulator.N_mod * modulator.n_frames);
	}

private:
	void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		assert((trans.get_data_length() / sizeof(B)) == X_N1.size());

		const B* buffer_in = (B*)trans.get_data_ptr();
		std::copy(buffer_in, buffer_in + X_N1.size(), X_N1.begin());

		modulator.modulate(X_N1, X_N2);

		tlm::tlm_generic_payload payload;
		payload.set_data_ptr((unsigned char*)X_N2.data());
		payload.set_data_length(X_N2.size() * sizeof(R));

		sc_core::sc_time zero_time(sc_core::SC_ZERO_TIME);
		s_out->b_transport(payload, zero_time);
	}
};

template <typename B, typename R, typename Q>
class SC_Modulator_module_filterer : public sc_core::sc_module
{
	SC_HAS_PROCESS(SC_Modulator_module_filterer);

public:
	tlm_utils::simple_target_socket   <SC_Modulator_module_filterer> s_in;
	tlm_utils::simple_initiator_socket<SC_Modulator_module_filterer> s_out;

private:
	SC_Modulator<B,R,Q> &modulator;
	mipp::vector<R> Y_N1, Y_N2;

public:
	SC_Modulator_module_filterer(SC_Modulator<B,R,Q> &modulator, 
	                              const sc_core::sc_module_name name = "SC_Modulator_module_filterer")
	: sc_module(name), s_in("s_in"), s_out("s_out"),
	  modulator(modulator),
	  Y_N1(modulator.N_mod * modulator.n_frames),
	  Y_N2(modulator.N_fil * modulator.n_frames)
	{
		s_in.register_b_transport(this, &SC_Modulator_module_filterer::b_transport);
	}

	void resize_buffers()
	{
		if ((int)Y_N1.size() != modulator.N_mod * modulator.n_frames) Y_N1.resize(modulator.N_mod * modulator.n_frames);
		if ((int)Y_N2.size() != modulator.N_fil * modulator.n_frames) Y_N2.resize(modulator.N_fil * modulator.n_frames);
	}

private:
	void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		assert((trans.get_data_length() / sizeof(R)) == Y_N1.size());

		const R* buffer_in = (R*)trans.get_data_ptr();
		std::copy(buffer_in, buffer_in + Y_N1.size(), Y_N1.begin());

		modulator.filter(Y_N1, Y_N2);

		tlm::tlm_generic_payload payload;
		payload.set_data_ptr((unsigned char*)Y_N2.data());
		payload.set_data_length(Y_N2.size() * sizeof(R));

		sc_core::sc_time zero_time(sc_core::SC_ZERO_TIME);
		s_out->b_transport(payload, zero_time);
	}
};

template <typename B, typename R, typename Q>
class SC_Modulator_module_demodulator : public sc_core::sc_module
{
	SC_HAS_PROCESS(SC_Modulator_module_demodulator);

public:
	tlm_utils::simple_target_socket   <SC_Modulator_module_demodulator> s_in;
	tlm_utils::simple_initiator_socket<SC_Modulator_module_demodulator> s_out;

private:
	SC_Modulator<B,R,Q> &modulator;
	mipp::vector<Q> Y_N1, Y_N2;

public:
	SC_Modulator_module_demodulator(SC_Modulator<B,R,Q> &modulator, 
	                                 const sc_core::sc_module_name name = "SC_Modulator_module_demodulator")
	: sc_module(name), s_in("s_in"), s_out("s_out"),
	  modulator(modulator),
	  Y_N1(modulator.N_fil * modulator.n_frames),
	  Y_N2(modulator.N     * modulator.n_frames)
	{
		s_in.register_b_transport(this, &SC_Modulator_module_demodulator::b_transport);
	}

	void resize_buffers()
	{
		if ((int)Y_N1.size() != modulator.N_fil * modulator.n_frames) Y_N1.resize(modulator.N_fil * modulator.n_frames);
		if ((int)Y_N2.size() != modulator.N     * modulator.n_frames) Y_N2.resize(modulator.N     * modulator.n_frames);
	}

private:
	void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		assert((trans.get_data_length() / sizeof(Q)) == Y_N1.size());

		const Q* buffer_in = (Q*)trans.get_data_ptr();
		std::copy(buffer_in, buffer_in + Y_N1.size(), Y_N1.begin());

		modulator.demodulate(Y_N1, Y_N2);

		tlm::tlm_generic_payload payload;
		payload.set_data_ptr((unsigned char*)Y_N2.data());
		payload.set_data_length(Y_N2.size() * sizeof(Q));

		sc_core::sc_time zero_time(sc_core::SC_ZERO_TIME);
		s_out->b_transport(payload, zero_time);
	}
};

template <typename B, typename R, typename Q>
class SC_Modulator_module_tdemodulator : public sc_core::sc_module
{
	SC_HAS_PROCESS(SC_Modulator_module_tdemodulator);

public:
	tlm_utils::simple_target_socket   <SC_Modulator_module_tdemodulator> s_in1;
	tlm_utils::simple_target_socket   <SC_Modulator_module_tdemodulator> s_in2;
	tlm_utils::simple_initiator_socket<SC_Modulator_module_tdemodulator> s_out;

private:
	SC_Modulator<B,R,Q> &modulator;
	mipp::vector<Q> Y_N1, Y_N2, Y_N3;

public:
	SC_Modulator_module_tdemodulator(SC_Modulator<B,R,Q> &modulator, 
	                                       const sc_core::sc_module_name name = "SC_Modulator_module_tdemodulator")
	: sc_module(name), s_in1("s_in1"), s_in2("s_in2"), s_out("s_out"),
	  modulator(modulator),
	  Y_N1(modulator.N_fil * modulator.n_frames),
	  Y_N2(modulator.N     * modulator.n_frames),
	  Y_N3(modulator.N     * modulator.n_frames)
	{
		s_in1.register_b_transport(this, &SC_Modulator_module_tdemodulator::b_transport1);
		s_in2.register_b_transport(this, &SC_Modulator_module_tdemodulator::b_transport2);
	}

	void resize_buffers()
	{
		if ((int)Y_N1.size() != modulator.N_fil * modulator.n_frames) Y_N1.resize(modulator.N_fil * modulator.n_frames);
		if ((int)Y_N2.size() != modulator.N     * modulator.n_frames) Y_N2.resize(modulator.N     * modulator.n_frames);
		if ((int)Y_N3.size() != modulator.N     * modulator.n_frames) Y_N3.resize(modulator.N     * modulator.n_frames);
	}

private:
	void b_transport1(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		assert((trans.get_data_length() / sizeof(Q)) == Y_N1.size());

		const Q* buffer_in = (Q*)trans.get_data_ptr();
		std::copy(buffer_in, buffer_in + Y_N1.size(), Y_N1.begin());

		std::fill(Y_N2.begin(), Y_N2.end(), 0);
		modulator.demodulate(Y_N1, Y_N2, Y_N3);
		// modulator.demodulate(Y_N1, Y_N3); // optim to avoid the 0 init

		tlm::tlm_generic_payload payload;
		payload.set_data_ptr((unsigned char*)Y_N3.data());
		payload.set_data_length(Y_N3.size() * sizeof(Q));

		sc_core::sc_time zero_time(sc_core::SC_ZERO_TIME);
		s_out->b_transport(payload, zero_time);
	}

	void b_transport2(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		assert((trans.get_data_length() / sizeof(Q)) == Y_N2.size());

		const Q* buffer_in = (Q*)trans.get_data_ptr();
		std::copy(buffer_in, buffer_in + Y_N2.size(), Y_N2.begin());

		modulator.demodulate(Y_N1, Y_N2, Y_N3);

		tlm::tlm_generic_payload payload;
		payload.set_data_ptr((unsigned char*)Y_N3.data());
		payload.set_data_length(Y_N3.size() * sizeof(Q));

		sc_core::sc_time zero_time(sc_core::SC_ZERO_TIME);
		s_out->b_transport(payload, zero_time);
	}
};

template <typename B, typename R, typename Q>
class SC_Modulator : public Modulator_interface<B,R,Q>
{
	friend SC_Modulator_module_modulator   <B,R,Q>;
	friend SC_Modulator_module_filterer    <B,R,Q>;
	friend SC_Modulator_module_demodulator <B,R,Q>;
	friend SC_Modulator_module_tdemodulator<B,R,Q>;

public:
	SC_Modulator_module_modulator   <B,R,Q> *module_mod;
	SC_Modulator_module_filterer    <B,R,Q> *module_filt;
	SC_Modulator_module_demodulator <B,R,Q> *module_demod;
	SC_Modulator_module_tdemodulator<B,R,Q> *module_tdemod;

public:
	SC_Modulator(const int N, const int N_mod, const int N_fil, const int n_frames = 1, 
	             const std::string name = "SC_Modulator")
	: Modulator_interface<B,R,Q>(N, N_mod, N_fil, n_frames, name), 
	  module_mod(nullptr), module_filt(nullptr), module_demod(nullptr), module_tdemod(nullptr) {}

	SC_Modulator(const int N, const int N_mod, const int n_frames = 1, const std::string name = "SC_Modulator")
	: Modulator_interface<B,R,Q>(N, N_mod, n_frames, name),
	  module_mod(nullptr), module_filt(nullptr), module_demod(nullptr), module_tdemod(nullptr) {}

	SC_Modulator(const int N, const int n_frames = 1, const std::string name = "SC_Modulator")
	: Modulator_interface<B,R,Q>(N, n_frames, name),
	  module_mod(nullptr), module_filt(nullptr), module_demod(nullptr), module_tdemod(nullptr) {}

	virtual ~SC_Modulator() 
	{ 
		if (module_mod    != nullptr) { delete module_mod;    module_mod    = nullptr; }
		if (module_filt   != nullptr) { delete module_filt;   module_filt   = nullptr; }
		if (module_demod  != nullptr) { delete module_demod;  module_demod  = nullptr; }
		if (module_tdemod != nullptr) { delete module_tdemod; module_tdemod = nullptr; }
	}

	virtual void   modulate(const mipp::vector<B>& X_N1,                              mipp::vector<R>& X_N2) = 0;
	virtual void     filter(const mipp::vector<R>& Y_N1,                              mipp::vector<R>& Y_N2)
	{
		Modulator_interface<B,R,Q>::filter(Y_N1, Y_N2);
	}
	virtual void demodulate(const mipp::vector<Q>& Y_N1,                              mipp::vector<Q>& Y_N2) = 0;
	virtual void demodulate(const mipp::vector<Q>& Y_N1, const mipp::vector<Q>& Y_N2, mipp::vector<Q>& Y_N3)
	{
		Modulator_interface<B,R,Q>::demodulate(Y_N1, Y_N2, Y_N3);
	}

	virtual int get_buffer_size_after_modulation(const int N) 
	{
		return Modulator_interface<B,R,Q>::get_buffer_size_after_modulation(N);
	}
	virtual int get_buffer_size_after_filtering (const int N)
	{
		return Modulator_interface<B,R,Q>::get_buffer_size_after_filtering(N);
	}

	virtual void set_n_frames(const int n_frames)
	{
		Modulator_interface<B,R,Q>::set_n_frames(n_frames);

		if (module_mod    != nullptr) module_mod   ->resize_buffers();
		if (module_filt   != nullptr) module_filt  ->resize_buffers();
		if (module_demod  != nullptr) module_demod ->resize_buffers();
		if (module_tdemod != nullptr) module_tdemod->resize_buffers();
	}

	void create_sc_module_modulator()
	{
		const std::string new_name = this->name + "_mod";
		this->module_mod = new SC_Modulator_module_modulator<B,R,Q>(*this, new_name.c_str());
	}

	void create_sc_module_filterer()
	{
		const std::string new_name = this->name + "_filt";
		this->module_filt = new SC_Modulator_module_filterer<B,R,Q>(*this, new_name.c_str());
	}

	void create_sc_module_demodulator()
	{
		const std::string new_name = this->name + "_demod";
		this->module_demod = new SC_Modulator_module_demodulator<B,R,Q>(*this, new_name.c_str());
	}

	void create_sc_module_tdemodulator()
	{
		const std::string new_name = this->name + "_tdemod";
		this->module_tdemod = new SC_Modulator_module_tdemodulator<B,R,Q>(*this, new_name.c_str());
	}
};

template <typename B, typename R, typename Q>
using Modulator = SC_Modulator<B,R,Q>;
#else
template <typename B, typename R, typename Q>
using Modulator = Modulator_interface<B,R,Q>;
#endif

#endif /* SC_MODULATOR_HPP_ */
