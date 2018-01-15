#ifndef SIMULATION_BFER_STD_HPP_
#define SIMULATION_BFER_STD_HPP_

#include <chrono>
#include <vector>
#include <random>

#include "Module/Source/Source.hpp"
#include "Module/CRC/CRC.hpp"
#include "Module/Codec/Codec_SIHO.hpp"
#include "Module/Modem/Modem.hpp"
#include "Module/Channel/Channel.hpp"
#include "Module/Quantizer/Quantizer.hpp"
#include "Module/Coset/Coset.hpp"

#include "Factory/Simulation/BFER/BFER_std.hpp"

#include "../BFER.hpp"

namespace aff3ct
{
namespace simulation
{
template <typename B = int, typename R = float, typename Q = R>
class BFER_std : public BFER<B,R,Q>
{
protected:
	const factory::BFER_std::parameters &params_BFER_std;

	// communication chain
	std::vector<module::Source    <B    >*> source;
	std::vector<module::CRC       <B    >*> crc;
	std::vector<module::Codec_SIHO<B,Q  >*> codec;
	std::vector<module::Modem     <B,R,R>*> modem;
	std::vector<module::Channel   <R    >*> channel;
	std::vector<module::Quantizer <R,Q  >*> quantizer;
	std::vector<module::Coset     <B,Q  >*> coset_real;
	std::vector<module::Coset     <B,B  >*> coset_bit;

	// a vector of random generator to generate the seeds
	std::vector<std::mt19937> rd_engine_seed;

public:
	explicit BFER_std(const factory::BFER_std::parameters &params_BFER_std);
	virtual ~BFER_std();

protected:
	virtual void __build_communication_chain(const int tid = 0);
	virtual void _launch();
	virtual void release_objects();

	module::Source    <B    >* build_source    (const int tid = 0);
	module::CRC       <B    >* build_crc       (const int tid = 0);
	module::Codec_SIHO<B,Q  >* build_codec     (const int tid = 0);
	module::Modem     <B,R,R>* build_modem     (const int tid = 0);
	module::Channel   <R    >* build_channel   (const int tid = 0);
	module::Quantizer <R,Q  >* build_quantizer (const int tid = 0);
	module::Coset     <B,Q  >* build_coset_real(const int tid = 0);
	module::Coset     <B,B  >* build_coset_bit (const int tid = 0);
};
}
}

#endif /* SIMULATION_BFER_STD_HPP_ */
