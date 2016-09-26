#ifndef SC_QUANTIZER_HPP_
#define SC_QUANTIZER_HPP_

#ifdef SYSTEMC
#include <vector>
#include <string>
#include <cassert>
#include <systemc>
#include <tlm>
#include <tlm_utils/simple_target_socket.h>
#include <tlm_utils/simple_initiator_socket.h>

#include "Tools/MIPP/mipp.h"

template <typename R, typename Q>
class SC_Quantizer;

template <typename R, typename Q>
class SC_Quantizer_module : public sc_core::sc_module
{
	SC_HAS_PROCESS(SC_Quantizer_module);

public:
	tlm_utils::simple_target_socket   <SC_Quantizer_module> s_in;
	tlm_utils::simple_initiator_socket<SC_Quantizer_module> s_out;

private:
	SC_Quantizer<R,Q> &quantizer;
	mipp::vector<R> Y_N1;
	mipp::vector<Q> Y_N2;

public:
	SC_Quantizer_module(SC_Quantizer<R,Q> &quantizer, const sc_core::sc_module_name name = "SC_Quantizer_module")
	: sc_module(name), s_in("s_in"), s_out("s_out"),
	  quantizer(quantizer),
	  Y_N1(quantizer.N * quantizer.n_frames),
	  Y_N2(quantizer.N * quantizer.n_frames)
	{
		s_in.register_b_transport(this, &SC_Quantizer_module::b_transport);
	}

	void resize_buffers()
	{
		if ((int)Y_N1.size() != quantizer.N * quantizer.n_frames) Y_N1.resize(quantizer.N * quantizer.n_frames);
		if ((int)Y_N2.size() != quantizer.N * quantizer.n_frames) Y_N2.resize(quantizer.N * quantizer.n_frames);
	}

private:
	void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		assert((trans.get_data_length() / sizeof(R)) == Y_N1.size());

		const R* buffer_in = (R*)trans.get_data_ptr();
		std::copy(buffer_in, buffer_in + Y_N1.size(), Y_N1.begin());

		quantizer.process(Y_N1, Y_N2);

		tlm::tlm_generic_payload payload;
		payload.set_data_ptr((unsigned char*)Y_N2.data());
		payload.set_data_length(Y_N2.size() * sizeof(Q));

		sc_core::sc_time zero_time(sc_core::SC_ZERO_TIME);
		s_out->b_transport(payload, zero_time);
	}
};

template <typename R, typename Q>
class SC_Quantizer : public Quantizer_interface<R,Q>
{
	friend SC_Quantizer_module<R,Q>;

public:
	SC_Quantizer_module<R,Q> *module;

public:
	SC_Quantizer(const int N, const int n_frames = 1, const std::string name = "SC_Quantizer")
	: Quantizer_interface<R,Q>(N, n_frames, name), module(nullptr) {}

	virtual ~SC_Quantizer() { if (module != nullptr) { delete module; module = nullptr; } };

	virtual void process(const mipp::vector<R>& Y_N1, mipp::vector<Q>& Y_N2) = 0;

	virtual void set_n_frames(const int n_frames)
	{
		Quantizer_interface<R,Q>::set_n_frames(n_frames);

		if (module != nullptr)
			module->resize_buffers();
	}

	void create_sc_module()
	{
		this->module = new SC_Quantizer_module<R,Q>(*this, this->name.c_str());
	}
};

template <typename R, typename Q>
using Quantizer = SC_Quantizer<R,Q>;
#else
template <typename R, typename Q>
using Quantizer = Quantizer_interface<R,Q>;
#endif

#endif /* SC_QUANTIZER_HPP_ */
