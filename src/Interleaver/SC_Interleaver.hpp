#ifndef SC_INTERLEAVER_HPP_
#define SC_INTERLEAVER_HPP_

#ifdef SYSTEMC
#include <vector>
#include <string>
#include <cassert>
#include <systemc>
#include <tlm>
#include <tlm_utils/simple_target_socket.h>
#include <tlm_utils/simple_initiator_socket.h>

#include "../Tools/Frame_trace/Frame_trace.hpp"
#include "../Tools/MIPP/mipp.h"

template <typename T>
class SC_Interleaver : public sc_core::sc_module, public Interleaver_interface<T>
{
	SC_HAS_PROCESS(SC_Interleaver);
	
public:
	tlm_utils::simple_target_socket   <SC_Interleaver> socket_in_interleave;
	tlm_utils::simple_target_socket   <SC_Interleaver> socket_in_deinterleave;
	tlm_utils::simple_initiator_socket<SC_Interleaver> socket_out;

public:
	SC_Interleaver(const int size, const int n_frames = 1, const sc_core::sc_module_name name = "SC_Interleaver")
	: sc_module(name), Interleaver_interface<T>(size, n_frames), 
	  socket_in_interleave  ("socket_in_interleave_SC_Interleaver"),
	  socket_in_deinterleave("socket_in_deinterleave_SC_Interleaver"), 
	  socket_out            ("socket_out_SC_Interleaver")
	{
		socket_in_interleave  .register_b_transport(this, &SC_Interleaver::b_transport_interleave);
		socket_in_deinterleave.register_b_transport(this, &SC_Interleaver::b_transport_deinterleave);
	}

	virtual ~SC_Interleaver() {};

protected:
	virtual void gen_lookup_tables() = 0; // to implement

private:
	void b_transport_interleave(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		assert(trans.get_data_length() % (this->pi.size() * this->n_frames) == 0);

		int size_of_data = trans.get_data_length() / (this->pi.size() * this->n_frames);
		switch (size_of_data)
		{
			case 1: _b_transport_interleave<char     >(trans, t); break;
			case 2: _b_transport_interleave<short    >(trans, t); break;
			case 4: _b_transport_interleave<int      >(trans, t); break;
			case 8: _b_transport_interleave<long long>(trans, t); break;
			default:
				std::cerr << "(EE) Unrecognized type of data, exiting." << std::endl; std::exit(-1);
				break;
		}
	}

	template <typename D>
	void _b_transport_interleave(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		const D* buffer_in = (D*)trans.get_data_ptr();
		int length = trans.get_data_length() / sizeof(D);
		
		mipp::vector<D> natural_vec(length), interleaved_vec(length);
		std::copy(buffer_in, buffer_in + length, natural_vec.begin());

		this->interleave(natural_vec, interleaved_vec);

		tlm::tlm_generic_payload payload;
		payload.set_data_ptr((unsigned char*)interleaved_vec.data());
		payload.set_data_length(interleaved_vec.size() * sizeof(D));

		sc_core::sc_time zero_time(sc_core::SC_ZERO_TIME);
		socket_out->b_transport(payload, zero_time);
	}

	void b_transport_deinterleave(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		assert(trans.get_data_length() % (this->pi.size() * this->n_frames) == 0);

		int size_of_data = trans.get_data_length() / (this->pi.size() * this->n_frames);
		switch (size_of_data)
		{
			case 1: _b_transport_deinterleave<char     >(trans, t); break;
			case 2: _b_transport_deinterleave<short    >(trans, t); break;
			case 4: _b_transport_deinterleave<int      >(trans, t); break;
			case 8: _b_transport_deinterleave<long long>(trans, t); break;
			default:
				std::cerr << "(EE) Unrecognized type of data, exiting." << std::endl; std::exit(-1);
				break;
		}
	}

	template <typename D>
	void _b_transport_deinterleave(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		const D* buffer_in = (D*)trans.get_data_ptr();
		int length = trans.get_data_length() / sizeof(D);
		
		mipp::vector<D> interleaved_vec(length), natural_vec(length);
		std::copy(buffer_in, buffer_in + length, interleaved_vec.begin());

		this->deinterleave(interleaved_vec, natural_vec);

		tlm::tlm_generic_payload payload;
		payload.set_data_ptr((unsigned char*)natural_vec.data());
		payload.set_data_length(natural_vec.size() * sizeof(D));

		sc_core::sc_time zero_time(sc_core::SC_ZERO_TIME);
		socket_out->b_transport(payload, zero_time);
	}
};

template <typename T>
using Interleaver = SC_Interleaver<T>;
#else
template <typename T>
using Interleaver = Interleaver_interface<T>;
#endif

#endif /* SC_INTERLEAVER_HPP_ */