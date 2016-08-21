#ifndef SC_CRC_HPP_
#define SC_CRC_HPP_

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

template <typename B>
class SC_CRC : public sc_core::sc_module, public CRC_interface<B>
{
	SC_HAS_PROCESS(SC_CRC);

public:
	tlm_utils::simple_target_socket   <SC_CRC> socket_in;
	tlm_utils::simple_initiator_socket<SC_CRC> socket_out;

private:
	bool sockets_binded;
	mipp::vector<B> U_K;

public:
	SC_CRC(const int K, const int n_frames = 1, const sc_core::sc_module_name name = "SC_CRC") 
	: sc_module(name),
	  CRC_interface<B>(K, n_frames),
	  socket_in ("socket_in_SC_CRC"),
	  socket_out("socket_out_SC_CRC"),
	  sockets_binded(false),
	  U_K(0)
	{
	}

	virtual ~SC_CRC() {};

	void register_sockets()
	{
		socket_in.register_b_transport(this, &SC_CRC::b_transport);
		sockets_binded = true;

		this->resize_buffers();
	}

	bool socket_binded() { return sockets_binded; }

	virtual void build(mipp::vector<B>& U_K) = 0;

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
	}

	void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		assert((trans.get_data_length() / sizeof(B)) == (int)U_K.size());

		const B* buffer_in = (B*)trans.get_data_ptr();
		std::copy(buffer_in, buffer_in + U_K.size(), U_K.begin());

		this->build(U_K);

		tlm::tlm_generic_payload payload;
		payload.set_data_ptr((unsigned char*)U_K.data());
		payload.set_data_length(U_K.size() * sizeof(B));

		sc_core::sc_time zero_time(sc_core::SC_ZERO_TIME);
		socket_out->b_transport(payload, zero_time);
	}
};

template <typename B>
using CRC = SC_CRC<B>;
#else
template <typename B>
using CRC = CRC_interface<B>;
#endif

#endif /* SC_CRC_HPP_ */
