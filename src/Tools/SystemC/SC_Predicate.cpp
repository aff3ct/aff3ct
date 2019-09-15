#if defined(AFF3CT_SYSTEMC_SIMU) || defined(AFF3CT_SYSTEMC_MODULE)

#include "Tools/SystemC/SC_Predicate.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

SC_Predicate
::SC_Predicate(Predicate &p, sc_core::sc_module_name name)
: sc_module(name), s_in("s_in"), p(p)
{
	s_in.register_b_transport(this, &SC_Predicate::b_transport);
}

void SC_Predicate
::b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
{
	p.reset();
}

#endif /* AFF3CT_SYSTEMC_SIMU */
