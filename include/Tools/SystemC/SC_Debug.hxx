#if defined(AFF3CT_SYSTEMC_SIMU) || defined(AFF3CT_SYSTEMC_MODULE)

#include <algorithm>
#include <typeinfo>
#include <iostream>
#include <vector>

#include "Tools/Display/Frame_trace/Frame_trace.hpp"
#include "Tools/SystemC/SC_Debug.hpp"

namespace aff3ct
{
namespace tools
{
template <typename T>
SC_Debug<T>
::SC_Debug(const std::string &message, const int debug_limit, sc_core::sc_module_name name)
: sc_module(name), s_in("s_in"), s_out("s_out"), message(message), debug_limit(debug_limit)
{
	s_in.register_b_transport(this, &SC_Debug::b_transport);
}

template <typename T>
void SC_Debug<T>
::b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
{
	T* buffer = (T*)trans.get_data_ptr();
	int length = trans.get_data_length() / sizeof(T);

	std::vector<T> data_in(length);
	std::copy(buffer, buffer + length, data_in.begin());

	// display input data
	Frame_trace<T> ft(debug_limit);
	std::cout << message;
	if (typeid(T) == typeid(float) || typeid(T) == typeid(double))
		ft.display_real_vector(data_in);
	else
		ft.display_bit_vector(data_in);
	std::cout << std::endl;

	sc_core::sc_time zero_time(sc_core::SC_ZERO_TIME);
	s_out->b_transport(trans, zero_time);
}
}
}

#endif /* AFF3CT_SYSTEMC_SIMU */
