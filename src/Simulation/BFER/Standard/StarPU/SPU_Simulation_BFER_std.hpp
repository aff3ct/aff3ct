#ifdef STARPU

#ifndef SPU_SIMULATION_BFER_STD_HPP_
#define SPU_SIMULATION_BFER_STD_HPP_

#include <vector>

#include <starpu.h>

#include "Tools/Perf/MIPP/mipp.h"

#include "../Simulation_BFER_std.hpp"

namespace aff3ct
{
namespace simulation
{
template <typename B = int, typename R = float, typename Q = R>
class SPU_Simulation_BFER_std : public Simulation_BFER_std<B,R,Q>
{
private:
	std::condition_variable cond_terminal;
	std::vector<std::vector<std::string>> task_names;
	unsigned long long frame_id;

protected:
	// data vector
	std::vector<mipp::vector<B>> U_K1; // information bit vector
	std::vector<mipp::vector<B>> U_K2; // information bit vector + CRC bits
	std::vector<mipp::vector<B>> X_N1; // encoded codeword
	std::vector<mipp::vector<B>> X_N2; // encoded and punctured codeword
	std::vector<mipp::vector<R>> X_N3; // modulate codeword
	std::vector<mipp::vector<R>> H_N;  // code gain for Rayleigh channels
	std::vector<mipp::vector<R>> Y_N1; // noisy codeword (after the channel noise)
	std::vector<mipp::vector<R>> Y_N2; // noisy codeword (after the filtering)
	std::vector<mipp::vector<R>> Y_N3; // noisy codeword (after the demodulation)
	std::vector<mipp::vector<Q>> Y_N4; // noisy codeword (after quantization)
	std::vector<mipp::vector<Q>> Y_N5; // noisy and depunctured codeword
	std::vector<mipp::vector<B>> V_K1; // decoded bits + CRC bits
	std::vector<mipp::vector<B>> V_K2; // decoded bits

	// starpu vector handlers
	std::vector<starpu_data_handle_t> spu_U_K1;
	std::vector<starpu_data_handle_t> spu_U_K2;
	std::vector<starpu_data_handle_t> spu_X_N1;
	std::vector<starpu_data_handle_t> spu_X_N2;
	std::vector<starpu_data_handle_t> spu_X_N3;
	std::vector<starpu_data_handle_t> spu_H_N;
	std::vector<starpu_data_handle_t> spu_Y_N1;
	std::vector<starpu_data_handle_t> spu_Y_N2;
	std::vector<starpu_data_handle_t> spu_Y_N3;
	std::vector<starpu_data_handle_t> spu_Y_N4;
	std::vector<starpu_data_handle_t> spu_Y_N5;
	std::vector<starpu_data_handle_t> spu_V_K1;
	std::vector<starpu_data_handle_t> spu_V_K2;

public:
	SPU_Simulation_BFER_std(const tools::parameters& params, tools::Codec<B,Q> &codec);
	virtual ~SPU_Simulation_BFER_std();

protected:
	virtual void build_communication_chain(const int tid = 0);
	virtual void _launch();

private:
	inline void seq_tasks_submission(const int tid = 0);
};
}
}

#endif /* SPU_SIMULATION_BFER_STD_HPP_ */

#endif /* STARPU */
