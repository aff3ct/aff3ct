#ifdef SYSTEMC

#ifndef SC_DUPLICATOR_HPP_
#define SC_DUPLICATOR_HPP_

#include <string>
#include <typeinfo>
#include <systemc>
#include <tlm>
#include <tlm_utils/simple_target_socket.h>
#include <tlm_utils/simple_initiator_socket.h>

#include "../Frame_trace/Frame_trace.hpp"

using namespace sc_core;

template <typename T>
class SC_Duplicator : sc_module
{
	SC_HAS_PROCESS(SC_Duplicator);

public:
	tlm_utils::simple_target_socket   <SC_Duplicator> socket_in;
	tlm_utils::simple_initiator_socket<SC_Duplicator> socket_out1;
	tlm_utils::simple_initiator_socket<SC_Duplicator> socket_out2;

public:
    SC_Duplicator(sc_module_name name = "SC_Duplicator")
    : sc_module(name), 
      socket_in  ("socket_in_SC_Duplicator"),
      socket_out1("socket_out1_SC_Duplicator"),
      socket_out2("socket_out2_SC_Duplicator")
	{
		socket_in.register_b_transport(this, &SC_Duplicator::b_transport);
	}

private:
	void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		sc_core::sc_time zero_time(sc_core::SC_ZERO_TIME);
		socket_out1->b_transport(trans, zero_time);
		socket_out2->b_transport(trans, zero_time);
	}
};

#endif /* SC_DUPLICATOR_HPP_ */

#endif /* SYSTEMC */