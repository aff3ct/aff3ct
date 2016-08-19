#include <systemc>
#include <tlm>
#include <tlm_utils/simple_target_socket.h>

#include "../Frame_trace/Frame_trace.hpp"

using namespace sc_core;

template <typename B>
class SC_Dummy : sc_module
{
public:
	tlm_utils::simple_target_socket<SC_Dummy> socket_in;

public:
    SC_Dummy(sc_module_name name)
    : sc_module(name), socket_in("socket_in_SC_Dummy")
	{
		socket_in.register_b_transport(this, &SC_Dummy::b_transport);
	}
	SC_HAS_PROCESS(SC_Dummy);

private:
	void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		B* buffer = (B*)trans.get_data_ptr();
		int length = trans.get_data_length() / sizeof(B);
		
		mipp::vector<B> data_in(length);
		std::copy(buffer, buffer + length, data_in.begin());

		/* DEBUG */
		Frame_trace<B> ft;
		std::cout << "Dummy intput:" << std::endl;
		ft.display_bit_vector(data_in);
		std::cout << std::endl;
	}
};
