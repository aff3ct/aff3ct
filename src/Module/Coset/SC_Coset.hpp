#ifndef SC_COSET_HPP_
#define SC_COSET_HPP_

#ifdef SYSTEMC
#include <vector>
#include <string>
#include <cassert>
#include <systemc>
#include <tlm>
#include <tlm_utils/simple_target_socket.h>
#include <tlm_utils/simple_initiator_socket.h>

#include "Tools/Perf/MIPP/mipp.h"

template <typename B, typename D>
class SC_Coset;

template <typename B, typename D>
class SC_Coset_module : public sc_core::sc_module
{
	SC_HAS_PROCESS(SC_Coset_module);

public:
	tlm_utils::simple_target_socket   <SC_Coset_module> s_in1;
	tlm_utils::simple_target_socket   <SC_Coset_module> s_in2;
	tlm_utils::simple_initiator_socket<SC_Coset_module> s_out;

private:
	SC_Coset<B,D> &coset;
	mipp::vector<B> ref;
	mipp::vector<D> in_data;
	mipp::vector<D> out_data;

public:
	SC_Coset_module(SC_Coset<B,D> &coset, const sc_core::sc_module_name name = "SC_Coset_module")
	: sc_module(name), s_in1("s_in1"), s_in2("s_in2"), s_out("s_out"),
	  coset   (coset),
	  ref     (coset.size * coset.n_frames),
	  in_data (coset.size * coset.n_frames),
	  out_data(coset.size * coset.n_frames)
	{
		s_in1.register_b_transport(this, &SC_Coset_module::b_transport_ref);
		s_in2.register_b_transport(this, &SC_Coset_module::b_transport_data);
	}

	void resize_buffers()
	{
		if ((int)ref     .size() != coset.size * coset.n_frames) ref     .resize(coset.size * coset.n_frames);
		if ((int)in_data .size() != coset.size * coset.n_frames) in_data .resize(coset.size * coset.n_frames);
		if ((int)out_data.size() != coset.size * coset.n_frames) out_data.resize(coset.size * coset.n_frames);
	}

private:
	void b_transport_ref(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		assert((trans.get_data_length() / sizeof(B)) == ref.size());

		const B* buffer_in = (B*)trans.get_data_ptr();
		std::copy(buffer_in, buffer_in + ref.size(), ref.begin());
	}

	void b_transport_data(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		assert((trans.get_data_length() / sizeof(D)) == in_data.size());

		const D* buffer_in = (B*)trans.get_data_ptr();
		std::copy(buffer_in, buffer_in + in_data.size(), in_data.begin());

		coset.apply(ref, in_data, out_data);

		tlm::tlm_generic_payload payload;
		payload.set_data_ptr((unsigned char*)out_data.data());
		payload.set_data_length(out_data.size() * sizeof(D));

		sc_core::sc_time zero_time(sc_core::SC_ZERO_TIME);
		s_out->b_transport(payload, zero_time);
	}
};

template <typename B, typename D>
class SC_Coset : public Coset_interface<B,D>
{
	friend SC_Coset_module<B,D>;

public:
	SC_Coset_module<B,D> *module;

public:
	SC_Coset(const int size, const int n_frames = 1, const std::string name = "SC_Coset")
	: Coset_interface<B,D>(size, n_frames, name), module(nullptr) {}

	virtual ~SC_Coset()
	{ 
		if (module != nullptr) { delete module; module = nullptr; }
	}

	virtual void set_n_frames(const int n_frames)
	{
		Coset_interface<B,D>::set_n_frames(n_frames);
		if (module != nullptr) module->resize_buffers();
	}

	void create_sc_module()
	{
		const std::string new_name = this->name + "_punct";
		this->module = new SC_Coset_module<B,D>(*this, new_name.c_str());
	}
};

template <typename B, typename D>
using Coset = SC_Coset<B,D>;
#else
template <typename B, typename D>
using Coset = Coset_interface<B,D>;
#endif

#endif /* SC_COSET_HPP_ */
