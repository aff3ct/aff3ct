#ifndef SOURCE_SC_HPP_
#define SOURCE_SC_HPP_

#include <vector>
#include <string>
#ifdef SYSTEMC
#include <systemc>
#include <tlm>
#include <tlm_utils/simple_initiator_socket.h>
#endif

#include "../Tools/MIPP/mipp.h"

#ifdef SYSTEMC
template <typename B>
class Source_SC : public sc_core::sc_module
{
private:
	tlm_utils::simple_initiator_socket<Source_SC> socket;

public:
	Source_SC(const std::string name = "Source_SC") 
	: sc_module(name.c_str()), socket("socket") 
	{ 
		SC_THREAD(generate_SC); 
	}

	SC_HAS_PROCESS(Source_SC);

	virtual ~Source_SC() {};

	virtual void generate(mipp::vector<B>& U_K) = 0;

	void generate_SC()
	{
		while (1)
		{
			mipp::vector<B> U_K(10);
			tlm::tlm_generic_payload payload;
			sc_core::sc_time zero_time(sc_core::SC_ZERO_TIME);

			generate(U_K);
			payload.set_data_ptr((unsigned char*)U_K.data());
			payload.set_data_length(U_K.size() * sizeof(int));

			/* DEBUG */
			std::cout << "Input:  [";
	    	for(auto i = 0; i < U_K.size(); i++) {
	    		std::cout << U_K[i] << ", ";
	    	}
	    	std::cout << "]" << std::endl;

			socket->b_transport(payload, zero_time);

			sc_core::wait(10, sc_core::SC_MS);
		}
	}
};
#else
template <typename B>
class Source_SC
{
public:
	Source_SC(const std::string name = "Source_SC") {};
	virtual ~Source_SC() {};
};
#endif

#endif /* SOURCE_SC_HPP_ */
