#if defined(AFF3CT_SYSTEMC_SIMU) || defined(AFF3CT_SYSTEMC_MODULE)

#include "Tools/SystemC/SC_Dummy.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

SC_Dummy
::SC_Dummy(sc_core::sc_module_name name)
: sc_module(name), s_in("s_in")
{
	s_in.register_b_transport(this, &SC_Dummy::b_transport);
}

void SC_Dummy
::b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
{
}

#endif /* AFF3CT_SYSTEMC_SIMU */
