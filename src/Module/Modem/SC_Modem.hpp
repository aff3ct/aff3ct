#ifndef SC_MODEM_HPP_
#define SC_MODEM_HPP_

#ifdef SYSTEMC_MODULE
#include <vector>
#include <string>
#include <stdexcept>
#include <systemc>
#include <tlm>
#include <tlm_utils/simple_target_socket.h>
#include <tlm_utils/simple_initiator_socket.h>

#include "Tools/Perf/MIPP/mipp.h"

namespace aff3ct
{
namespace module
{
template <typename B, typename R, typename Q>
class SC_Modem;

template <typename B = int, typename R = float, typename Q = R>
class SC_Modem_module_modulator : public sc_core::sc_module
{
public:
	tlm_utils::simple_target_socket   <SC_Modem_module_modulator> s_in;
	tlm_utils::simple_initiator_socket<SC_Modem_module_modulator> s_out;

private:
	SC_Modem<B,R,Q> &modulator;
	mipp::vector<R> X_N2;

public:
	SC_Modem_module_modulator(SC_Modem<B,R,Q> &modulator,
	                          const sc_core::sc_module_name name = "SC_Modem_module_modulator")
	: sc_module(name), s_in("s_in"), s_out("s_out"),
	  modulator(modulator),
	  X_N2(modulator.get_N_mod() * modulator.get_n_frames())
	{
		s_in.register_b_transport(this, &SC_Modem_module_modulator::b_transport);
	}

	const mipp::vector<R>& get_X_N()
	{
		return X_N2;
	}

private:
	void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		if (modulator.get_N() * modulator.get_n_frames() != (int)(trans.get_data_length() / sizeof(B)))
			throw std::length_error("aff3ct::module::Modem: TLM input data size is invalid.");

		const auto X_N1 = (B*)trans.get_data_ptr();

		modulator.modulate(X_N1, X_N2.data());

		tlm::tlm_generic_payload payload;
		payload.set_data_ptr((unsigned char*)X_N2.data());
		payload.set_data_length(X_N2.size() * sizeof(R));

		sc_core::sc_time zero_time(sc_core::SC_ZERO_TIME);
		s_out->b_transport(payload, zero_time);
	}
};

template <typename B, typename R, typename Q>
class SC_Modem_module_filterer : public sc_core::sc_module
{
	SC_HAS_PROCESS(SC_Modem_module_filterer);

public:
	tlm_utils::simple_target_socket   <SC_Modem_module_filterer> s_in;
	tlm_utils::simple_initiator_socket<SC_Modem_module_filterer> s_out;

private:
	SC_Modem<B,R,Q> &modulator;
	mipp::vector<R> Y_N2;

public:
	SC_Modem_module_filterer(SC_Modem<B,R,Q> &modulator,
	                         const sc_core::sc_module_name name = "SC_Modem_module_filterer")
	: sc_module(name), s_in("s_in"), s_out("s_out"),
	  modulator(modulator),
	  Y_N2(modulator.get_N_fil() * modulator.get_n_frames())
	{
		s_in.register_b_transport(this, &SC_Modem_module_filterer::b_transport);
	}

	const mipp::vector<R>& get_Y_N()
	{
		return Y_N2;
	}

private:
	void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		if (modulator.get_N_mod() * modulator.get_n_frames() != (int)(trans.get_data_length() / sizeof(R)))
			throw std::length_error("aff3ct::module::Modem: TLM input data size is invalid.");

		const auto Y_N1 = (R*)trans.get_data_ptr();

		modulator.filter(Y_N1, Y_N2.data());

		tlm::tlm_generic_payload payload;
		payload.set_data_ptr((unsigned char*)Y_N2.data());
		payload.set_data_length(Y_N2.size() * sizeof(R));

		sc_core::sc_time zero_time(sc_core::SC_ZERO_TIME);
		s_out->b_transport(payload, zero_time);
	}
};

