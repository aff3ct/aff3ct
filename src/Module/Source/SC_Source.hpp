#ifndef SC_SOURCE_HPP_
#define SC_SOURCE_HPP_

#ifdef SYSTEMC_MODULE
#include <vector>
#include <string>
#include <systemc>
#include <tlm>
#include <tlm_utils/simple_initiator_socket.h>
#include <mipp.h>

namespace aff3ct
{
namespace module
{
template <typename B>
class SC_Source;

template <typename B = int>
class SC_Source_module : public sc_core::sc_module
{
	SC_HAS_PROCESS(SC_Source_module); // required because of the "SC_THREAD" call in the constructor

public:
	tlm_utils::simple_initiator_socket<SC_Source_module> s_out;

private:
	SC_Source<B> &source;
	mipp::vector<B> U_K;

public:
	SC_Source_module(SC_Source<B> &source, const sc_core::sc_module_name name = "SC_Source_module")
	: sc_module(name), s_out("s_out"),
	  source(source),
	  U_K(source.get_K() * source.get_n_frames())
	{
		SC_THREAD(sc_generate);
	}

	const mipp::vector<B>& get_U_K()
	{
		return U_K;
	}

private:
	void sc_generate()
	{
		while (1)
		{
			source.generate(U_K);

			tlm::tlm_generic_payload payload;
			payload.set_data_ptr((unsigned char*)U_K.data());
			payload.set_data_length(U_K.size() * sizeof(B));

			sc_core::sc_time zero_time(sc_core::SC_ZERO_TIME);
			s_out->b_transport(payload, zero_time);

			// required to give the hand to the SystemC scheduler (yield)
			sc_core::wait(0, sc_core::SC_MS);
		}
	}
};

template <typename B>
class SC_Source : public Source_i<B>
{
public:
	SC_Source_module<B> *sc_module;

public:
	SC_Source(const int K, const int n_frames = 1, const std::string name = "Source_SC")
	: Source_i<B>(K, n_frames, name), sc_module(nullptr) {}

	virtual ~SC_Source()
	{
		if (sc_module != nullptr) { delete sc_module; sc_module = nullptr; }
	};

	void create_sc_module()
	{
		if (sc_module != nullptr) { delete sc_module; sc_module = nullptr; }
		this->sc_module = new SC_Source_module<B>(*this, this->name.c_str());
	}
};

template <typename B = int>
using Source = SC_Source<B>;
}
}
#else
#include "SPU_Source.hpp"
#endif

#endif /* SC_SOURCE_HPP_ */
