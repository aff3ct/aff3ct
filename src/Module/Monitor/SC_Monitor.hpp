#ifndef SC_MONITOR_HPP_
#define SC_MONITOR_HPP_

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
class SC_Monitor;

template <typename B = int>
class SC_Monitor_module : public sc_core::sc_module
{
public:
	tlm_utils::simple_target_socket<SC_Monitor_module> s_in1;
	tlm_utils::simple_target_socket<SC_Monitor_module> s_in2;

private:
	SC_Monitor<B> &monitor;
	B *U_K;

public:
	SC_Monitor_module(SC_Monitor<B> &monitor, const sc_core::sc_module_name name = "SC_Monitor_module")
	: sc_module(name), s_in1("s_in1"), s_in2("s_in2"),
	  monitor(monitor),
	  U_K(nullptr)
	{
		s_in1.register_b_transport(this, &SC_Monitor_module::b_transport_source);
		s_in2.register_b_transport(this, &SC_Monitor_module::b_transport_decoder);
	}

private:
	void b_transport_source(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		if (monitor.get_size() * monitor.get_n_frames() != (int)(trans.get_data_length() / sizeof(B)))
			throw std::length_error("aff3ct::module::Monitor: TLM input data size is invalid.");

		U_K = (B*)trans.get_data_ptr();
	}

	void b_transport_decoder(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		if (U_K == nullptr)
			throw std::runtime_error("aff3ct::module::Monitor: TLM \"U_K\" pointer can't be NULL.");

		if (monitor.get_size() * monitor.get_n_frames() != (int)(trans.get_data_length() / sizeof(B)))
			throw std::length_error("aff3ct::module::Monitor: TLM input data size is invalid.");

		const auto V_K = (B*)trans.get_data_ptr();

		monitor.check_errors(U_K, V_K);

		if (monitor.fe_limit_achieved())
			sc_core::sc_stop();
	}
};

template <typename B>
class SC_Monitor : public Monitor_i<B>
{
public:
	SC_Monitor_module<B> *sc_module;

public:
	SC_Monitor(const int size, const int n_frames = 1, const std::string name = "SC_Monitor")
	: Monitor_i<B>(size, n_frames, name), sc_module(nullptr) {}

	virtual ~SC_Monitor()
	{
		if (sc_module != nullptr) { delete sc_module; sc_module = nullptr; }
	};

	void create_sc_module()
	{
		if (sc_module != nullptr) { delete sc_module; sc_module = nullptr; }
		this->sc_module = new SC_Monitor_module<B>(*this, this->name.c_str());
	}
};

template <typename B = int>
using Monitor = SC_Monitor<B>;
}
}
#else
#include "SPU_Monitor.hpp"
#endif

#endif /* SC_MONITOR_HPP_ */