template <typename B, typename R, typename Q>
class SC_Modem_module_demodulator : public sc_core::sc_module
{
	SC_HAS_PROCESS(SC_Modem_module_demodulator);

public:
	tlm_utils::simple_target_socket   <SC_Modem_module_demodulator> s_in;
	tlm_utils::simple_initiator_socket<SC_Modem_module_demodulator> s_out;

private:
	SC_Modem<B,R,Q> &modulator;
	mipp::vector<Q> Y_N2;

public:
	SC_Modem_module_demodulator(SC_Modem<B,R,Q> &modulator,
	                            const sc_core::sc_module_name name = "SC_Modem_module_demodulator")
	: sc_module(name), s_in("s_in"), s_out("s_out"),
	  modulator(modulator),
	  Y_N2(modulator.get_N() * modulator.get_n_frames())
	{
		s_in.register_b_transport(this, &SC_Modem_module_demodulator::b_transport);
	}

	const mipp::vector<Q>& get_Y_N()
	{
		return Y_N2;
	}

private:
	void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		if (modulator.get_N_fil() * modulator.get_n_frames() != (int)(trans.get_data_length() / sizeof(Q)))
			throw std::length_error("aff3ct::module::Modem: TLM input data size is invalid.");

		const auto Y_N1 = (Q*)trans.get_data_ptr();

		modulator.demodulate(Y_N1, Y_N2.data());

		tlm::tlm_generic_payload payload;
		payload.set_data_ptr((unsigned char*)Y_N2.data());
		payload.set_data_length(Y_N2.size() * sizeof(Q));

		sc_core::sc_time zero_time(sc_core::SC_ZERO_TIME);
		s_out->b_transport(payload, zero_time);
	}
};

template <typename B, typename R, typename Q>
class SC_Modem_module_demodulator_wg : public sc_core::sc_module
{
	SC_HAS_PROCESS(SC_Modem_module_demodulator_wg);

public:
	tlm_utils::simple_target_socket   <SC_Modem_module_demodulator_wg> s_in1;
	tlm_utils::simple_target_socket   <SC_Modem_module_demodulator_wg> s_in2;
	tlm_utils::simple_initiator_socket<SC_Modem_module_demodulator_wg> s_out;

private:
	SC_Modem<B,R,Q> &modulator;
	R *H_N;
	mipp::vector<Q> Y_N2;

public:
	SC_Modem_module_demodulator_wg(SC_Modem<B,R,Q> &modulator,
	                               const sc_core::sc_module_name name = "SC_Modem_module_demodulator_wg")
	: sc_module(name), s_in1("s_in1"), s_in2("s_in2"), s_out("s_out"),
	  modulator(modulator),
	  H_N(nullptr),
	  Y_N2(modulator.get_N() * modulator.get_n_frames())
	{
		s_in1.register_b_transport(this, &SC_Modem_module_demodulator_wg::b_transport1);
		s_in2.register_b_transport(this, &SC_Modem_module_demodulator_wg::b_transport2);
	}

	const mipp::vector<Q>& get_Y_N()
	{
		return Y_N2;
	}

private:
	void b_transport1(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		if (modulator.get_N_fil() * modulator.get_n_frames() != (int)(trans.get_data_length() / sizeof(R)))
			throw std::length_error("aff3ct::module::Modem: TLM input data size is invalid.");

		H_N = (R*)trans.get_data_ptr();
	}

	void b_transport2(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		if (H_N == nullptr)
			throw std::runtime_error("aff3ct::module::Modem: TLM \"H_N\" pointer can't be NULL.");

		if (modulator.get_N_fil() * modulator.get_n_frames() != (int)(trans.get_data_length() / sizeof(Q)))
			throw std::length_error("aff3ct::module::Modem: TLM input data size is invalid.");

		const auto Y_N1 = (Q*)trans.get_data_ptr();

		modulator.demodulate_with_gains(Y_N1, H_N, Y_N2.data());

		tlm::tlm_generic_payload payload;
		payload.set_data_ptr((unsigned char*)Y_N2.data());
		payload.set_data_length(Y_N2.size() * sizeof(Q));

		sc_core::sc_time zero_time(sc_core::SC_ZERO_TIME);
		s_out->b_transport(payload, zero_time);
	}
};

