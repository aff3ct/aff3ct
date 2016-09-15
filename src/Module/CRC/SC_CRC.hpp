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

template <typename B>
class SC_CRC;

template <typename B>
class SC_CRC_module : public sc_core::sc_module
{
	SC_HAS_PROCESS(SC_CRC_module);

public:
	tlm_utils::simple_target_socket   <SC_CRC_module> s_in;
	tlm_utils::simple_initiator_socket<SC_CRC_module> s_out;

private:
	SC_CRC<B> &crc;
	mipp::vector<B> U_K;

public:
	SC_CRC_module(SC_CRC<B> &crc, const sc_core::sc_module_name name = "SC_CRC_module")
	: sc_module(name), s_in("s_in"), s_out("s_out"),
	  crc(crc),
	  U_K(crc.K * crc.n_frames)
	{
		s_in.register_b_transport(this, &SC_CRC_module::b_transport);
	}

	void resize_buffers()
	{
		if ((int)U_K.size() != crc.K * crc.n_frames) U_K.resize(crc.K * crc.n_frames);
	}

private:
	void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		assert((trans.get_data_length() / sizeof(B)) == (int)U_K.size());

		const B* buffer_in = (B*)trans.get_data_ptr();
		std::copy(buffer_in, buffer_in + U_K.size(), U_K.begin());

		crc.build(U_K);

		tlm::tlm_generic_payload payload;
		payload.set_data_ptr((unsigned char*)U_K.data());
		payload.set_data_length(U_K.size() * sizeof(B));

		sc_core::sc_time zero_time(sc_core::SC_ZERO_TIME);
		s_out->b_transport(payload, zero_time);
	}
};

template <typename B>
class SC_CRC : public CRC_interface<B>
{
	friend SC_CRC_module<B>;

public:
	SC_CRC_module<B> *module;

public:
	SC_CRC(const int K, const int n_frames = 1, const std::string name = "SC_CRC") 
	: CRC_interface<B>(K, n_frames, name), module(nullptr) {}

	virtual ~SC_CRC() { if (module != nullptr) { delete module; module = nullptr; } }

	virtual void build(mipp::vector<B>& U_K) = 0;

	virtual void set_n_frames(const int n_frames)
	{
		CRC_interface<B>::set_n_frames(n_frames);

		if (module != nullptr)
			module->resize_buffers();
	}

	void create_sc_module()
	{
		this->module = new SC_CRC_module<B>(*this, this->name.c_str());
	}
};

template <typename B>
using CRC = SC_CRC<B>;
#else
template <typename B>
using CRC = CRC_interface<B>;
#endif

#endif /* SC_CRC_HPP_ */
