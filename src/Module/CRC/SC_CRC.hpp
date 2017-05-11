#ifndef SC_CRC_HPP_
#define SC_CRC_HPP_

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
template <typename B>
class SC_CRC;

template <typename B = int>
class SC_CRC_module_build : public sc_core::sc_module
{
public:
	tlm_utils::simple_target_socket   <SC_CRC_module_build> s_in;
	tlm_utils::simple_initiator_socket<SC_CRC_module_build> s_out;

private:
	SC_CRC<B> &crc;
	mipp::vector<B> U_K2;

public:
	SC_CRC_module_build(SC_CRC<B> &crc, const sc_core::sc_module_name name = "SC_CRC_module_build")
	: sc_module(name), s_in("s_in"), s_out("s_out"),
	  crc(crc),
	  U_K2((crc.get_K() + crc.get_size()) * crc.get_n_frames())
	{
		s_in.register_b_transport(this, &SC_CRC_module_build::b_transport);
	}

private:
	void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		if (crc.get_K() * crc.get_n_frames() != (int)(trans.get_data_length() / sizeof(B)))
			throw std::length_error("aff3ct::module::CRC: TLM input data size is invalid.");

		const auto U_K1 = (B*)trans.get_data_ptr();

		crc.build(U_K1, U_K2.data());

		tlm::tlm_generic_payload payload;
		payload.set_data_ptr((unsigned char*)U_K2.data());
		payload.set_data_length(U_K2.size() * sizeof(B));

		sc_core::sc_time zero_time(sc_core::SC_ZERO_TIME);
		s_out->b_transport(payload, zero_time);
	}
};

template <typename B = int>
class SC_CRC_module_extract : public sc_core::sc_module
{
public:
	tlm_utils::simple_target_socket   <SC_CRC_module_extract> s_in;
	tlm_utils::simple_initiator_socket<SC_CRC_module_extract> s_out;

private:
	SC_CRC<B> &crc;
	mipp::vector<B> V_K2;

public:
	SC_CRC_module_extract(SC_CRC<B> &crc, const sc_core::sc_module_name name = "SC_CRC_module_extract")
	: sc_module(name), s_in("s_in"), s_out("s_out"),
	  crc(crc),
	  V_K2(crc.get_K() * crc.get_n_frames())
	{
		s_in.register_b_transport(this, &SC_CRC_module_extract::b_transport);
	}

private:
	void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		if ((crc.get_K() + crc.get_size()) * crc.get_n_frames() != (int)(trans.get_data_length() / sizeof(B)))
			throw std::length_error("aff3ct::module::CRC: TLM input data size is invalid.");

		const auto V_K1 = (B*)trans.get_data_ptr();

		crc.extract(V_K1, V_K2.data());

		tlm::tlm_generic_payload payload;
		payload.set_data_ptr((unsigned char*)V_K2.data());
		payload.set_data_length(V_K2.size() * sizeof(B));

		sc_core::sc_time zero_time(sc_core::SC_ZERO_TIME);
		s_out->b_transport(payload, zero_time);
	}
};

template <typename B>
class SC_CRC : public CRC_i<B>
{
public:
	SC_CRC_module_build  <B> *sc_module_build;
	SC_CRC_module_extract<B> *sc_module_extract;

public:
	SC_CRC(const int K, const int n_frames = 1, const std::string name = "SC_CRC") 
	: CRC_i<B>(K, n_frames, name), sc_module_build(nullptr), sc_module_extract(nullptr) {}

	virtual ~SC_CRC()
	{
		if (sc_module_build   != nullptr) { delete sc_module_build;   sc_module_build   = nullptr; }
		if (sc_module_extract != nullptr) { delete sc_module_extract; sc_module_extract = nullptr; }
	}

	void create_sc_module_build()
	{
		const std::string new_name = this->name + "_build";
		this->sc_module_build = new SC_CRC_module_build<B>(*this, new_name.c_str());
	}

	void create_sc_module_extract()
	{
		const std::string new_name = this->name + "_extract";
		this->sc_module_extract = new SC_CRC_module_extract<B>(*this, new_name.c_str());
	}
};

template <typename B = int>
using CRC = SC_CRC<B>;
}
}
#else
#include "SPU_CRC.hpp"
#endif

#endif /* SC_CRC_HPP_ */
