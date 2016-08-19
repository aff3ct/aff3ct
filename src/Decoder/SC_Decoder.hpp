#ifndef SC_DECODER_HPP_
#define SC_DECODER_HPP_

#ifdef SYSTEMC
#include <vector>
#include <string>
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

public:
	SC_Decoder(const int K, const int N, const sc_core::sc_module_name name = "SC_Decoder")
	: sc_module(name), Decoder_interface<B,R>(K, N), socket_in("socket_in_SC_Decoder"), socket_out("socket_out_SC_Decoder")
	{
		socket_in.register_b_transport(this, &SC_Decoder::b_transport);
	};

	virtual ~SC_Decoder() {};

	virtual void load  (const mipp::vector<R>& Y_N)       = 0;
	virtual void decode(                          )       = 0;
	virtual void store (      mipp::vector<B>& V_K) const = 0;

private:
	void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		const R* buffer_in = (R*)trans.get_data_ptr();
		int length = trans.get_data_length() / sizeof(R);
		
		assert(length == this->N * this->get_n_frames());

		mipp::vector<R> Y_N(this->N * this->get_n_frames());
		mipp::vector<B> V_K(this->K * this->get_n_frames());

		std::copy(buffer_in, buffer_in + length, Y_N.begin());

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
template <typename R>
using Decoder = Decoder_interface<B,R>;
#endif

#endif /* DECODER_HPP_ */