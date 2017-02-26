#ifndef SC_ENCODER_HPP_
#define SC_ENCODER_HPP_

#ifdef SYSTEMC
#include <vector>
#include <string>
#include <cassert>
#include <systemc>
#include <tlm>
#include <tlm_utils/simple_target_socket.h>
#include <tlm_utils/simple_initiator_socket.h>

#include "Tools/Perf/MIPP/mipp.h"

namespace aff3ct
{
namespace module
{
template <typename B>
class SC_Encoder;

template <typename B = int>
class SC_Encoder_module : public sc_core::sc_module
{
	SC_HAS_PROCESS(SC_Encoder_module);

public:
	tlm_utils::simple_target_socket   <SC_Encoder_module> s_in;
	tlm_utils::simple_initiator_socket<SC_Encoder_module> s_out;

private:
	SC_Encoder<B> &encoder;
	mipp::vector<B> U_K;
	mipp::vector<B> X_N;

public:
	SC_Encoder_module(SC_Encoder<B> &encoder, const sc_core::sc_module_name name = "SC_Encoder_module")
	: sc_module(name), s_in("s_in"), s_out("s_out"),
	  encoder(encoder),
	  U_K(encoder.K * encoder.n_frames),
	  X_N(encoder.N * encoder.n_frames)
	{
		s_in.register_b_transport(this, &SC_Encoder_module::b_transport);
	}

	void resize_buffers()
	{
		if ((int)U_K.size() != encoder.K * encoder.n_frames) U_K.resize(encoder.K * encoder.n_frames);
		if ((int)X_N.size() != encoder.N * encoder.n_frames) X_N.resize(encoder.N * encoder.n_frames);
	}

private:
	void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		assert((trans.get_data_length() / sizeof(B)) == (int)U_K.size());

		const B* buffer_in = (B*)trans.get_data_ptr();
		std::copy(buffer_in, buffer_in + U_K.size(), U_K.begin());

		encoder.encode(U_K, X_N);

		tlm::tlm_generic_payload payload;
		payload.set_data_ptr((unsigned char*)X_N.data());
		payload.set_data_length(X_N.size() * sizeof(B));

		sc_core::sc_time zero_time(sc_core::SC_ZERO_TIME);
		s_out->b_transport(payload, zero_time);
	}
};

template <typename B>
class SC_Encoder : public Encoder_i<B>
{
	friend SC_Encoder_module<B>;

public:
	SC_Encoder_module<B> *module;

public:
	SC_Encoder(const int K, const int N, const int n_frames = 1, const std::string name = "SC_Encoder")
	: Encoder_i<B>(K, N, n_frames, name), module(nullptr) {}

	virtual ~SC_Encoder() { if (module != nullptr) { delete module; module = nullptr; } }

	virtual void set_n_frames(const int n_frames)
	{
		Encoder_i<B>::set_n_frames(n_frames);

		if (module != nullptr)
			module->resize_buffers();
	}

	void create_sc_module()
	{
		this->module = new SC_Encoder_module<B>(*this, this->name.c_str());
	}
};

template <typename B>
using Encoder = SC_Encoder<B>;
}
}
#else
#include "SPU_Encoder.hpp"
#endif

#endif /* SC_ENCODER_HPP_ */
