#ifndef SC_ENCODER_HPP_
#define SC_ENCODER_HPP_

#ifdef SYSTEMC
#include <vector>
#include <string>
#include <cassert>
#include <systemc>
#include <tlm>
#include <tlm_utils/simple_target_socket.h>
#include <tlm_utils/simple_initiator_socket.h>

#include "../Tools/MIPP/mipp.h"

template <typename B>
class SC_Encoder : public sc_core::sc_module, public Encoder_interface<B>
{
	SC_HAS_PROCESS(SC_Encoder);

public:
	tlm_utils::simple_target_socket   <SC_Encoder> socket_in;
	tlm_utils::simple_initiator_socket<SC_Encoder> socket_out;

private:
	bool sockets_binded;
	mipp::vector<B> U_K;
	mipp::vector<B> X_N;

public:
	SC_Encoder(const int K, const int N, const int n_frames = 1, const sc_core::sc_module_name name = "SC_Encoder")
	: sc_module(name),
	  Encoder_interface<B>(K, N, n_frames),
	  socket_in ("socket_in_SC_Encoder"),
	  socket_out("socket_out_SC_Encoder"),
	  sockets_binded(false),
	  U_K(0),
	  X_N(0)
	{
	};

	virtual ~SC_Encoder() {};

	void register_sockets()
	{
		socket_in.register_b_transport(this, &SC_Encoder::b_transport);
		sockets_binded = true;

		this->resize_buffers();
	}

	bool socket_binded() { return sockets_binded; }

	virtual void encode(const mipp::vector<B>& U_K, mipp::vector<B>& X_N) = 0;

	virtual void set_n_frames(const int n_frames)
	{
		assert(n_frames > 0);
		this->n_frames = n_frames;

		if (sockets_binded)
			this->resize_buffers();
	}

private:
	void resize_buffers()
	{
		if ((int)U_K.size() != this->K * this->n_frames) this->U_K.resize(this->K * this->n_frames);
		if ((int)X_N.size() != this->N * this->n_frames) this->X_N.resize(this->N * this->n_frames);
	}

	void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		assert((trans.get_data_length() / sizeof(B)) == (int)U_K.size());

		const B* buffer_in = (B*)trans.get_data_ptr();
		std::copy(buffer_in, buffer_in + U_K.size(), U_K.begin());

		this->encode(U_K, X_N);

		tlm::tlm_generic_payload payload;
		payload.set_data_ptr((unsigned char*)X_N.data());
		payload.set_data_length(X_N.size() * sizeof(B));

		sc_core::sc_time zero_time(sc_core::SC_ZERO_TIME);
		socket_out->b_transport(payload, zero_time);
	}
};

template <typename B>
using Encoder = SC_Encoder<B>;
#else
template <typename B>
using Encoder = Encoder_interface<B>;
#endif

#endif /* SC_ENCODER_HPP_ */
