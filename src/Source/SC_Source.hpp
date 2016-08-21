#ifndef SC_SOURCE_HPP_
#define SC_SOURCE_HPP_

#ifdef SYSTEMC
#include <vector>
#include <string>
#include <systemc>
#include <cassert>
#include <tlm>
#include <tlm_utils/simple_initiator_socket.h>

#include "../Tools/Frame_trace/Frame_trace.hpp"
#include "../Tools/MIPP/mipp.h"

template <typename B>
class SC_Source : public sc_core::sc_module, public Source_interface<B>
{
	SC_HAS_PROCESS(SC_Source);

public:
	tlm_utils::simple_initiator_socket<SC_Source> socket_out;

private:
	mipp::vector<B> U_K;

public:
	SC_Source(const int K, const int n_frames = 1, const sc_core::sc_module_name name = "Source_SC")
	: sc_module(name), 
	  Source_interface<B>(K, n_frames), 
	  socket_out("socket_out_SC_Source"),
	  U_K(K * n_frames)
	{ 
		SC_THREAD(sc_generate); 
	}

	virtual ~SC_Source() {};

	virtual void generate(mipp::vector<B>& U_K) = 0;

	virtual void set_n_frames(const int n_frames)
	{
		assert(n_frames > 0);
		this->n_frames = n_frames;

		if ((int)U_K.size() != this->K * this->n_frames) this->U_K.resize(this->K * this->n_frames);
	}

private:
	void sc_generate()
	{
		while (1)
		{
			generate(U_K);

			tlm::tlm_generic_payload payload;
			payload.set_data_ptr((unsigned char*)U_K.data());
			payload.set_data_length(U_K.size() * sizeof(B));

			/* DEBUG */
			Frame_trace<B> ft;
			std::cout << "Source output:" << std::endl;
			ft.display_bit_vector(U_K);
			std::cout << std::endl;

			sc_core::sc_time zero_time(sc_core::SC_ZERO_TIME);
			socket_out->b_transport(payload, zero_time);

			sc_core::wait(10, sc_core::SC_MS);
		}
	}
};

template <typename B>
using Source = SC_Source<B>;
#else
template <typename B>
using Source = Source_interface<B>;
#endif

#endif /* SC_SOURCE_HPP_ */