template <typename B, typename R, typename Q>
class SC_Modem_module_tdemodulator : public sc_core::sc_module
{
	SC_HAS_PROCESS(SC_Modem_module_tdemodulator);

public:
	tlm_utils::simple_target_socket   <SC_Modem_module_tdemodulator> s_in1;
	tlm_utils::simple_target_socket   <SC_Modem_module_tdemodulator> s_in2;
	tlm_utils::simple_initiator_socket<SC_Modem_module_tdemodulator> s_out;

private:
	SC_Modem<B,R,Q> &modulator;
	Q *Y_N1;
	mipp::vector<Q> Y_N3;

public:
	SC_Modem_module_tdemodulator(SC_Modem<B,R,Q> &modulator,
	                             const sc_core::sc_module_name name = "SC_Modem_module_tdemodulator")
	: sc_module(name), s_in1("s_in1"), s_in2("s_in2"), s_out("s_out"),
	  modulator(modulator),
	  Y_N1(nullptr),
	  Y_N3(modulator.get_N() * modulator.get_n_frames())
	{
		s_in1.register_b_transport(this, &SC_Modem_module_tdemodulator::b_transport1);
		s_in2.register_b_transport(this, &SC_Modem_module_tdemodulator::b_transport2);
	}

	const mipp::vector<Q>& get_Y_N()
	{
		return Y_N3;
	}

private:
	void b_transport1(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		if (modulator.get_N_fil() * modulator.get_n_frames() != (int)(trans.get_data_length() / sizeof(Q)))
			throw std::length_error("aff3ct::module::Modem: TLM input data size is invalid.");

		Y_N1 = (Q*)trans.get_data_ptr();

		modulator.demodulate(Y_N1, Y_N3.data());

		tlm::tlm_generic_payload payload;
		payload.set_data_ptr((unsigned char*)Y_N3.data());
		payload.set_data_length(Y_N3.size() * sizeof(Q));

		sc_core::sc_time zero_time(sc_core::SC_ZERO_TIME);
		s_out->b_transport(payload, zero_time);
	}

	void b_transport2(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		if (Y_N1 == nullptr)
			throw std::runtime_error("aff3ct::module::Modem: TLM \"Y_N1\" pointer can't be NULL.");

		if (modulator.get_N() * modulator.get_n_frames() != (int)(trans.get_data_length() / sizeof(Q)))
			throw std::length_error("aff3ct::module::Modem: TLM input data size is invalid.");

		const auto Y_N2 = (Q*)trans.get_data_ptr();

		modulator.demodulate(Y_N1, Y_N2, Y_N3.data());

		tlm::tlm_generic_payload payload;
		payload.set_data_ptr((unsigned char*)Y_N3.data());
		payload.set_data_length(Y_N3.size() * sizeof(Q));

		sc_core::sc_time zero_time(sc_core::SC_ZERO_TIME);
		s_out->b_transport(payload, zero_time);
	}
};

template <typename B, typename R, typename Q>
class SC_Modem_module_tdemodulator_wg : public sc_core::sc_module
{
	SC_HAS_PROCESS(SC_Modem_module_tdemodulator_wg);

public:
	tlm_utils::simple_target_socket   <SC_Modem_module_tdemodulator_wg> s_in1;
	tlm_utils::simple_target_socket   <SC_Modem_module_tdemodulator_wg> s_in2;
	tlm_utils::simple_target_socket   <SC_Modem_module_tdemodulator_wg> s_in3;
	tlm_utils::simple_initiator_socket<SC_Modem_module_tdemodulator_wg> s_out;

private:
	SC_Modem<B,R,Q> &modulator;
	R *H_N;
	Q *Y_N1;
	mipp::vector<Q> Y_N3;

public:
	SC_Modem_module_tdemodulator_wg(SC_Modem<B,R,Q> &modulator,
	                                const sc_core::sc_module_name name = "SC_Modem_module_tdemodulator_wg")
	: sc_module(name), s_in1("s_in1"), s_in2("s_in2"), s_in3("s_in3"), s_out("s_out"),
	  modulator(modulator),
	  H_N (nullptr),
	  Y_N1(nullptr),
	  Y_N3(modulator.get_N() * modulator.get_n_frames())
	{
		s_in1.register_b_transport(this, &SC_Modem_module_tdemodulator_wg::b_transport1);
		s_in2.register_b_transport(this, &SC_Modem_module_tdemodulator_wg::b_transport2);
		s_in3.register_b_transport(this, &SC_Modem_module_tdemodulator_wg::b_transport3);
	}

