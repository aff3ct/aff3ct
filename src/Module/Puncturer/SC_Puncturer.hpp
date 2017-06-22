#ifndef SC_PUNCTURER_HPP_
#define SC_PUNCTURER_HPP_

#ifdef SYSTEMC_MODULE
#include <vector>
#include <string>
#include <sstream>
#include <systemc>
#include <tlm>
#include <tlm_utils/simple_target_socket.h>
#include <tlm_utils/simple_initiator_socket.h>

#include "Tools/Exception/exception.hpp"
#include "Tools/Perf/MIPP/mipp.h"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename Q = float>
class SC_Puncturer;

template <typename B, typename Q>
class SC_Puncturer_module_puncturer : public sc_core::sc_module
{
public:
	tlm_utils::simple_target_socket   <SC_Puncturer_module_puncturer> s_in;
	tlm_utils::simple_initiator_socket<SC_Puncturer_module_puncturer> s_out;

private:
	SC_Puncturer<B,Q> &puncturer;
	mipp::vector<B> X_N2;

public:
	SC_Puncturer_module_puncturer(SC_Puncturer<B,Q> &puncturer, 
	                              const sc_core::sc_module_name name = "SC_Puncturer_module_puncturer")
	: sc_module(name), s_in("s_in"), s_out("s_out"),
	  puncturer(puncturer),
	  X_N2(puncturer.get_N() * puncturer.get_n_frames())
	{
		s_in.register_b_transport(this, &SC_Puncturer_module_puncturer::b_transport);
	}

	const mipp::vector<B>& get_X_N()
	{
		return X_N2;
	}

private:
	void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		if (puncturer.get_N_code() * puncturer.get_n_frames() != (int)(trans.get_data_length() / sizeof(B)))
		{
			std::stringstream message;
			message << "'puncturer.get_N_code()' * 'puncturer.get_n_frames()' has to be equal to "
			        << "'trans.get_data_length()' / 'sizeof(B)' ('puncturer.get_N_code()' = " << puncturer.get_N_code()
			        << ", 'puncturer.get_n_frames()' = " << puncturer.get_n_frames()
			        << ", 'trans.get_data_length()' = " << trans.get_data_length()
			        << ", 'sizeof(B)' = " << sizeof(B) << ").";
			throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
		}

		const auto X_N1 = (B*)trans.get_data_ptr();

		puncturer.puncture(X_N1, X_N2.data());

		tlm::tlm_generic_payload payload;
		payload.set_data_ptr((unsigned char*)X_N2.data());
		payload.set_data_length(X_N2.size() * sizeof(B));

		sc_core::sc_time zero_time(sc_core::SC_ZERO_TIME);
		s_out->b_transport(payload, zero_time);
	}
};

template <typename B, typename Q>
class SC_Puncturer_module_depuncturer : public sc_core::sc_module
{
	SC_HAS_PROCESS(SC_Puncturer_module_depuncturer);

public:
	tlm_utils::simple_target_socket   <SC_Puncturer_module_depuncturer> s_in;
	tlm_utils::simple_initiator_socket<SC_Puncturer_module_depuncturer> s_out;

private:
	SC_Puncturer<B,Q> &puncturer;
	mipp::vector<Q> Y_N2;

public:
	SC_Puncturer_module_depuncturer(SC_Puncturer<B,Q> &puncturer,
	                                const sc_core::sc_module_name name = "SC_Puncturer_module_depuncturer")
	: sc_module(name), s_in("s_in"), s_out("s_out"),
	  puncturer(puncturer),
	  Y_N2(puncturer.get_N_code() * puncturer.get_n_frames())
	{
		s_in.register_b_transport(this, &SC_Puncturer_module_depuncturer::b_transport);
	}

	const mipp::vector<Q>& get_Y_N()
	{
		return Y_N2;
	}

private:
	void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		if (puncturer.get_N() * puncturer.get_n_frames() != (int)(trans.get_data_length() / sizeof(Q)))
		{
			std::stringstream message;
			message << "'puncturer.get_N()' * 'puncturer.get_n_frames()' has to be equal to "
			        << "'trans.get_data_length()' / 'sizeof(Q)' ('puncturer.get_N()' = " << puncturer.get_N()
			        << ", 'puncturer.get_n_frames()' = " << puncturer.get_n_frames()
			        << ", 'trans.get_data_length()' = " << trans.get_data_length()
			        << ", 'sizeof(Q)' = " << sizeof(Q) << ").";
			throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
		}

		const auto Y_N1 = (Q*)trans.get_data_ptr();

		puncturer.depuncture(Y_N1, Y_N2.data());

		tlm::tlm_generic_payload payload;
		payload.set_data_ptr((unsigned char*)Y_N2.data());
		payload.set_data_length(Y_N2.size() * sizeof(Q));

		sc_core::sc_time zero_time(sc_core::SC_ZERO_TIME);
		s_out->b_transport(payload, zero_time);
	}
};

template <typename B, typename Q>
class SC_Puncturer : public Puncturer_i<B,Q>
{
public:
	SC_Puncturer_module_puncturer  <B,Q> *sc_module_punct;
	SC_Puncturer_module_depuncturer<B,Q> *sc_module_depunct;

public:
	SC_Puncturer(const int K, const int N, const int N_code, const int n_frames = 1, 
	             const std::string name = "SC_Puncturer")
	: Puncturer_i<B,Q>(K, N, N_code, n_frames, name),
	  sc_module_punct(nullptr), sc_module_depunct(nullptr) {}

	virtual ~SC_Puncturer() 
	{ 
		if (sc_module_punct   != nullptr) { delete sc_module_punct;   sc_module_punct   = nullptr; }
		if (sc_module_depunct != nullptr) { delete sc_module_depunct; sc_module_depunct = nullptr; }
	}

	void create_sc_module_puncturer()
	{
		if (sc_module_punct != nullptr) { delete sc_module_punct; sc_module_punct = nullptr; }
		const std::string new_name = this->name + "_punct";
		this->sc_module_punct = new SC_Puncturer_module_puncturer<B,Q>(*this, new_name.c_str());
	}

	void create_sc_module_depuncturer()
	{
		if (sc_module_depunct != nullptr) { delete sc_module_depunct; sc_module_depunct = nullptr; }
		const std::string new_name = this->name + "_depunct";
		this->sc_module_depunct = new SC_Puncturer_module_depuncturer<B,Q>(*this, new_name.c_str());
	}
};

template <typename B = int, typename Q = float>
using Puncturer = SC_Puncturer<B,Q>;
}
}
#else
#include "SPU_Puncturer.hpp"
#endif

#endif /* SC_PUNCTURER_HPP_ */
