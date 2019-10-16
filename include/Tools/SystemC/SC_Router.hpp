#if defined(AFF3CT_SYSTEMC_SIMU) || defined(AFF3CT_SYSTEMC_MODULE)

#ifndef SC_ROUTER_HPP_
#define SC_ROUTER_HPP_

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
	SC_Router(Predicate &p, sc_core::sc_module_name name = "SC_Router");

private:
	void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& t);
};
}
}

#endif /* SC_ROUTER_HPP_ */

#endif /* AFF3CT_SYSTEMC_SIMU */
