#ifdef STARPU

#ifndef SPU_SIMULATION_BFER_HPP_
#define SPU_SIMULATION_BFER_HPP_

#include <condition_variable>
#include <mutex>
#include <chrono>
#include <vector>

#include <starpu.h>

#include "Tools/Perf/MIPP/mipp.h"
#include "Tools/params.h"
#include "Tools/Display/Terminal/Terminal.hpp"

#include "Module/Monitor/Standard/Monitor_reduction.hpp"

#include "../Simulation_BFER.hpp"

namespace aff3ct
{
namespace simulation
{
template <typename B, typename R, typename Q>
class Simulation_BFER : public Simulation_BFER_i<B,R,Q>
{
private:
	std::mutex mutex_terminal;
	std::condition_variable cond_terminal;
	std::vector<std::vector<std::string>> task_names;
	unsigned long long frame_id;

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
	module::Monitor_reduction<B,R> *monitor_red;
	// terminal (for the output of the code)
	tools::Terminal *terminal;

public:
	Simulation_BFER(const tools::parameters& params);
	virtual ~Simulation_BFER();

protected:
	        void _launch        ();
	virtual void release_objects();

private:
	void Monte_Carlo_method();

	static void build_communication_chain(Simulation_BFER<B,R,Q> *simu, const int tid = 0);
	static void allocate_data            (Simulation_BFER<B,R,Q> *simu, const int tid = 0);
	inline void seq_tasks_submission     (                              const int tid = 0);

	tools::Terminal* build_terminal();

	static void terminal_temp_report(Simulation_BFER<B,R,Q> *simu);
};
}
}

#endif /* SPU_SIMULATION_BFER_HPP_ */

#endif /* STARPU */
