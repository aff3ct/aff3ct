#ifndef SC_ERROR_ANALYZER_HPP_
#define SC_ERROR_ANALYZER_HPP_

#ifdef SYSTEMC
#include <vector>
#include <string>
#include <cassert>
#include <systemc>
#include <tlm>
#include <tlm_utils/simple_target_socket.h>
#include <tlm_utils/simple_initiator_socket.h>

#include "../Tools/MIPP/mipp.h"

template <typename B>
class SC_Error_analyzer;

template <typename B>
class SC_Error_analyzer_sockets : public sc_core::sc_module
{
	SC_HAS_PROCESS(SC_Error_analyzer_sockets);

public:
	tlm_utils::simple_target_socket<SC_Error_analyzer_sockets> in_source;
	tlm_utils::simple_target_socket<SC_Error_analyzer_sockets> in_decoder;

private:
	SC_Error_analyzer<B> &analyzer;
	mipp::vector<B> V_K;
	mipp::vector<B> U_K;

public:
	SC_Error_analyzer_sockets(SC_Error_analyzer<B> &analyzer, const sc_core::sc_module_name name = "SC_Error_analyzer_sockets")
	: sc_module(name), in_source("in_source"), in_decoder("in_decoder"),
	  analyzer(analyzer),
	  V_K(analyzer.K * analyzer.n_frames),
	  U_K(analyzer.K * analyzer.n_frames)
	{
		in_source .register_b_transport(this, &SC_Error_analyzer_sockets::b_transport_source);
		in_decoder.register_b_transport(this, &SC_Error_analyzer_sockets::b_transport_decoder);
	}

	void resize_buffers()
	{
		if ((int)U_K.size() != analyzer.K * analyzer.n_frames) U_K.resize(analyzer.K * analyzer.n_frames);
		if ((int)V_K.size() != analyzer.K * analyzer.n_frames) V_K.resize(analyzer.K * analyzer.n_frames);
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

		analyzer.check_errors(U_K, V_K);

		if (analyzer.fe_limit_achieved())
			sc_core::sc_stop();
	}
};

template <typename B>
class SC_Error_analyzer : public Error_analyzer_interface<B>
{
	friend SC_Error_analyzer_sockets<B>;

private:
	std::string name;

public:
	SC_Error_analyzer_sockets<B> *sockets;

public:
	SC_Error_analyzer(const int K, const int N, const int n_frames = 1, const std::string name = "SC_Error_analyzer")
	: Error_analyzer_interface<B>(K, N, n_frames, name), name(name), sockets(nullptr) {}

	virtual ~SC_Error_analyzer() {if (sockets != nullptr) { delete sockets; sockets = nullptr; }};

	virtual void check_errors(const mipp::vector<B>& U, const mipp::vector<B>& V) = 0;

	virtual int get_n_be() const = 0;
	virtual int get_n_fe() const = 0;

	virtual float get_ber_value() const = 0;
	virtual float get_fer_value() const = 0;

	virtual unsigned long long get_n_analyzed_frames() const = 0;

	virtual int  get_fe_limit     () const = 0;
	virtual bool fe_limit_achieved() const = 0;

	virtual void set_n_frames(const int n_frames)
	{
		Error_analyzer_interface<B>::set_n_frames(n_frames);

		if (sockets != nullptr)
			sockets->resize_buffers();
	}

	void create_sc_sockets()
	{
		this->sockets = new SC_Error_analyzer_sockets<B>(*this, name.c_str());
	}
};

template <typename B>
using Error_analyzer = SC_Error_analyzer<B>;
#else
template <typename B>
using Error_analyzer = Error_analyzer_interface<B>;
#endif

#endif /* SC_ERROR_ANALYZER_HPP_ */
