#ifndef SIMULATION_BFER_ITE_THREADS_HPP_
#define SIMULATION_BFER_ITE_THREADS_HPP_

#include <map>
#include <thread>
#include <chrono>
#include <vector>
#include <utility>
#include <mipp.h>

#include "../Simulation_BFER_ite.hpp"
#include "Tools/Factory/Simulation/BFER/Factory_simulation_BFER_ite.hpp"

namespace aff3ct
{
namespace simulation
{
template <typename B = int, typename R = float, typename Q = R>
class Simulation_BFER_ite_threads : public Simulation_BFER_ite<B,R,Q>
{
protected:
	// data vector
	std::vector<mipp::vector<B>> U_K1; // information bit vector
	std::vector<mipp::vector<B>> U_K2; // information bit vector + CRC bits
	std::vector<mipp::vector<B>> X_N1; // encoded codeword
	std::vector<mipp::vector<B>> X_N2; // interleaved codeword
	std::vector<mipp::vector<R>> X_N3; // modulate codeword
	std::vector<mipp::vector<R>> H_N;  // code gain for Rayleigh channels
	std::vector<mipp::vector<R>> Y_N1; // noisy codeword (after the channel noise)
	std::vector<mipp::vector<R>> Y_N2; // noisy codeword (after the filtering)
	std::vector<mipp::vector<Q>> Y_N3; // noisy codeword (after the quatization)
	std::vector<mipp::vector<Q>> Y_N4; // noisy codeword (after the demodulation)
	std::vector<mipp::vector<Q>> Y_N5; // noisy codeword (after the deinterleaving process)
	std::vector<mipp::vector<Q>> Y_N6; // noisy codeword (after the decoding process)
	std::vector<mipp::vector<Q>> Y_N7; // noisy codeword (after the interleaving process)
	std::vector<mipp::vector<B>> V_K1; // decoded bits + CRC bits
	std::vector<mipp::vector<B>> V_K2; // decoded bits

public:
	Simulation_BFER_ite_threads(const tools::Factory_simulation_BFER_ite::parameters &params, tools::Codec_SISO<B,Q> &codec);
	virtual ~Simulation_BFER_ite_threads();

protected:
	virtual void _build_communication_chain(const int tid = 0);
	virtual void _launch();
	virtual tools::Terminal_BFER<B>* build_terminal();

private:
	void Monte_Carlo_method   (const int tid = 0);
	void simulation_loop      (const int tid = 0);
	void simulation_loop_debug(                 );

	static void start_thread(Simulation_BFER_ite_threads<B,R,Q> *simu, const int tid = 0);
};
}
}

#endif /* SIMULATION_BFER_ITE_THREADS_HPP_ */
