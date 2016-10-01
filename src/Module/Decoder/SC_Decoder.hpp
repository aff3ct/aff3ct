#ifndef SC_DECODER_HPP_
#define SC_DECODER_HPP_

#ifdef SYSTEMC
#include <vector>
#include <string>
#include <cassert>
#include <systemc>
#include <tlm>
#include <tlm_utils/simple_target_socket.h>
#include <tlm_utils/simple_initiator_socket.h>

#include "Tools/Perf/MIPP/mipp.h"

template <typename B, typename R>
class SC_Decoder;

template <typename B, typename R>
class SC_Decoder_module : public sc_core::sc_module
{
	SC_HAS_PROCESS(SC_Decoder_module);

public:
	tlm_utils::simple_target_socket   <SC_Decoder_module> s_in;
	tlm_utils::simple_initiator_socket<SC_Decoder_module> s_out;

private:
	SC_Decoder<B,R> &decoder;
	mipp::vector<R> Y_N;
	mipp::vector<B> V_K;

public:
	SC_Decoder_module(SC_Decoder<B,R> &decoder, const sc_core::sc_module_name name = "SC_Decoder_module")
	: sc_module(name), s_in ("s_in"), s_out("s_out"),
	  decoder(decoder),
	  Y_N(decoder.N * decoder.n_frames),
	  V_K(decoder.K * decoder.n_frames)
	{
		s_in.register_b_transport(this, &SC_Decoder_module::b_transport);
	}

	void resize_buffers()
	{
		if ((int)Y_N.size() != decoder.N * decoder.n_frames) Y_N.resize(decoder.N * decoder.n_frames);
		if ((int)V_K.size() != decoder.K * decoder.n_frames) V_K.resize(decoder.K * decoder.n_frames);
	}

private:
	void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		assert((trans.get_data_length() / sizeof(R)) == Y_N.size());

		const R* buffer_in = (R*)trans.get_data_ptr();
		std::copy(buffer_in, buffer_in + Y_N.size(), Y_N.begin());

		decoder.load  (Y_N);
		decoder.decode(   );
		decoder.store (V_K);

		tlm::tlm_generic_payload payload;
		payload.set_data_ptr((unsigned char*)V_K.data());
		payload.set_data_length(V_K.size() * sizeof(B));

		sc_core::sc_time zero_time(sc_core::SC_ZERO_TIME);
		s_out->b_transport(payload, zero_time);
	}
};

template <typename B, typename R>
class SC_Decoder : public Decoder_interface<B,R>
{
	friend SC_Decoder_module<B,R>;

public:
	SC_Decoder_module<B,R> *module;

public:
	SC_Decoder(const int K, const int N, const int n_frames, const std::string name = "SC_Decoder")
	: Decoder_interface<B,R>(K, N, n_frames, name), module(nullptr) {}

	virtual ~SC_Decoder() { if (module != nullptr) { delete module; module = nullptr; } }

	void create_sc_module()
	{
		this->module = new SC_Decoder_module<B,R>(*this, this->name.c_str());
	}
};

template <typename B, typename R>
using Decoder = SC_Decoder<B,R>;
#else
template <typename B, typename R>
using Decoder = Decoder_interface<B,R>;
#endif

#endif /* DECODER_HPP_ */
