#ifdef SYSTEMC

#ifndef SC_DEBUG_HPP_
#define SC_DEBUG_HPP_

#include <string>
#include <typeinfo>
#include <systemc>
#include <tlm>
#include <tlm_utils/simple_target_socket.h>
#include <tlm_utils/simple_initiator_socket.h>

#include "Tools/Display/Frame_trace/Frame_trace.hpp"

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
	SC_Debug(std::string message = "Debug:\n", const int debug_limit = 0, sc_core::sc_module_name name = "SC_Debug")
	: sc_module(name), s_in("s_in"), s_out("s_out"), message(message), debug_limit(debug_limit)
	{
		s_in.register_b_transport(this, &SC_Debug::b_transport);
	}

private:
	void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		T* buffer = (T*)trans.get_data_ptr();
		int length = trans.get_data_length() / sizeof(T);
		
		mipp::vector<T> data_in(length);
		std::copy(buffer, buffer + length, data_in.begin());

		// display input data
		Frame_trace<T> ft(debug_limit);
		std::cout << message;
		if (typeid(T) == typeid(float) || typeid(T) == typeid(double))
			ft.display_real_vector(data_in);
		else
			ft.display_bit_vector(data_in);
		std::cout << std::endl;

		tlm::tlm_generic_payload payload;
		payload.set_data_ptr((unsigned char*)data_in.data());
		payload.set_data_length(data_in.size() * sizeof(T));

		sc_core::sc_time zero_time(sc_core::SC_ZERO_TIME);
		s_out->b_transport(payload, zero_time);
	}
};

#endif /* SC_DEBUG_HPP_ */

#endif /* SYSTEMC */
