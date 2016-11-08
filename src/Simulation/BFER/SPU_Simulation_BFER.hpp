#ifdef STARPU

#ifndef SPU_SIMULATION_BFER_HPP_
#define SPU_SIMULATION_BFER_HPP_

#include <thread>
#include <chrono>
#include <vector>

#include <starpu.h>

#include "Tools/Perf/MIPP/mipp.h"
#include "Tools/params.h"
#include "Tools/Display/Terminal/Terminal.hpp"

#include "Module/Monitor/Standard/Monitor_reduction.hpp"

#include "Simulation_BFER_i.hpp"

template <typename B, typename R, typename Q>
class Simulation_BFER : public Simulation_BFER_i<B,R,Q>
{
protected:
	// data vector
	std::vector<mipp::vector<B>> U_K;  // information bit vector
	std::vector<mipp::vector<B>> X_N1; // encoded codeword
	std::vector<mipp::vector<B>> X_N2; // encoded and punctured codeword
	std::vector<mipp::vector<R>> X_N3; // modulate codeword
	std::vector<mipp::vector<R>> H_N;  // code gain for Rayleigh channels
	std::vector<mipp::vector<R>> Y_N1; // noisy codeword (after the channel noise)
	std::vector<mipp::vector<R>> Y_N2; // noisy codeword (after the filtering)
	std::vector<mipp::vector<R>> Y_N3; // noisy codeword (after the demodulation)
	std::vector<mipp::vector<Q>> Y_N4; // noisy codeword (after quantization)
	std::vector<mipp::vector<Q>> Y_N5; // noisy and depunctured codeword
	std::vector<mipp::vector<B>> V_K;  // decoded codeword 

	// starpu vector handlers
	std::vector<starpu_data_handle_t> spu_U_K;
	std::vector<starpu_data_handle_t> spu_X_N1;
	std::vector<starpu_data_handle_t> spu_X_N2;
	std::vector<starpu_data_handle_t> spu_X_N3;
	std::vector<starpu_data_handle_t> spu_H_N;
	std::vector<starpu_data_handle_t> spu_Y_N1;
	std::vector<starpu_data_handle_t> spu_Y_N2;
	std::vector<starpu_data_handle_t> spu_Y_N3;
	std::vector<starpu_data_handle_t> spu_Y_N4;
	std::vector<starpu_data_handle_t> spu_Y_N5;
	std::vector<starpu_data_handle_t> spu_V_K;

	// objects
	Monitor_reduction<B> *monitor_red;
	Terminal             *terminal;

	// time points and durations
	std::chrono::nanoseconds d_decod_total_fake;

public:
	Simulation_BFER(const parameters& params);
	virtual ~Simulation_BFER();

protected:
	        void _launch         ();
	virtual void release_objects ();

private:
	void build_communication_chain(const int tid = 0);
	void allocate_data            (const int tid = 0);
	void Monte_Carlo_method       (const int tid = 0);
	void simulation_loop          (const int tid = 0);
	void spu_create_tasks         (const int tid = 0);

	Terminal* build_terminal(const int tid = 0);
};

#endif /* SPU_SIMULATION_BFER_HPP_ */

#endif /* STARPU */
