#ifndef SC_INTERLEAVER_HPP_
#define SC_INTERLEAVER_HPP_

#ifdef SYSTEMC_MODULE
#include <vector>
#include <string>
#include <stdexcept>
#include <systemc>
#include <tlm>
#include <tlm_utils/simple_target_socket.h>
#include <tlm_utils/simple_initiator_socket.h>

#include "Tools/Perf/MIPP/mipp.h"

namespace aff3ct
{
namespace module
{
template <typename T>
class SC_Interleaver;

template <typename T = int>
class SC_Interleaver_module_interleaver : public sc_core::sc_module
{
	SC_HAS_PROCESS(SC_Interleaver_module_interleaver);

public:
	tlm_utils::simple_target_socket   <SC_Interleaver_module_interleaver> s_in;
	tlm_utils::simple_initiator_socket<SC_Interleaver_module_interleaver> s_out;

private:
	SC_Interleaver<T> &interleaver;
	mipp::vector<char     > vec_1;
	mipp::vector<short    > vec_2;
	mipp::vector<int      > vec_4;
	mipp::vector<long long> vec_8;

public:
	SC_Interleaver_module_interleaver(SC_Interleaver<T> &interleaver,
	                                  const sc_core::sc_module_name name = "SC_Interleaver_module_interleaver")
	: sc_module(name), s_in("s_in"), s_out("s_out"),
	  interleaver(interleaver),
	  vec_1(interleaver.get_size() * interleaver.get_n_frames()),
	  vec_2(interleaver.get_size() * interleaver.get_n_frames()),
	  vec_4(interleaver.get_size() * interleaver.get_n_frames()),
	  vec_8(interleaver.get_size() * interleaver.get_n_frames())
	{
		s_in.register_b_transport(this, &SC_Interleaver_module_interleaver::b_transport);
	}

private:
	void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		int size_of_data = trans.get_data_length() / (interleaver.get_size() * interleaver.get_n_frames());

		switch (size_of_data)
		{
			case 1: _b_transport<char     >(trans, t, vec_1); break;
			case 2: _b_transport<short    >(trans, t, vec_2); break;
			case 4: _b_transport<int      >(trans, t, vec_4); break;
			case 8: _b_transport<long long>(trans, t, vec_8); break;
			default:
				throw std::runtime_error("aff3ct::module::Interleaver: TLM unrecognized type of data.");
				break;
		}
	}

	template <typename D>
	void _b_transport(tlm::tlm_generic_payload& trans, 
	                  sc_core::sc_time& t, 
	                  mipp::vector<D> &interleaved_vec)
	{
		if (interleaver.get_size() * interleaver.get_n_frames() != (trans.get_data_length() / sizeof(D)))
			throw std::length_error("aff3ct::module::Interleaver: TLM input data size is invalid.");

		const auto natural_vec = (D*)trans.get_data_ptr();

		interleaver.interleave(natural_vec, interleaved_vec.data());

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
	mipp::vector<char     > vec_1;
	mipp::vector<short    > vec_2;
	mipp::vector<int      > vec_4;
	mipp::vector<long long> vec_8;

public:
	SC_Interleaver_module_deinterleaver(SC_Interleaver<T> &interleaver, 
	                                    const sc_core::sc_module_name name = "SC_Interleaver_module_deinterleaver")
	: sc_module(name), s_in("s_in"), s_out("s_out"),
	  interleaver(interleaver),
	  vec_1(interleaver.get_size() * interleaver.get_n_frames()),
	  vec_2(interleaver.get_size() * interleaver.get_n_frames()),
	  vec_4(interleaver.get_size() * interleaver.get_n_frames()),
	  vec_8(interleaver.get_size() * interleaver.get_n_frames())
	{
		s_in.register_b_transport(this, &SC_Interleaver_module_deinterleaver::b_transport);
	}

private:
	void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		int size_of_data = trans.get_data_length() / (interleaver.get_size() * interleaver.get_n_frames());
		switch (size_of_data)
		{
			case 1: _b_transport<char     >(trans, t, vec_1); break;
			case 2: _b_transport<short    >(trans, t, vec_2); break;
			case 4: _b_transport<int      >(trans, t, vec_4); break;
			case 8: _b_transport<long long>(trans, t, vec_8); break;
			default:
				throw std::runtime_error("aff3ct::module::Interleaver: TLM unrecognized type of data.");
				break;
		}
	}

	template <typename D>
	void _b_transport(tlm::tlm_generic_payload& trans, 
	                  sc_core::sc_time& t, 
	                  mipp::vector<D> &natural_vec)
	{
		if (interleaver.get_size() * interleaver.get_n_frames() != (trans.get_data_length() / sizeof(D)))
			throw std::length_error("aff3ct::module::Interleaver: TLM input data size is invalid.");

		const auto interleaved_vec = (D*)trans.get_data_ptr();

		interleaver.deinterleave(interleaved_vec, natural_vec.data());

		tlm::tlm_generic_payload payload;
		payload.set_data_ptr((unsigned char*)natural_vec.data());
		payload.set_data_length(natural_vec.size() * sizeof(D));

		sc_core::sc_time zero_time(sc_core::SC_ZERO_TIME);
		s_out->b_transport(payload, zero_time);
	}
};

template <typename T>
class SC_Interleaver : public Interleaver_i<T>
{
public:
	SC_Interleaver_module_interleaver  <T> *sc_module_inter;
	SC_Interleaver_module_deinterleaver<T> *sc_module_deinter;

public:
	SC_Interleaver(const int size, const int n_frames = 1, const std::string name = "SC_Interleaver")
	: Interleaver_i<T>(size, n_frames, name), sc_module_inter(nullptr), sc_module_deinter(nullptr) {}

	virtual ~SC_Interleaver() 
	{
		if (sc_module_inter   != nullptr) { delete sc_module_inter;   sc_module_inter   = nullptr; }
		if (sc_module_deinter != nullptr) { delete sc_module_deinter; sc_module_deinter = nullptr; }
	}

	void create_sc_module_interleaver()
	{
		const std::string new_name = this->name + "_inter";
		this->sc_module_inter = new SC_Interleaver_module_interleaver<T>(*this, new_name.c_str());
	}

	void create_sc_module_deinterleaver()
	{
		const std::string new_name = this->name + "_deinter";
		this->sc_module_deinter = new SC_Interleaver_module_deinterleaver<T>(*this, new_name.c_str());
	}
};

template <typename T = int>
using Interleaver = SC_Interleaver<T>;
}
}
#else
#include "SPU_Interleaver.hpp"
#endif

#endif /* SC_INTERLEAVER_HPP_ */
