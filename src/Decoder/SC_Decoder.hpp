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

#include "../Tools/Frame_trace/Frame_trace.hpp"
#include "../Tools/MIPP/mipp.h"

template <typename B, typename R>
class SC_Decoder : public sc_core::sc_module, public Decoder_interface<B,R>
{
	SC_HAS_PROCESS(SC_Decoder);

public:
	tlm_utils::simple_target_socket   <SC_Decoder> socket_in;
	tlm_utils::simple_initiator_socket<SC_Decoder> socket_out;

private:
	bool sockets_binded;
	mipp::vector<R> Y_N;
	mipp::vector<B> V_K;

public:
	SC_Decoder(const int K, const int N, const int n_frames, const sc_core::sc_module_name name = "SC_Decoder")
	: sc_module(name), 
	  Decoder_interface<B,R>(K, N),
	  socket_in ("socket_in_SC_Decoder"),
	  socket_out("socket_out_SC_Decoder"),
	  sockets_binded(false),
	  Y_N(0),
	  V_K(0)
	{
	}

	virtual ~SC_Decoder() {};

	void register_sockets()
	{
		socket_in.register_b_transport(this, &SC_Decoder::b_transport);
		sockets_binded = true;

		this->resize_buffers();
	}

	bool socket_binded() { return sockets_binded; }

	virtual void load  (const mipp::vector<R>& Y_N)       = 0;
	virtual void decode(                          )       = 0;
	virtual void store (      mipp::vector<B>& V_K) const = 0;

private:
	void resize_buffers()
	{
		if ((int)Y_N.size() != this->N * this->n_frames) Y_N.resize(this->N * this->n_frames);
		if ((int)V_K.size() != this->K * this->n_frames) V_K.resize(this->K * this->n_frames);
	}

	void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		assert((trans.get_data_length() / sizeof(R)) == Y_N.size());

		const R* buffer_in = (R*)trans.get_data_ptr();
		std::copy(buffer_in, buffer_in + Y_N.size(), Y_N.begin());

		this->load  (Y_N);
		this->decode(   );
		this->store (V_K);

		tlm::tlm_generic_payload payload;
		payload.set_data_ptr((unsigned char*)V_K.data());
		payload.set_data_length(V_K.size() * sizeof(B));

		sc_core::sc_time zero_time(sc_core::SC_ZERO_TIME);
		socket_out->b_transport(payload, zero_time);
	}
};

template <typename B, typename R>
using Decoder = SC_Decoder<B,R>;
#else
template <typename B, typename R>
using Decoder = Decoder_interface<B,R>;
#endif

#endif /* DECODER_HPP_ */