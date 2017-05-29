#ifndef SIMULATION_BFER_STD_HPP_
#define SIMULATION_BFER_STD_HPP_

#include <random>
#include <thread>
#include <chrono>
#include <vector>
#include <utility>

#include "Tools/Perf/MIPP/mipp.h"

#include "Module/Source/Source.hpp"
#include "Module/CRC/CRC.hpp"
#include "Module/Encoder/Encoder.hpp"
#include "Module/Puncturer/Puncturer.hpp"
#include "Module/Modulator/Modulator.hpp"
#include "Module/Channel/Channel.hpp"
#include "Module/Quantizer/Quantizer.hpp"
#include "Module/Coset/Coset.hpp"
#include "Module/Decoder/Decoder.hpp"
#include "Module/Interleaver/Interleaver.hpp"

#include "../Simulation_BFER.hpp"

namespace aff3ct
{
namespace simulation
{
template <typename B = int, typename R = float, typename Q = R>
class Simulation_BFER_std : public Simulation_BFER<B,R,Q>
{
protected:
	// communication chain
	std::vector<module::Source     <B    >*> source;
	std::vector<module::CRC        <B    >*> crc;
	std::vector<module::Encoder    <B    >*> encoder;
	std::vector<module::Puncturer  <B,Q  >*> puncturer;
	std::vector<module::Modulator  <B,R,R>*> modulator;
	std::vector<module::Channel    <R    >*> channel;
	std::vector<module::Quantizer  <R,Q  >*> quantizer;
	std::vector<module::Coset      <B,Q  >*> coset_real;
	std::vector<module::Decoder    <B,Q  >*> decoder;
	std::vector<module::Coset      <B,B  >*> coset_bit;
	std::vector<module::Interleaver<int  >*> interleaver;

	// a vector of random generator to generate the seeds
	std::vector<std::mt19937> rd_engine_seed;

public:
	Simulation_BFER_std(const tools::parameters& params, tools::Codec<B,Q> &codec);
	virtual ~Simulation_BFER_std();

protected:
	virtual void build_communication_chain(const int tid = 0);
	virtual void release_objects();

	virtual module::Source     <B    >* build_source     (const int tid = 0, const int seed = 0);
	virtual module::CRC        <B    >* build_crc        (const int tid = 0                    );
	virtual module::Encoder    <B    >* build_encoder    (const int tid = 0, const int seed = 0);
	virtual module::Interleaver<int  >* build_interleaver(const int tid = 0, const int seed = 0);
	virtual module::Puncturer  <B,Q  >* build_puncturer  (const int tid = 0                    );
	virtual module::Modulator  <B,R,R>* build_modulator  (const int tid = 0                    );
	virtual module::Channel    <R    >* build_channel    (const int tid = 0, const int seed = 0);
	virtual module::Quantizer  <R,Q  >* build_quantizer  (const int tid = 0                    );
	virtual module::Coset      <B,Q  >* build_coset_real (const int tid = 0                    );
	virtual module::Decoder    <B,Q  >* build_decoder    (const int tid = 0                    );
	virtual module::Coset      <B,B  >* build_coset_bit  (const int tid = 0                    );
};
}
}

#endif /* SIMULATION_BFER_STD_HPP_ */
