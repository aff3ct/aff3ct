#ifndef SC_MONITOR_HPP_
#define SC_MONITOR_HPP_

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
template <typename B, typename R>
class SC_Monitor;

template <typename B, typename R>
class SC_Monitor_module : public sc_core::sc_module
{
	SC_HAS_PROCESS(SC_Monitor_module);

public:
	tlm_utils::simple_target_socket<SC_Monitor_module> s_in1;
	tlm_utils::simple_target_socket<SC_Monitor_module> s_in2;

private:
	SC_Monitor<B,R> &monitor;
	mipp::vector<B> V_K;
	mipp::vector<B> U_K;

public:
	SC_Monitor_module(SC_Monitor<B,R> &monitor,
	                  const sc_core::sc_module_name name = "SC_Monitor_module")
	: sc_module(name), s_in1("s_in1"), s_in2("s_in2"),
	  monitor(monitor),
	  V_K(monitor.K * monitor.n_frames),
	  U_K(monitor.K * monitor.n_frames)
	{
		s_in1.register_b_transport(this, &SC_Monitor_module::b_transport_source);
		s_in2.register_b_transport(this, &SC_Monitor_module::b_transport_decoder);
	}

	void resize_buffers()
	{
		if ((int)U_K.size() != monitor.K * monitor.n_frames) U_K.resize(monitor.K * monitor.n_frames);
		if ((int)V_K.size() != monitor.K * monitor.n_frames) V_K.resize(monitor.K * monitor.n_frames);
	}

private:
	void b_transport_source(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		assert((trans.get_data_length() / sizeof(B)) == U_K.size());

		const B* buffer_in = (B*)trans.get_data_ptr();
		std::copy(buffer_in, buffer_in + U_K.size(), U_K.begin());
	}

	void b_transport_decoder(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		assert((trans.get_data_length() / sizeof(B)) == V_K.size());

		const B* buffer_in = (B*)trans.get_data_ptr();
		std::copy(buffer_in, buffer_in + V_K.size(), V_K.begin());

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
	SC_Monitor(const int K, const int N, const int n_frames = 1, const std::string name = "SC_Monitor")
	: Monitor_i<B,R>(K, N, n_frames, name), module(nullptr) {}

	virtual ~SC_Monitor() {if (module != nullptr) { delete module; module = nullptr; }};

	virtual void set_n_frames(const int n_frames)
	{
		Monitor_i<B,R>::set_n_frames(n_frames);

		if (module != nullptr)
			module->resize_buffers();
	}

	void create_sc_module()
	{
		this->module = new SC_Monitor_module<B,R>(*this, this->name.c_str());
	}
};

template <typename B, typename R>
using Monitor = SC_Monitor<B,R>;
}
}
#else
#include "SPU_Monitor.hpp"
#endif

#endif /* SC_MONITOR_HPP_ */
