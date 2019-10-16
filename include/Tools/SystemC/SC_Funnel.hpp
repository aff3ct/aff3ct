#if defined(AFF3CT_SYSTEMC_SIMU) || defined(AFF3CT_SYSTEMC_MODULE)

#ifndef SC_FUNNEL_HPP_
#define SC_FUNNEL_HPP_

#include <systemc>
#include <tlm>
#include <tlm_utils/simple_target_socket.h>
#include <tlm_utils/simple_initiator_socket.h>

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

public:
	SC_Funnel(sc_core::sc_module_name name = "SC_Double_input");

private:
	void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& t);
};
}
}

#endif /* SC_FUNNEL_HPP_ */

#endif /* AFF3CT_SYSTEMC */
