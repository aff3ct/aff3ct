#ifndef SC_ERROR_ANALYZER_HPP_
#define SC_ERROR_ANALYZER_HPP_

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

template <typename B>
class SC_Error_analyzer : public sc_core::sc_module, public Error_analyzer_interface<B>
{
	SC_HAS_PROCESS(SC_Error_analyzer);
	
public:
	tlm_utils::simple_target_socket   <SC_Error_analyzer> socket_in_source;
	tlm_utils::simple_target_socket   <SC_Error_analyzer> socket_in_decoder;
	tlm_utils::simple_initiator_socket<SC_Error_analyzer> socket_out;

private:
	bool sockets_binded;
	mipp::vector<B> U_K, V_K;

public:
	SC_Error_analyzer(const int K, const int N, const int n_frames = 1, const sc_core::sc_module_name name = "SC_Error_analyzer")
	: sc_module(name),
	  Error_analyzer_interface<B>(K, N, n_frames),
	  socket_in_source ("socket_in_source_SC_Error_analyzer"),
	  socket_in_decoder("socket_in_decoder_SC_Error_analyzer"),
	  socket_out       ("socket_out_SC_Error_analyzer"),
	  sockets_binded(false),
	  U_K(0),
	  V_K(0)
	{
	}

	virtual ~SC_Error_analyzer() {};

	void register_sockets()
	{
		socket_in_source .register_b_transport(this, &SC_Error_analyzer::b_transport_source);
		socket_in_decoder.register_b_transport(this, &SC_Error_analyzer::b_transport_decoder);
		sockets_binded = true;

		this->resize_buffers();
	}

	bool socket_binded() { return sockets_binded; }

	virtual void check_errors(const mipp::vector<B>& U, const mipp::vector<B>& V) = 0;

	virtual int get_n_be() const = 0;
	virtual int get_n_fe() const = 0;

	virtual float get_ber_value() const = 0;
	virtual float get_fer_value() const = 0;

	virtual unsigned long long get_n_analyzed_frames() const = 0;

	virtual int  get_fe_limit     () const = 0;
	virtual bool fe_limit_achieved() const = 0;

	virtual void set_n_frames(const int n_frames)
	{
		assert(n_frames > 0);
		this->n_frames = n_frames;

		if (sockets_binded)
			this->resize_buffers();
	}

private:
	void resize_buffers()
	{
		if ((int)U_K.size() != this->K * this->n_frames) this->U_K.resize(this->K * this->n_frames);
		if ((int)V_K.size() != this->K * this->n_frames) this->V_K.resize(this->K * this->n_frames);
	}

	void b_transport_source(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		assert((trans.get_data_length() / sizeof(B)) == U_K.size());

		const B* buffer_in = (B*)trans.get_data_ptr();
		std::copy(buffer_in, buffer_in + U_K.size(), U_K.begin());
	}

	void b_transport_decoder(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		assert((trans.get_data_length() / sizeof(B)) == V_K.size());

		const B* buffer_in = (B*)trans.get_data_ptr();
		std::copy(buffer_in, buffer_in + V_K.size(), V_K.begin());

		this->check_errors(U_K, V_K);

		// if (this->fe_limit_achieved())
		// 	sc_stop();

		tlm::tlm_generic_payload payload;
		payload.set_data_ptr((unsigned char*)V_K.data());
		payload.set_data_length(V_K.size() * sizeof(B));

		sc_core::sc_time zero_time(sc_core::SC_ZERO_TIME);
		socket_out->b_transport(payload, zero_time);
	}
};

template <typename B>
using Error_analyzer = SC_Error_analyzer<B>;
#else
template <typename B>
using Error_analyzer = Error_analyzer_interface<B>;
#endif

#endif /* SC_ERROR_ANALYZER_HPP_ */
