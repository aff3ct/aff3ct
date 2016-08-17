#ifndef SOURCE_HPP_
#define SOURCE_HPP_

#include <vector>
#include <string>
#ifdef SYSTEMC
#include <systemc>
#include <tlm>
#include <tlm_utils/simple_initiator_socket.h>
#endif

#include "../Tools/MIPP/mipp.h"

template <typename B>
class Source 
#ifdef SYSTEMC
: public sc_core::sc_module
#endif
{
public:
#ifndef SYSTEMC
	Source(const std::string name = "Source") {};
#else
	Source(const std::string name = "Source") : sc_module(name.c_str()), socket("socket") { SC_THREAD(sc_generate); };
	SC_HAS_PROCESS(Source);
#endif
	virtual ~Source() {};

	virtual void generate(mipp::vector<B>& U_K) = 0;

#ifdef SYSTEMC
	void sc_generate() 
	{
		while (1)
		{
			mipp::vector<B> U_K(10);
			tlm::tlm_generic_payload payload;
			sc_core::sc_time zero_time(sc_core::SC_ZERO_TIME);

			generate(U_K);
			payload.set_data_ptr((unsigned char*)U_K.data());
			payload.set_data_length(U_K.size() * sizeof(int));

			std::cout << "Input:  [";
	    	for(auto i = 0; i < U_K.size(); i++) {
	    		std::cout << U_K[i] << ", ";
	    	}
	    	std::cout << "]" << std::endl;

			socket->b_transport(payload, zero_time);

			sc_core::wait(10, sc_core::SC_MS);
		}
	}

	tlm_utils::simple_initiator_socket<Source> socket;
#endif
};

#endif /* SOURCE_HPP_ */
