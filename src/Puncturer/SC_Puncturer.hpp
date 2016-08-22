#ifndef SC_PUNCTURER_HPP_
#define SC_PUNCTURER_HPP_

#ifdef SYSTEMC
#include <vector>
#include <string>
#include <cassert>
#include <systemc>
#include <tlm>
#include <tlm_utils/simple_target_socket.h>
#include <tlm_utils/simple_initiator_socket.h>

#include "../Tools/MIPP/mipp.h"

template <typename B, typename Q>
class SC_Puncturer;

template <typename B, typename Q>
class SC_Puncturer_sockets : public sc_core::sc_module
{
	SC_HAS_PROCESS(SC_Puncturer_sockets);

public:
	tlm_utils::simple_target_socket   <SC_Puncturer_sockets> in_punct;
	tlm_utils::simple_target_socket   <SC_Puncturer_sockets> in_depunct;
	tlm_utils::simple_initiator_socket<SC_Puncturer_sockets> out_punct;
	tlm_utils::simple_initiator_socket<SC_Puncturer_sockets> out_depunct;

private:
	SC_Puncturer<B,Q> &puncturer;
	mipp::vector<B> X_N1, X_N2;
	mipp::vector<Q> Y_N1, Y_N2;

public:
	SC_Puncturer_sockets(SC_Puncturer<B,Q> &puncturer, const sc_core::sc_module_name name = "SC_Puncturer_sockets")
	: sc_module(name), in_punct("in_punct"), in_depunct("in_depunct"), out_punct("out_punct"), out_depunct("out_depunct"),
	  puncturer(puncturer),
	  X_N1(puncturer.N_code * puncturer.n_frames),
	  X_N2(puncturer.N      * puncturer.n_frames),
	  Y_N1(puncturer.N      * puncturer.n_frames),
	  Y_N2(puncturer.N_code * puncturer.n_frames)
	{
		in_punct  .register_b_transport(this, &SC_Puncturer_sockets::b_transport_puncture);
		in_depunct.register_b_transport(this, &SC_Puncturer_sockets::b_transport_depuncture);
	}

	void resize_buffers()
	{
		if ((int)X_N1.size() != puncturer.N_code * puncturer.n_frames) X_N1.resize(puncturer.N_code * puncturer.n_frames);
		if ((int)X_N2.size() != puncturer.N      * puncturer.n_frames) X_N2.resize(puncturer.N      * puncturer.n_frames);
		if ((int)Y_N1.size() != puncturer.N      * puncturer.n_frames) Y_N1.resize(puncturer.N      * puncturer.n_frames);
		if ((int)Y_N2.size() != puncturer.N_code * puncturer.n_frames) Y_N2.resize(puncturer.N_code * puncturer.n_frames);
	}

private:
	void b_transport_puncture(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		assert((trans.get_data_length() / sizeof(B)) == X_N1.size());

		const B* buffer_in = (B*)trans.get_data_ptr();
		std::copy(buffer_in, buffer_in + X_N1.size(), X_N1.begin());

		puncturer.puncture(X_N1, X_N2);

		tlm::tlm_generic_payload payload;
		payload.set_data_ptr((unsigned char*)X_N2.data());
		payload.set_data_length(X_N2.size() * sizeof(B));

		sc_core::sc_time zero_time(sc_core::SC_ZERO_TIME);
		out_punct->b_transport(payload, zero_time);
	}

	void b_transport_depuncture(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		assert((trans.get_data_length() / sizeof(Q)) == (int)Y_N1.size());

		const Q* buffer_in = (Q*)trans.get_data_ptr();
		std::copy(buffer_in, buffer_in + Y_N1.size(), Y_N1.begin());

		puncturer.depuncture(Y_N1, Y_N2);

		tlm::tlm_generic_payload payload;
		payload.set_data_ptr((unsigned char*)Y_N2.data());
		payload.set_data_length(Y_N2.size() * sizeof(Q));

		sc_core::sc_time zero_time(sc_core::SC_ZERO_TIME);
		out_depunct->b_transport(payload, zero_time);
	}
};

template <typename B, typename Q>
class SC_Puncturer : public Puncturer_interface<B,Q>
{
	friend SC_Puncturer_sockets<B,Q>;

private:
	std::string name;

public:
	SC_Puncturer_sockets<B,Q> *sockets;

public:
	SC_Puncturer(const int K, const int N, const int N_code, const int n_frames = 1, 
	             const std::string name = "SC_Puncturer")
	: Puncturer_interface<B,Q>(K, N, N_code, n_frames, name), name(name), sockets(nullptr) {}

	virtual ~SC_Puncturer() { if (sockets != nullptr) { delete sockets; sockets = nullptr; } }

	virtual void   puncture(const mipp::vector<B>& X_N1, mipp::vector<B>& X_N2) const = 0;
	virtual void depuncture(const mipp::vector<Q>& Y_N1, mipp::vector<Q>& Y_N2) const = 0;

	virtual void set_n_frames(const int n_frames)
	{
		Puncturer_interface<B,Q>::set_n_frames(n_frames);

		if (sockets != nullptr)
			sockets->resize_buffers();
	}

	void create_sc_sockets()
	{
		this->sockets = new SC_Puncturer_sockets<B,Q>(*this, name.c_str());
	}
};

template <typename B, typename Q>
using Puncturer = SC_Puncturer<B,Q>;
#else
template <typename B, typename Q>
using Puncturer = Puncturer_interface<B,Q>;
#endif

#endif /* SC_PUNCTURER_HPP_ */