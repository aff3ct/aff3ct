#ifndef SC_PUNCTURER_HPP_
#define SC_PUNCTURER_HPP_

#ifdef SYSTEMC
#include <vector>
#include <string>
#include <systemc>
#include <tlm>
#include <tlm_utils/simple_target_socket.h>
#include <tlm_utils/simple_initiator_socket.h>

#include "../Tools/Frame_trace/Frame_trace.hpp"
#include "../Tools/MIPP/mipp.h"

template <typename B, typename Q>
class SC_Puncturer : public sc_core::sc_module, public Puncturer_interface<B,Q>
{
	SC_HAS_PROCESS(SC_Puncturer);

public:
	tlm_utils::simple_target_socket   <SC_Puncturer> socket_in_puncture;
	tlm_utils::simple_target_socket   <SC_Puncturer> socket_in_depuncture;
	tlm_utils::simple_initiator_socket<SC_Puncturer> socket_out;

public:
	SC_Puncturer(const int K, const int N, const int N_code, const int n_frames = 1, 
	             const sc_core::sc_module_name name = "SC_Puncturer")
	: sc_module(name), Puncturer_interface<B,Q>(K, N, N_code, n_frames),
	  socket_in_puncture  ("socket_in_puncture_SC_Puncturer"),
	  socket_in_depuncture("socket_in_depuncture_SC_Puncturer"),
	  socket_out          ("socket_out_SC_Puncturer")
	{
		socket_in_puncture  .register_b_transport(this, &SC_Puncturer::b_transport_puncture);
		socket_in_depuncture.register_b_transport(this, &SC_Puncturer::b_transport_depuncture);
	};

	virtual ~SC_Puncturer() {};

	virtual void   puncture(const mipp::vector<B>& X_N1, mipp::vector<B>& X_N2) const = 0;
	virtual void depuncture(const mipp::vector<Q>& Y_N1, mipp::vector<Q>& Y_N2) const = 0;

private:
	void b_transport_puncture(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		const B* buffer_in = (B*)trans.get_data_ptr();
		int length = trans.get_data_length() / sizeof(B);

		assert(length == this->N_code * this->n_frames);

		mipp::vector<B> X_N1(this->N_code * this->n_frames);
		mipp::vector<B> X_N2(this->N      * this->n_frames);

		std::copy(buffer_in, buffer_in + length, X_N1.begin());

		this->puncture(X_N1, X_N2);

		tlm::tlm_generic_payload payload;
		payload.set_data_ptr((unsigned char*)X_N2.data());
		payload.set_data_length(X_N2.size() * sizeof(B));

		sc_core::sc_time zero_time(sc_core::SC_ZERO_TIME);
		socket_out->b_transport(payload, zero_time);
	}

	void b_transport_depuncture(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		const Q* buffer_in = (Q*)trans.get_data_ptr();
		int length = trans.get_data_length() / sizeof(Q);

		assert(length == this->N * this->n_frames);

		mipp::vector<Q> Y_N1(this->N      * this->n_frames);
		mipp::vector<Q> Y_N2(this->N_code * this->n_frames);

		std::copy(buffer_in, buffer_in + length, Y_N1.begin());

		this->depuncture(Y_N1, Y_N2);

		tlm::tlm_generic_payload payload;
		payload.set_data_ptr((unsigned char*)Y_N2.data());
		payload.set_data_length(Y_N2.size() * sizeof(Q));

		sc_core::sc_time zero_time(sc_core::SC_ZERO_TIME);
		socket_out->b_transport(payload, zero_time);
	}
};

template <typename B, typename Q>
using Puncturer = SC_Puncturer<B,Q>;
#else
template <typename B, typename Q>
using Puncturer = Puncturer_interface<B,Q>;
#endif

#endif /* SC_PUNCTURER_HPP_ */