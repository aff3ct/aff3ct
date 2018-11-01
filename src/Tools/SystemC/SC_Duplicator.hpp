#if defined(AFF3CT_SYSTEMC_SIMU) || defined(AFF3CT_SYSTEMC_MODULE)

#ifndef SC_DUPLICATOR_HPP_
#define SC_DUPLICATOR_HPP_

#include <string>
#include <typeinfo>
#include <systemc>
#include <tlm>
#include <tlm_utils/simple_target_socket.h>
#include <tlm_utils/simple_initiator_socket.h>

namespace aff3ct
{
namespace tools
{
class SC_Duplicator : sc_core::sc_module
{
	SC_HAS_PROCESS(SC_Duplicator);

public:
	using SI_t = tlm_utils::simple_target_socket   <SC_Duplicator>;
	using SO_t = tlm_utils::simple_initiator_socket<SC_Duplicator>;

	SI_t s_in;
	SO_t s_out1;
	SO_t s_out2;

	explicit SC_Duplicator(sc_core::sc_module_name name = "SC_Duplicator")
	: sc_module(name), s_in("s_in"), s_out1("s_out1"), s_out2("s_out2")
	{
		s_in.register_b_transport(this, &SC_Duplicator::b_transport);
	}

private:
	void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		sc_core::sc_time zero_time(sc_core::SC_ZERO_TIME);
		s_out1->b_transport(trans, zero_time);
		s_out2->b_transport(trans, zero_time);
	}

};

}
}

#endif /* SC_DUPLICATOR_HPP_ */

#endif /* AFF3CT_SYSTEMC_SIMU */
