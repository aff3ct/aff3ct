#ifndef SIMULATION_BFER_STD_THREADS_HPP_
#define SIMULATION_BFER_STD_THREADS_HPP_

#include <vector>
#include <thread>
#include <mutex>
#include <mipp.h>

#include "../BFER_std.hpp"

namespace aff3ct
{
namespace simulation
{
template <typename B = int, typename R = float, typename Q = R>
class BFER_std_threads : public BFER_std<B,R,Q>
{
protected:
	std::map<std::thread::id, int> thread_id;

	std::mutex mutex_debug;

	// data vector
	std::vector<mipp::vector<B>> U_K1; // information bit vector
	std::vector<mipp::vector<B>> U_K2; // information bit vector + CRC bits
	std::vector<mipp::vector<B>> X_N1; // encoded codeword
	std::vector<mipp::vector<B>> X_N2; // encoded and punctured codeword
	std::vector<mipp::vector<R>> X_N3; // modulate codeword
	std::vector<mipp::vector<R>> H_N;  // code gain for Rayleigh channels
	std::vector<mipp::vector<R>> Y_N1; // noisy codeword (after the channel noise)
	std::vector<mipp::vector<R>> Y_N2; // noisy codeword (after the filtering)
	std::vector<mipp::vector<R>> Y_N3; // noisy codeword (after quantization)
	std::vector<mipp::vector<Q>> Y_N4; // noisy codeword (after the demodulation)
	std::vector<mipp::vector<Q>> Y_N5; // noisy and depunctured codeword
	std::vector<mipp::vector<B>> V_N1; // decoded bits (full codeword)
	std::vector<mipp::vector<B>> V_K1; // decoded bits + CRC bits
	std::vector<mipp::vector<B>> V_K2; // decoded bits

public:
	BFER_std_threads(const factory::BFER_std::parameters &params, tools::Codec<B,Q> &codec);
	virtual ~BFER_std_threads();

protected:
	virtual void _build_communication_chain(const int tid = 0);
	virtual void _launch();
	virtual tools::Terminal_BFER<B>* build_terminal();

private:
	void Monte_Carlo_method   (const int tid = 0);
	void simulation_loop      (const int tid = 0);
	void simulation_loop_bench(const int tid = 0);

	void display_debug();

	static void start_thread(BFER_std_threads<B,R,Q> *simu, const int tid = 0);
};
}
}

#endif /* SIMULATION_BFER_STD_THREADS_HPP_ */
