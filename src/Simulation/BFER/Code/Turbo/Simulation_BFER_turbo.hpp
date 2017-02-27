#ifndef SIMULATION_TURBO_HPP_
#define SIMULATION_TURBO_HPP_

#include <fstream>

#include "Tools/Code/Turbo/Scaling_factor/Scaling_factor.hpp"

#include "Module/Interleaver/Interleaver.hpp"
#include "Module/Encoder/RSC/Encoder_RSC_sys.hpp"
#include "Module/Decoder/SISO.hpp"

#include "Simulation/BFER/Standard/STD_Simulation_BFER.hpp"

namespace aff3ct
{
namespace simulation
{
template <typename B = int, typename R = float, typename Q = R, typename QD = Q>
class Simulation_BFER_turbo : public Simulation_BFER<B,R,Q>
{
protected:
	// the trellis representation
	std::vector<std::vector<int>>            trellis;
	std::vector<module::Interleaver<short>*> interleaver;
	std::vector<module::Encoder_RSC_sys<B>*> sub_encoder;
	std::vector<module::SISO<Q>*>            siso;
	std::vector<tools::Scaling_factor<Q>*>   sf;
	std::ofstream                            json_stream;

public:
	Simulation_BFER_turbo(const tools::parameters& params);
	virtual ~Simulation_BFER_turbo();

protected:
	void                  release_objects  ();
	void                  launch_precompute();
	void                  snr_precompute   ();
	module::Encoder<B>*   build_encoder    (const int tid = 0);
	module::Decoder<B,Q>* build_decoder    (const int tid = 0);
};
}
}

#endif /* SIMULATION_TURBO_HPP_ */
