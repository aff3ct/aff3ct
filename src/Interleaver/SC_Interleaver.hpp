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

#include "../Tools/MIPP/mipp.h"

template <typename T>
class SC_Interleaver;

template <typename T>
class SC_Interleaver_module : public sc_core::sc_module
{
	SC_HAS_PROCESS(SC_Interleaver_module);

public:
	tlm_utils::simple_target_socket   <SC_Interleaver_module> s_in_interleave;
	tlm_utils::simple_target_socket   <SC_Interleaver_module> s_in_deinterleave;
	tlm_utils::simple_initiator_socket<SC_Interleaver_module> s_out_interleave;
	tlm_utils::simple_initiator_socket<SC_Interleaver_module> s_out_deinterleave;

private:
	SC_Interleaver<T> &interleaver;
	mipp::vector<char     > natural_vec_1, interleaved_vec_1;
	mipp::vector<short    > natural_vec_2, interleaved_vec_2;
	mipp::vector<int      > natural_vec_4, interleaved_vec_4;
	mipp::vector<long long> natural_vec_8, interleaved_vec_8;

public:
	SC_Interleaver_module(SC_Interleaver<T> &interleaver, const sc_core::sc_module_name name = "SC_Interleaver_module")
	: sc_module(name), 
	  s_in_interleave ("s_in_interleave"),  s_in_deinterleave ("s_in_deinterleave"), 
	  s_out_interleave("s_out_interleave"), s_out_deinterleave("s_out_deinterleave"),
	  interleaver(interleaver),
	  natural_vec_1    (interleaver.size * interleaver.n_frames),
	  interleaved_vec_1(interleaver.size * interleaver.n_frames),
	  natural_vec_2    (interleaver.size * interleaver.n_frames),
	  interleaved_vec_2(interleaver.size * interleaver.n_frames),
	  natural_vec_4    (interleaver.size * interleaver.n_frames),
	  interleaved_vec_4(interleaver.size * interleaver.n_frames),
	  natural_vec_8    (interleaver.size * interleaver.n_frames),
	  interleaved_vec_8(interleaver.size * interleaver.n_frames)
	{
		s_in_interleave  .register_b_transport(this, &SC_Interleaver_module::b_transport_interleave);
		s_in_deinterleave.register_b_transport(this, &SC_Interleaver_module::b_transport_deinterleave);
	}

	void resize_buffers()
	{
		const int size = interleaver.pi.size();
		if ((int)natural_vec_1    .size() != size * interleaver.n_frames) natural_vec_1    .resize(size * interleaver.n_frames);
		if ((int)natural_vec_2    .size() != size * interleaver.n_frames) natural_vec_2    .resize(size * interleaver.n_frames);
		if ((int)natural_vec_4    .size() != size * interleaver.n_frames) natural_vec_4    .resize(size * interleaver.n_frames);
		if ((int)natural_vec_8    .size() != size * interleaver.n_frames) natural_vec_8    .resize(size * interleaver.n_frames);
		if ((int)interleaved_vec_1.size() != size * interleaver.n_frames) interleaved_vec_1.resize(size * interleaver.n_frames);
		if ((int)interleaved_vec_2.size() != size * interleaver.n_frames) interleaved_vec_2.resize(size * interleaver.n_frames);
		if ((int)interleaved_vec_4.size() != size * interleaver.n_frames) interleaved_vec_4.resize(size * interleaver.n_frames);
		if ((int)interleaved_vec_8.size() != size * interleaver.n_frames) interleaved_vec_8.resize(size * interleaver.n_frames);
	}

private:
	void b_transport_interleave(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		assert(trans.get_data_length() % (this->pi.size() * this->n_frames) == 0);

		int size_of_data = trans.get_data_length() / (this->pi.size() * this->n_frames);
		switch (size_of_data)
		{
			case 1: _b_transport_interleave<char     >(trans, t, natural_vec_1, interleaved_vec_1); break;
			case 2: _b_transport_interleave<short    >(trans, t, natural_vec_2, interleaved_vec_2); break;
			case 4: _b_transport_interleave<int      >(trans, t, natural_vec_4, interleaved_vec_4); break;
			case 8: _b_transport_interleave<long long>(trans, t, natural_vec_8, interleaved_vec_8); break;
			default:
				std::cerr << "(EE) Unrecognized type of data, exiting." << std::endl; std::exit(-1);
				break;
		}
	}