	const mipp::vector<Q>& get_Y_N()
	{
		return Y_N3;
	}

private:
	void b_transport1(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		if (modulator.get_N_fil() * modulator.get_n_frames() != (int)(trans.get_data_length() / sizeof(R)))
			throw std::length_error("aff3ct::module::Modem: TLM input data size is invalid.");

		H_N = (R*)trans.get_data_ptr();
	}

	void b_transport2(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		if (H_N == nullptr)
			throw std::runtime_error("aff3ct::module::Modem: TLM \"H_N\" pointer can't be NULL.");

		if (modulator.get_N_fil() * modulator.get_n_frames() != (int)(trans.get_data_length() / sizeof(Q)))
			throw std::length_error("aff3ct::module::Modem: TLM input data size is invalid.");

		Y_N1 = (Q*)trans.get_data_ptr();

		modulator.demodulate_with_gains(Y_N1, H_N, Y_N3.data());

		tlm::tlm_generic_payload payload;
		payload.set_data_ptr((unsigned char*)Y_N3.data());
		payload.set_data_length(Y_N3.size() * sizeof(Q));

		sc_core::sc_time zero_time(sc_core::SC_ZERO_TIME);
		s_out->b_transport(payload, zero_time);
	}

	void b_transport3(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		if (H_N == nullptr)
			throw std::runtime_error("aff3ct::module::Modem: TLM \"H_N\" pointer can't be NULL.");

		if (Y_N1 == nullptr)
			throw std::runtime_error("aff3ct::module::Modem: TLM \"Y_N1\" pointer can't be NULL.");

		if (modulator.get_N() * modulator.get_n_frames() != (int)(trans.get_data_length() / sizeof(Q)))
			throw std::length_error("aff3ct::module::Modem: TLM input data size is invalid.");

		const auto Y_N2 = (Q*)trans.get_data_ptr();

		modulator.demodulate_with_gains(Y_N1, H_N, Y_N2, Y_N3.data());

		tlm::tlm_generic_payload payload;
		payload.set_data_ptr((unsigned char*)Y_N3.data());
		payload.set_data_length(Y_N3.size() * sizeof(Q));

		sc_core::sc_time zero_time(sc_core::SC_ZERO_TIME);
		s_out->b_transport(payload, zero_time);
	}
};

template <typename B, typename R, typename Q>
class SC_Modem : public Modem_i<B,R,Q>
{
public:
	SC_Modem_module_modulator      <B,R,Q> *sc_module_mod;
	SC_Modem_module_filterer       <B,R,Q> *sc_module_filt;
	SC_Modem_module_demodulator    <B,R,Q> *sc_module_demod;
	SC_Modem_module_demodulator_wg <B,R,Q> *sc_module_demod_wg;
	SC_Modem_module_tdemodulator   <B,R,Q> *sc_module_tdemod;
	SC_Modem_module_tdemodulator_wg<B,R,Q> *sc_module_tdemod_wg;

public:
	SC_Modem(const int N, const int N_mod, const int N_fil, const R sigma, const int n_frames = 1,
	         const std::string name = "SC_Modem")
	: Modem_i<B,R,Q>(N, N_mod, N_fil, sigma, n_frames, name),
	  sc_module_mod(nullptr), sc_module_filt(nullptr), sc_module_demod(nullptr), sc_module_demod_wg(nullptr),
	  sc_module_tdemod(nullptr), sc_module_tdemod_wg(nullptr) {}

