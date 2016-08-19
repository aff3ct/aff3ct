#ifndef SC_QUANTIZER_HPP_
#define SC_QUANTIZER_HPP_

#ifdef SYSTEMC
#include <vector>
#include <string>
#include <systemc>
#include <tlm>
#include <tlm_utils/simple_target_socket.h>
#include <tlm_utils/simple_initiator_socket.h>

#include "../Tools/Frame_trace/Frame_trace.hpp"
#include "../Tools/MIPP/mipp.h"

template <typename R, typename Q>
class SC_Quantizer : public sc_core::sc_module, public Quantizer_interface<R,Q>
{
	SC_HAS_PROCESS(SC_Quantizer);
	
public:
	tlm_utils::simple_target_socket   <SC_Quantizer> socket_in;
	tlm_utils::simple_initiator_socket<SC_Quantizer> socket_out;

public:
	SC_Quantizer(const sc_core::sc_module_name name = "SC_Quantizer")
	: sc_module(name), Quantizer_interface<R,Q>(), 
	  socket_in("socket_in_SC_Quantizer"), 
	  socket_out("socket_out_SC_Quantizer")
	{ 
		socket_in.register_b_transport(this, &SC_Quantizer::b_transport);
	}

	virtual ~SC_Quantizer() {};

	virtual void process(const mipp::vector<R>& Y_N1, mipp::vector<Q>& Y_N2) = 0;

private:
	void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		const R* buffer_in = (R*)trans.get_data_ptr();
		int length = trans.get_data_length() / sizeof(R);
		
		mipp::vector<R> Y_N1(length);
		mipp::vector<Q> Y_N2(length);

		std::copy(buffer_in, buffer_in + length, Y_N1.begin());

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
