#include <systemc>
#include <tlm>
#include <tlm_utils/simple_target_socket.h>

using namespace sc_core;

template <typename B>
class Dummy : sc_module
{
    SC_HAS_PROCESS(Dummy);
public:
    Dummy(sc_module_name name):
            sc_module(name),
            socket("socket")
    {
        socket.register_b_transport(this, &Dummy::b_transport);
    }

private:

    void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
    {
    	B* data = (B*)trans.get_data_ptr();
    	int length = trans.get_data_length() / sizeof(B);
    	std::cout << "Output: [";
    	for(auto i = 0; i < length; i++) {
    		std::cout << data[i] << ", ";
    	}
    	std::cout << "]" << std::endl;
    }

public:
    tlm_utils::simple_target_socket<Dummy> socket;

};
