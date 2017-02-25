#ifdef SYSTEMC

#ifndef SC_ROUTER_HPP_
#define SC_ROUTER_HPP_

#include <string>
#include <typeinfo>
#include <systemc>
#include <tlm>
#include <tlm_utils/simple_target_socket.h>
#include <tlm_utils/simple_initiator_socket.h>

#include "Tools/Algo/Predicate.hpp"

namespace aff3ct
{
namespace tools
{
class SC_Router : sc_core::sc_module
{
	SC_HAS_PROCESS(SC_Router);

public:
	tlm_utils::simple_target_socket   <SC_Router> s_in;
	tlm_utils::simple_initiator_socket<SC_Router> s_out1;
	tlm_utils::simple_initiator_socket<SC_Router> s_out2;

private:
	Predicate &p;

public:
    SC_Router(Predicate &p, sc_core::sc_module_name name = "SC_Router")
    : sc_module(name), s_in("s_in"), s_out1("s_out1"), s_out2("s_out2"), p(p)
	{
		s_in.register_b_transport(this, &SC_Router::b_transport);
	}

private:
	void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		sc_core::sc_time zero_time(sc_core::SC_ZERO_TIME);
		if (!p())
			s_out1->b_transport(trans, zero_time);
		else
			s_out2->b_transport(trans, zero_time);
	}
};
}
}

#endif /* SC_ROUTER_HPP_ */

#endif /* SYSTEMC */
