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

#include "Tools/Perf/MIPP/mipp.h"

template <typename T>
class SC_Interleaver;

template <typename T>
class SC_Interleaver_module_interleaver : public sc_core::sc_module
{
	SC_HAS_PROCESS(SC_Interleaver_module_interleaver);

public:
	tlm_utils::simple_target_socket   <SC_Interleaver_module_interleaver> s_in;
	tlm_utils::simple_initiator_socket<SC_Interleaver_module_interleaver> s_out;

private:
	SC_Interleaver<T> &interleaver;
	mipp::vector<char     > natural_vec_1, interleaved_vec_1;
	mipp::vector<short    > natural_vec_2, interleaved_vec_2;
	mipp::vector<int      > natural_vec_4, interleaved_vec_4;
	mipp::vector<long long> natural_vec_8, interleaved_vec_8;

public:
	SC_Interleaver_module_interleaver(SC_Interleaver<T> &interleaver,
	                                  const sc_core::sc_module_name name = "SC_Interleaver_module_interleaver")
	: sc_module(name), s_in("s_in"), s_out("s_out"),
	  interleaver(interleaver),
	  natural_vec_1    (interleaver.pi.size() * interleaver.n_frames),
	  interleaved_vec_1(interleaver.pi.size() * interleaver.n_frames),
	  natural_vec_2    (interleaver.pi.size() * interleaver.n_frames),
	  interleaved_vec_2(interleaver.pi.size() * interleaver.n_frames),
	  natural_vec_4    (interleaver.pi.size() * interleaver.n_frames),
	  interleaved_vec_4(interleaver.pi.size() * interleaver.n_frames),
	  natural_vec_8    (interleaver.pi.size() * interleaver.n_frames),
	  interleaved_vec_8(interleaver.pi.size() * interleaver.n_frames)
	{
		s_in.register_b_transport(this, &SC_Interleaver_module_interleaver::b_transport);
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
	void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		int size_of_data = trans.get_data_length() / (interleaver.pi.size() * interleaver.n_frames);

		switch (size_of_data)
		{
			case 1: _b_transport<char     >(trans, t, natural_vec_1, interleaved_vec_1); break;
			case 2: _b_transport<short    >(trans, t, natural_vec_2, interleaved_vec_2); break;
			case 4: _b_transport<int      >(trans, t, natural_vec_4, interleaved_vec_4); break;
			case 8: _b_transport<long long>(trans, t, natural_vec_8, interleaved_vec_8); break;
			default:
				std::cerr << "(EE) Unrecognized type of data, exiting." << std::endl; std::exit(-1);
				break;
		}
	}

	template <typename D>
	void _b_transport(tlm::tlm_generic_payload& trans, 
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
		s_out->b_transport(payload, zero_time);
	}
};

template <typename T>
class SC_Interleaver_module_deinterleaver : public sc_core::sc_module
{
	SC_HAS_PROCESS(SC_Interleaver_module_deinterleaver);

public:
	tlm_utils::simple_target_socket   <SC_Interleaver_module_deinterleaver> s_in;
	tlm_utils::simple_initiator_socket<SC_Interleaver_module_deinterleaver> s_out;

private:
	SC_Interleaver<T> &interleaver;
	mipp::vector<char     > natural_vec_1, interleaved_vec_1;
	mipp::vector<short    > natural_vec_2, interleaved_vec_2;
	mipp::vector<int      > natural_vec_4, interleaved_vec_4;
	mipp::vector<long long> natural_vec_8, interleaved_vec_8;

public:
	SC_Interleaver_module_deinterleaver(SC_Interleaver<T> &interleaver, 
	                                   const sc_core::sc_module_name name = "SC_Interleaver_module_deinterleaver")
	: sc_module(name), s_in("s_in"), s_out("s_out"),
	  interleaver(interleaver),
	  natural_vec_1    (interleaver.pi.size() * interleaver.n_frames),
	  interleaved_vec_1(interleaver.pi.size() * interleaver.n_frames),
	  natural_vec_2    (interleaver.pi.size() * interleaver.n_frames),
	  interleaved_vec_2(interleaver.pi.size() * interleaver.n_frames),
	  natural_vec_4    (interleaver.pi.size() * interleaver.n_frames),
	  interleaved_vec_4(interleaver.pi.size() * interleaver.n_frames),
	  natural_vec_8    (interleaver.pi.size() * interleaver.n_frames),
	  interleaved_vec_8(interleaver.pi.size() * interleaver.n_frames)
	{
		s_in.register_b_transport(this, &SC_Interleaver_module_deinterleaver::b_transport);
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
	void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		int size_of_data = trans.get_data_length() / (interleaver.pi.size() * interleaver.n_frames);
		switch (size_of_data)
		{
			case 1: _b_transport<char     >(trans, t, interleaved_vec_1, natural_vec_1); break;
			case 2: _b_transport<short    >(trans, t, interleaved_vec_2, natural_vec_2); break;
			case 4: _b_transport<int      >(trans, t, interleaved_vec_4, natural_vec_4); break;
			case 8: _b_transport<long long>(trans, t, interleaved_vec_8, natural_vec_8); break;
			default:
				std::cerr << "(EE) Unrecognized type of data, exiting." << std::endl; std::exit(-1);
				break;
		}
	}

	template <typename D>
	void _b_transport(tlm::tlm_generic_payload& trans, 
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
		s_out->b_transport(payload, zero_time);
	}
};

template <typename T>
class SC_Interleaver : public Interleaver_interface<T>
{
	friend SC_Interleaver_module_interleaver  <T>;
	friend SC_Interleaver_module_deinterleaver<T>;

public:
	SC_Interleaver_module_interleaver  <T> *module_inter;
	SC_Interleaver_module_deinterleaver<T> *module_deinter;

public:
	SC_Interleaver(const int size, const int n_frames = 1, const std::string name = "SC_Interleaver")
	: Interleaver_interface<T>(size, n_frames, name), module_inter(nullptr), module_deinter(nullptr) {}

	virtual ~SC_Interleaver() 
	{
		if (module_inter   != nullptr) { delete module_inter;   module_inter   = nullptr; }
		if (module_deinter != nullptr) { delete module_deinter; module_deinter = nullptr; }
	}

	virtual void set_n_frames(const int n_frames)
	{
		Interleaver_interface<T>::set_n_frames(n_frames);

		if (module_inter   != nullptr) module_inter  ->resize_buffers();
		if (module_deinter != nullptr) module_deinter->resize_buffers();
	}

	void create_sc_module_interleaver()
	{
		const std::string new_name = this->name + "_inter";
		this->module_inter = new SC_Interleaver_module_interleaver<T>(*this, new_name.c_str());
	}

	void create_sc_module_deinterleaver()
	{
		const std::string new_name = this->name + "_deinter";
		this->module_deinter = new SC_Interleaver_module_deinterleaver<T>(*this, new_name.c_str());
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
