#if defined(AFF3CT_SYSTEMC_SIMU) || defined(AFF3CT_SYSTEMC_MODULE)

#ifndef SC_DUMMY_HPP_
#define SC_DUMMY_HPP_

#include <systemc>
#include <tlm>
#include <tlm_utils/simple_target_socket.h>

namespace aff3ct
{
namespace tools
{
class SC_Dummy : sc_core::sc_module
{
	SC_HAS_PROCESS(SC_Dummy);

public:
	tlm_utils::simple_target_socket<SC_Dummy> s_in;

public:
	explicit SC_Dummy(sc_core::sc_module_name name)
	: sc_module(name), s_in("s_in")
	{
		s_in.register_b_transport(this, &SC_Dummy::b_transport);
	}

private:
	void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
	}
};
}
}

#endif /* SC_DUMMY_HPP_ */

#endif /* AFF3CT_SYSTEMC_SIMU */
