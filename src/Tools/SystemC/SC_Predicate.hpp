#if defined(AFF3CT_SYSTEMC_SIMU) || defined(AFF3CT_SYSTEMC_MODULE)

#ifndef SC_PREDICATE_HPP_
#define SC_PREDICATE_HPP_

#include <systemc>
#include <tlm>
#include <tlm_utils/simple_target_socket.h>

#include "Tools/Algo/Predicate.hpp"

namespace aff3ct
{
namespace tools
{
class SC_Predicate : sc_core::sc_module
{
	SC_HAS_PROCESS(SC_Predicate);

public:
	tlm_utils::simple_target_socket<SC_Predicate> s_in;

private:
	Predicate &p;

public:
	SC_Predicate(Predicate &p, sc_core::sc_module_name name);

private:
	void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& t);
};
}
}

#endif /* SC_PREDICATE_HPP_ */

#endif /* AFF3CT_SYSTEMC_SIMU */
