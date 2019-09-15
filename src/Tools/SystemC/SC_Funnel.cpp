#if defined(AFF3CT_SYSTEMC_SIMU) || defined(AFF3CT_SYSTEMC_MODULE)

#include "Tools/SystemC/SC_Funnel.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

SC_Funnel
::SC_Funnel(sc_core::sc_module_name name)
: sc_module(name), s_in1("s_in1"), s_in2("s_in2"), s_out("s_out")
{
	s_in1.register_b_transport(this, &SC_Funnel::b_transport);
	s_in2.register_b_transport(this, &SC_Funnel::b_transport);
}

void SC_Funnel
::b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
{
	sc_core::sc_time zero_time(sc_core::SC_ZERO_TIME);
	s_out->b_transport(trans, zero_time);
}

#endif /* AFF3CT_SYSTEMC */