	template <typename D>
	void _b_transport_interleave(tlm::tlm_generic_payload& trans, 
	                             sc_core::sc_time& t, 
	                             mipp::vector<D> &natural_vec, 
	                             mipp::vector<D> &interleaved_vec)
	{
		assert((trans.get_data_length() / sizeof(D)) == natural_vec.size());

		const D* buffer_in = (D*)trans.get_data_ptr();
		std::copy(buffer_in, buffer_in + natural_vec.size(), natural_vec.begin());

		interleaver.interleave(natural_vec, interleaved_vec);

		tlm::tlm_generic_payload payload;
		payload.set_data_ptr((unsigned char*)interleaved_vec.data());
		payload.set_data_length(interleaved_vec.size() * sizeof(D));

		sc_core::sc_time zero_time(sc_core::SC_ZERO_TIME);
		s_out_interleave->b_transport(payload, zero_time);
	}

	void b_transport_deinterleave(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		assert(trans.get_data_length() % (this->pi.size() * this->n_frames) == 0);

		int size_of_data = trans.get_data_length() / (this->pi.size() * this->n_frames);
		switch (size_of_data)
		{
			case 1: _b_transport_deinterleave<char     >(trans, t, interleaved_vec_1, natural_vec_1); break;
			case 2: _b_transport_deinterleave<short    >(trans, t, interleaved_vec_2, natural_vec_2); break;
			case 4: _b_transport_deinterleave<int      >(trans, t, interleaved_vec_4, natural_vec_4); break;
			case 8: _b_transport_deinterleave<long long>(trans, t, interleaved_vec_8, natural_vec_8); break;
			default:
				std::cerr << "(EE) Unrecognized type of data, exiting." << std::endl; std::exit(-1);
				break;
		}
	}

	template <typename D>
	void _b_transport_deinterleave(tlm::tlm_generic_payload& trans, 
	                               sc_core::sc_time& t, 
	                               mipp::vector<D> &interleaved_vec, 
	                               mipp::vector<D> &natural_vec)
	{
		assert((trans.get_data_length() / sizeof(D)) == interleaved_vec.size());

		const D* buffer_in = (D*)trans.get_data_ptr();
		std::copy(buffer_in, buffer_in + interleaved_vec.size(), interleaved_vec.begin());

		interleaver.deinterleave(interleaved_vec, natural_vec);

		tlm::tlm_generic_payload payload;
		payload.set_data_ptr((unsigned char*)natural_vec.data());
		payload.set_data_length(natural_vec.size() * sizeof(D));

		sc_core::sc_time zero_time(sc_core::SC_ZERO_TIME);
		s_out_deinterleave->b_transport(payload, zero_time);
	}
};

template <typename T>
class SC_Interleaver : public Interleaver_interface<T>
{
	friend SC_Interleaver_module<T>;

private:
	std::string name;

public:
	SC_Interleaver_module<T> *module;

public:
	SC_Interleaver(const int size, const int n_frames = 1, const std::string name = "SC_Interleaver")
	: Interleaver_interface<T>(size, n_frames, name), name(name), module(nullptr) {}

	virtual ~SC_Interleaver() { if (module != nullptr) { delete module; module = nullptr; } }

	virtual void set_n_frames(const int n_frames)
	{
		Interleaver_interface<T>::set_n_frames(n_frames);

		if (module != nullptr)
			module->resize_buffers();
	}

	void create_sc_module()
	{
		this->module = new SC_Interleaver_module<T>(*this, name.c_str());
	}

protected:
	virtual void gen_lookup_tables() = 0;
};

template <typename T>
using Interleaver = SC_Interleaver<T>;
#else
template <typename T>
using Interleaver = Interleaver_interface<T>;
#endif

#endif /* SC_INTERLEAVER_HPP_ */