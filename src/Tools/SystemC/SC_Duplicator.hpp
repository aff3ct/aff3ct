#if defined(SYSTEMC) || defined(SYSTEMC_MODULE)

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

	SI s_in;
	std::vector<SO_t*> s_out;

public:
	explicit SC_Duplicator(sc_core::sc_module_name name = "SC_Duplicator", unsigned n_output = 2)
	: sc_module(name), s_in("s_in"), s_out(n_output, nullptr)
	{
		s_in.register_b_transport(this, &SC_Duplicator::b_transport);

		for (unsigned i = 0; i < n_output ; i++)
			s_out[i] = new SO_t((std::string("s_out") + std::to_string(i)).c_str());
	}

	~SC_Duplicator()
	{
		for(auto& so: s_out)
			if (so != nullptr)
				delete so;
	}

	SO_t& operator[](size_t idx)
	{
		return *s_out[idx];
	}

private:
	void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		sc_core::sc_time zero_time(sc_core::SC_ZERO_TIME);
		for (auto& so : s_out)
			(*so)->b_transport(trans, zero_time);
	}
};
}
}

#endif /* SC_DUPLICATOR_HPP_ */

#endif /* SYSTEMC */
