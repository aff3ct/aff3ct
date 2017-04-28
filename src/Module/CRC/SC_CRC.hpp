#ifndef SC_CRC_HPP_
#define SC_CRC_HPP_

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
class SC_CRC;

template <typename B = int>
class SC_CRC_module : public sc_core::sc_module
{
	SC_HAS_PROCESS(SC_CRC_module);

public:
	tlm_utils::simple_target_socket   <SC_CRC_module> s_in;
	tlm_utils::simple_initiator_socket<SC_CRC_module> s_out;

private:
	SC_CRC<B> &crc;

public:
	SC_CRC_module(SC_CRC<B> &crc, const sc_core::sc_module_name name = "SC_CRC_module")
	: sc_module(name), s_in("s_in"), s_out("s_out"),
	  crc(crc)
	{
		s_in.register_b_transport(this, &SC_CRC_module::b_transport);
	}

private:
	void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		if (crc.K * crc.n_frames != (int)(trans.get_data_length() / sizeof(B)))
			throw std::length_error("aff3ct::module::CRC: TLM input data size is invalid.");

		const auto U_K = (B*)trans.get_data_ptr();

		crc.build(U_K);

		tlm::tlm_generic_payload payload;
		payload.set_data_ptr((unsigned char*)U_K);
		payload.set_data_length(trans.get_data_length());

		sc_core::sc_time zero_time(sc_core::SC_ZERO_TIME);
		s_out->b_transport(payload, zero_time);
	}
};

template <typename B>
class SC_CRC : public CRC_i<B>
{
	friend SC_CRC_module<B>;

public:
	SC_CRC_module<B> *module;

public:
	SC_CRC(const int K, const int n_frames = 1, const std::string name = "SC_CRC") 
	: CRC_i<B>(K, n_frames, name), module(nullptr) {}

	virtual ~SC_CRC() { if (module != nullptr) { delete module; module = nullptr; } }

	void create_sc_module()
	{
		this->module = new SC_CRC_module<B>(*this, this->name.c_str());
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
