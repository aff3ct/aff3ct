#if defined(AFF3CT_SYSTEMC_SIMU) || defined(AFF3CT_SYSTEMC_MODULE)

#ifndef SC_DEBUG_HPP_
#define SC_DEBUG_HPP_

#include <string>
#include <systemc>
#include <tlm>
#include <tlm_utils/simple_target_socket.h>
#include <tlm_utils/simple_initiator_socket.h>

namespace aff3ct
{
namespace tools
{
template <typename T>
class SC_Debug : sc_core::sc_module
{
	SC_HAS_PROCESS(SC_Debug);

public:
	tlm_utils::simple_target_socket   <SC_Debug> s_in;
	tlm_utils::simple_initiator_socket<SC_Debug> s_out;

private:
	std::string message;
	const int debug_limit;

public:
	SC_Debug(const std::string &message = std::string("Debug:\n"), const int debug_limit = 0,
	         sc_core::sc_module_name name = "SC_Debug");

private:
	void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& t);
};
}
}

#include "Tools/SystemC/SC_Debug.hxx"

#endif /* SC_DEBUG_HPP_ */

#endif /* AFF3CT_SYSTEMC_SIMU */
