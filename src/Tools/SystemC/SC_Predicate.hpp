#ifdef SYSTEMC

#ifndef SC_PREDICATE_HPP_
#define SC_PREDICATE_HPP_

#include <systemc>
#include <tlm>
#include <tlm_utils/simple_target_socket.h>

#include "../Math/Predicate.hpp"

class SC_Predicate : sc_core::sc_module
{
	SC_HAS_PROCESS(SC_Predicate);

public:
	tlm_utils::simple_target_socket<SC_Predicate> s_in;

private:
	Predicate &p;

public:
    SC_Predicate(Predicate &p, sc_core::sc_module_name name)
    : sc_module(name), s_in("s_in"), p(p)
	{
		s_in.register_b_transport(this, &SC_Predicate::b_transport);
	}

private:
	void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		p.reset();
	}
};

#endif /* SC_PREDICATE_HPP_ */

#endif /* SYSTEMC */