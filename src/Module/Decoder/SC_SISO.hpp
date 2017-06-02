#ifndef SC_SISO_HPP_
#define SC_SISO_HPP_

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
template <typename R>
class SC_SISO;

template <typename R = float>
class SC_SISO_module : public sc_core::sc_module
{
public:
	tlm_utils::simple_target_socket   <SC_SISO_module> s_in;
	tlm_utils::simple_initiator_socket<SC_SISO_module> s_out;

private:
	SC_SISO<R> &siso;
	mipp::vector<R> Y_N2;

public:
	SC_SISO_module(SC_SISO<R> &siso, const sc_core::sc_module_name name = "SC_SISO_module")
	: sc_module(name), s_in ("s_in"), s_out("s_out"),
	  siso(siso),
	  Y_N2(siso.get_N() * siso.get_n_frames())
	{
		s_in.register_b_transport(this, &SC_SISO_module::b_transport);
	}

	const mipp::vector<R>& get_Y_N()
	{
		return Y_N2;
	}

private:
	void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		if (siso.get_N() * siso.get_n_frames() != (int)(trans.get_data_length() / sizeof(R)))
			throw std::length_error("aff3ct::module::SISO: TLM input data size is invalid.");

		const auto Y_N1 = (R*)trans.get_data_ptr();

		siso.soft_decode(Y_N1, Y_N2.data());

		tlm::tlm_generic_payload payload;
		payload.set_data_ptr((unsigned char*)Y_N2.data());
		payload.set_data_length(Y_N2.size() * sizeof(R));

		sc_core::sc_time zero_time(sc_core::SC_ZERO_TIME);
		s_out->b_transport(payload, zero_time);
	}
};

template <typename R>
class SC_SISO : public SISO_i<R>
{
public:
	SC_SISO_module<R> *sc_module_siso;

public:
	SC_SISO(const int K, const int N, const int n_frames, const int simd_inter_frame_level = 1,
	        const std::string name = "SC_SISO")
	: SISO_i<R>(K, N, n_frames, simd_inter_frame_level, name), sc_module_siso(nullptr) {}

	virtual ~SC_SISO()
	{
		if (sc_module_siso != nullptr) { delete sc_module_siso; sc_module_siso = nullptr; }
	}

	void create_sc_module_siso()
	{
		if (sc_module_siso != nullptr) { delete sc_module_siso; sc_module_siso = nullptr; }
		this->sc_module_siso = new SC_SISO_module<R>(*this, this->name.c_str());
	}
};

template <typename R>
using SISO = SC_SISO<R>;
}
}
#else
#include "SPU_SISO.hpp"
#endif

#endif /* SC_SISO_HPP_ */
