#ifndef SIMULATION_BFER_ITE_HPP_
#define SIMULATION_BFER_ITE_HPP_

#include <chrono>
#include <vector>
#include <random>

#include "Module/Source/Source.hpp"
#include "Module/CRC/CRC.hpp"
#include "Module/Encoder/Encoder.hpp"
#include "Module/Modem/Modem.hpp"
#include "Module/Channel/Channel.hpp"
#include "Module/Quantizer/Quantizer.hpp"
#include "Module/Coset/Coset.hpp"
#include "Module/Decoder/SISO.hpp"
#include "Module/Decoder/Decoder.hpp"
#include "Module/Interleaver/Interleaver.hpp"

#include "Tools/Codec/Codec_SISO.hpp"
#include "Tools/Factory/Simulation/BFER/Factory_simulation_BFER_ite.hpp"

#include "../Simulation_BFER.hpp"

namespace aff3ct
{
namespace simulation
{
template <typename B = int, typename R = float, typename Q = R>
class Simulation_BFER_ite : public Simulation_BFER<B,R,Q>
{
protected:
	const typename tools::Factory_simulation_BFER_ite::chain_parameters_BFER_ite<B,R,Q> &chain_params;
	const typename tools::Factory_simulation_BFER_ite::simu_parameters_BFER_ite         &simu_params;
	tools::Codec_SISO<B,Q> &codec_siso;

	// communication chain
	std::vector<module::Source     <B    >*> source;
	std::vector<module::CRC        <B    >*> crc;
	std::vector<module::Encoder    <B    >*> encoder;
	std::vector<module::Modem      <B,R,Q>*> modem;
	std::vector<module::Channel    <R    >*> channel;
	std::vector<module::Quantizer  <R,Q  >*> quantizer;
	std::vector<module::Interleaver<int  >*> interleaver;
	std::vector<module::Coset      <B,Q  >*> coset_real;
	std::vector<module::SISO       <Q    >*> siso;
	std::vector<module::Decoder    <B,Q  >*> decoder;
	std::vector<module::Coset      <B,B  >*> coset_bit;

	// a vector of random generator to generate the seeds
	std::vector<std::mt19937> rd_engine_seed;

public:
	Simulation_BFER_ite(const typename tools::Factory_simulation_BFER_ite::chain_parameters_BFER_ite<B,R,Q> &chain_params, tools::Codec_SISO<B,Q> &codec);
	virtual ~Simulation_BFER_ite();

protected:
	virtual void _build_communication_chain(const int tid = 0);
	virtual void release_objects();

	virtual module::Source     <B    >* build_source     (const int tid = 0, const int seed = 0);
	virtual module::CRC        <B    >* build_crc        (const int tid = 0                    );
	virtual module::Encoder    <B    >* build_encoder    (const int tid = 0, const int seed = 0);
	virtual module::Interleaver<int  >* build_interleaver(const int tid = 0, const int seed = 0, const int rd_seed = 0);
	virtual module::Modem      <B,R,Q>* build_modem      (const int tid = 0                    );
	virtual module::Channel    <R    >* build_channel    (const int tid = 0, const int seed = 0);
	virtual module::Quantizer  <R,Q  >* build_quantizer  (const int tid = 0                    );
	virtual module::SISO       <Q    >* build_siso       (const int tid = 0                    );
	virtual module::Coset      <B,Q  >* build_coset_real (const int tid = 0                    );
	virtual module::Decoder    <B,Q  >* build_decoder    (const int tid = 0                    );
	virtual module::Coset      <B,B  >* build_coset_bit  (const int tid = 0                    );
};
}
}

#endif /* SIMULATION_BFER_ITE_HPP_ */
