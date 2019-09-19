#if defined(AFF3CT_SYSTEMC_SIMU) || defined(AFF3CT_SYSTEMC_MODULE)

#include "Tools/SystemC/SC_Duplicator.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

SC_Duplicator
::SC_Duplicator(sc_core::sc_module_name name)
: sc_module(name), s_in("s_in"), s_out1("s_out1"), s_out2("s_out2")
{
	s_in.register_b_transport(this, &SC_Duplicator::b_transport);
}

void SC_Duplicator
::b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
{
	sc_core::sc_time zero_time(sc_core::SC_ZERO_TIME);
	s_out1->b_transport(trans, zero_time);
	s_out2->b_transport(trans, zero_time);
}

#endif /* AFF3CT_SYSTEMC_SIMU */
