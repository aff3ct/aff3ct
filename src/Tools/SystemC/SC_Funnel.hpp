#if defined(SYSTEMC) || defined(SYSTEMC_MODULE)

#ifndef SC_FUNNEL_HPP_
#define SC_FUNNEL_HPP_

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
class SC_Funnel : sc_core::sc_module
{
	SC_HAS_PROCESS(SC_Funnel);

public:
	tlm_utils::simple_target_socket   <SC_Funnel> s_in1;
	tlm_utils::simple_target_socket   <SC_Funnel> s_in2;
	tlm_utils::simple_initiator_socket<SC_Funnel> s_out;

private:
	Predicate &p;

public:
	SC_Funnel(Predicate &p, sc_core::sc_module_name name = "SC_Double_input")
	: sc_module(name), s_in1("s_in1"), s_in2("s_in2"), s_out("s_out"), p(p)
	{
		s_in1.register_b_transport(this, &SC_Funnel::b_transport);
		s_in2.register_b_transport(this, &SC_Funnel::b_transport);
	}

private:
	void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		sc_core::sc_time zero_time(sc_core::SC_ZERO_TIME);
		s_out->b_transport(trans, zero_time);
	}
};
}
}

#endif /* SC_FUNNEL_HPP_ */

#endif /* SYSTEMC */
