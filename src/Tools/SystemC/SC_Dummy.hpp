#ifdef SYSTEMC

#ifndef SC_DUMMY_HPP_
#define SC_DUMMY_HPP_

#include <systemc>
#include <tlm>
#include <tlm_utils/simple_target_socket.h>

using namespace sc_core;

template <typename T>
class SC_Dummy : sc_module
{
	SC_HAS_PROCESS(SC_Dummy);

public:
	tlm_utils::simple_target_socket<SC_Dummy> socket_in;

public:
    SC_Dummy(sc_module_name name)
    : sc_module(name), 
      socket_in("socket_in_SC_Dummy")
	{
		socket_in.register_b_transport(this, &SC_Dummy::b_transport);
	}

private:
	void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
	}
};

#endif /* SC_DUMMY_HPP_ */

#endif /* SYSTEMC */