#ifndef SC_QUANTIZER_HPP_
#define SC_QUANTIZER_HPP_

#ifdef SYSTEMC
#include <vector>
#include <string>
#include <cassert>
#include <systemc>
#include <tlm>
#include <tlm_utils/simple_target_socket.h>
#include <tlm_utils/simple_initiator_socket.h>

#include "../Tools/MIPP/mipp.h"

template <typename R, typename Q>
class SC_Quantizer : public sc_core::sc_module, public Quantizer_interface<R,Q>
{
	SC_HAS_PROCESS(SC_Quantizer);
	
public:
	tlm_utils::simple_target_socket   <SC_Quantizer> socket_in;
	tlm_utils::simple_initiator_socket<SC_Quantizer> socket_out;

private:
	bool sockets_binded;
	mipp::vector<R> Y_N1;
	mipp::vector<Q> Y_N2;

public:
	SC_Quantizer(const int N, const int n_frames = 1, const sc_core::sc_module_name name = "SC_Quantizer")
	: sc_module(name),
	  Quantizer_interface<R,Q>(N, n_frames), 
	  socket_in("socket_in_SC_Quantizer"), 
	  socket_out("socket_out_SC_Quantizer"),
	  sockets_binded(false),
	  Y_N1(0),
	  Y_N2(0)
	{ 
	}

	virtual ~SC_Quantizer() {};

	void register_sockets()
	{
		socket_in.register_b_transport(this, &SC_Quantizer::b_transport);
		sockets_binded = true;

		this->resize_buffers();
	}

	bool socket_binded() { return sockets_binded; }

	virtual void process(const mipp::vector<R>& Y_N1, mipp::vector<Q>& Y_N2) = 0;

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
		if ((int)Y_N1.size() != this->N * this->n_frames) this->Y_N1.resize(this->N * this->n_frames);
		if ((int)Y_N2.size() != this->N * this->n_frames) this->Y_N2.resize(this->N * this->n_frames);
	}

	void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		assert((trans.get_data_length() / sizeof(R)) == Y_N1.size());

		const R* buffer_in = (R*)trans.get_data_ptr();
		std::copy(buffer_in, buffer_in + Y_N1.size(), Y_N1.begin());

		this->process(Y_N1, Y_N2);

		tlm::tlm_generic_payload payload;
		payload.set_data_ptr((unsigned char*)Y_N2.data());
		payload.set_data_length(Y_N2.size() * sizeof(Q));

		sc_core::sc_time zero_time(sc_core::SC_ZERO_TIME);
		socket_out->b_transport(payload, zero_time);
	}
};

template <typename R, typename Q>
using Quantizer = SC_Quantizer<R,Q>;
#else
template <typename R, typename Q>
using Quantizer = Quantizer_interface<R,Q>;
#endif

#endif /* SC_QUANTIZER_HPP_ */
