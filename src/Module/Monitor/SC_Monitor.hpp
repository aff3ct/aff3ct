#ifndef SC_MONITOR_HPP_
#define SC_MONITOR_HPP_

#ifdef SYSTEMC
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
template <typename B, typename R>
class SC_Monitor;

template <typename B = int, typename R = float>
class SC_Monitor_module : public sc_core::sc_module
{
	SC_HAS_PROCESS(SC_Monitor_module);

public:
	tlm_utils::simple_target_socket<SC_Monitor_module> s_in1;
	tlm_utils::simple_target_socket<SC_Monitor_module> s_in2;

private:
	SC_Monitor<B,R> &monitor;
	B *U_K;

public:
	SC_Monitor_module(SC_Monitor<B,R> &monitor,
	                  const sc_core::sc_module_name name = "SC_Monitor_module")
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
		if (monitor.K * monitor.n_frames != (int)(trans.get_data_length() / sizeof(B)))
			throw std::length_error("aff3ct::module::Monitor: TLM input data size is invalid.");

		U_K = (B*)trans.get_data_ptr();
	}

	void b_transport_decoder(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		if (U_K == nullptr)
			throw std::runtime_error("aff3ct::module::Monitor: TLM \"U_K\" pointer can't be NULL.");

		if (monitor.K * monitor.n_frames != (int)(trans.get_data_length() / sizeof(B)))
			throw std::length_error("aff3ct::module::Monitor: TLM input data size is invalid.");

		const auto V_K = (B*)trans.get_data_ptr();

		monitor.check_errors(U_K, V_K);

		if (monitor.fe_limit_achieved())
			sc_core::sc_stop();
	}
};

template <typename B, typename R>
class SC_Monitor : public Monitor_i<B,R>
{
	friend SC_Monitor_module<B,R>;

public:
	SC_Monitor_module<B,R> *module;

public:
	SC_Monitor(const int K, const int N, const int N_mod, const int n_frames = 1, const std::string name = "SC_Monitor")
	: Monitor_i<B,R>(K, N, N_mod, n_frames, name), module(nullptr) {}

	virtual ~SC_Monitor() {if (module != nullptr) { delete module; module = nullptr; }};

	void create_sc_module()
	{
		this->module = new SC_Monitor_module<B,R>(*this, this->name.c_str());
	}
};

template <typename B = int, typename R = float>
using Monitor = SC_Monitor<B,R>;
}
}
#else
#include "SPU_Monitor.hpp"
#endif

#endif /* SC_MONITOR_HPP_ */