	SC_Modem(const int N, const int N_mod, const R sigma, const int n_frames = 1,
	         const std::string name = "SC_Modem")
	: Modem_i<B,R,Q>(N, N_mod, sigma, n_frames, name),
	  sc_module_mod(nullptr), sc_module_filt(nullptr), sc_module_demod(nullptr), sc_module_demod_wg(nullptr),
	  sc_module_tdemod(nullptr), sc_module_tdemod_wg(nullptr) {}

	SC_Modem(const int N, const R sigma, const int n_frames = 1, const std::string name = "SC_Modem")
	: Modem_i<B,R,Q>(N, sigma, n_frames, name),
	  sc_module_mod(nullptr), sc_module_filt(nullptr), sc_module_demod(nullptr), sc_module_demod_wg(nullptr),
	  sc_module_tdemod(nullptr), sc_module_tdemod_wg(nullptr) {}

	virtual ~SC_Modem()
	{ 
		if (sc_module_mod       != nullptr) { delete sc_module_mod;       sc_module_mod       = nullptr; }
		if (sc_module_filt      != nullptr) { delete sc_module_filt;      sc_module_filt      = nullptr; }
		if (sc_module_demod     != nullptr) { delete sc_module_demod;     sc_module_demod     = nullptr; }
		if (sc_module_demod_wg  != nullptr) { delete sc_module_demod_wg;  sc_module_demod_wg  = nullptr; }
		if (sc_module_tdemod    != nullptr) { delete sc_module_tdemod;    sc_module_tdemod    = nullptr; }
		if (sc_module_tdemod_wg != nullptr) { delete sc_module_tdemod_wg; sc_module_tdemod_wg = nullptr; }
	}

	void create_sc_module_modulator()
	{
		if (sc_module_mod != nullptr) { delete sc_module_mod; sc_module_mod = nullptr; }
		const std::string new_name = this->name + "_mod";
		this->sc_module_mod = new SC_Modem_module_modulator<B,R,Q>(*this, new_name.c_str());
	}

	void create_sc_module_filterer()
	{
		if (sc_module_filt != nullptr) { delete sc_module_filt; sc_module_filt = nullptr; }
		const std::string new_name = this->name + "_filt";
		this->sc_module_filt = new SC_Modem_module_filterer<B,R,Q>(*this, new_name.c_str());
	}

	void create_sc_module_demodulator()
	{
		if (sc_module_demod != nullptr) { delete sc_module_demod; sc_module_demod = nullptr; }
		const std::string new_name = this->name + "_demod";
		this->sc_module_demod = new SC_Modem_module_demodulator<B,R,Q>(*this, new_name.c_str());
	}

	void create_sc_module_demodulator_wg()
	{
		if (sc_module_demod_wg != nullptr) { delete sc_module_demod_wg; sc_module_demod_wg = nullptr; }
		const std::string new_name = this->name + "_demod_wg";
		this->sc_module_demod_wg = new SC_Modem_module_demodulator_wg<B,R,Q>(*this, new_name.c_str());
	}

	void create_sc_module_tdemodulator()
	{
		if (sc_module_tdemod != nullptr) { delete sc_module_tdemod; sc_module_tdemod = nullptr; }
		const std::string new_name = this->name + "_tdemod";
		this->sc_module_tdemod = new SC_Modem_module_tdemodulator<B,R,Q>(*this, new_name.c_str());
	}

	void create_sc_module_tdemodulator_wg()
	{
		if (sc_module_tdemod_wg != nullptr) { delete sc_module_tdemod_wg; sc_module_tdemod_wg = nullptr; }
		const std::string new_name = this->name + "_tdemod_wg";
		this->sc_module_tdemod_wg = new SC_Modem_module_tdemodulator_wg<B,R,Q>(*this, new_name.c_str());
	}
};

template <typename B = int, typename R = float, typename Q = R>
using Modem = SC_Modem<B,R,Q>;
}
}
#else
#include "SPU_Modem.hpp"
#endif

#endif /* SC_MODEM_HPP_ */
