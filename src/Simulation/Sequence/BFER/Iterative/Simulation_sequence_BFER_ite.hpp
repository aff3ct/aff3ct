#ifndef SIMULATION_SEQUENCE_BFER_ITE_HPP_
#define SIMULATION_SEQUENCE_BFER_ITE_HPP_

#include <memory>

#include "Module/Source/Source.hpp"
#include "Module/CRC/CRC.hpp"
#include "Module/Modem/Modem.hpp"
#include "Module/Channel/Channel.hpp"
#include "Module/Quantizer/Quantizer.hpp"
#include "Module/Coset/Coset.hpp"
#include "Module/Interleaver/Interleaver.hpp"
#include "Module/Loop/Predicate/Loop_predicate.hpp"
#include "Module/Loop/CRC/Loop_CRC.hpp"
#include "Tools/Interleaver/Interleaver_core.hpp"
#include "Tools/Codec/Codec_SISO.hpp"
#include "Tools/Constellation/Constellation.hpp"
#include "Tools/Math/Distribution/Distributions.hpp"
#include "Factory/Simulation/BFER/BFER_ite.hpp"
#include "Simulation/Sequence/BFER/Simulation_sequence_BFER.hpp"

namespace aff3ct
{
namespace simulation
{

template <typename B = int, typename R = float, typename Q = R>
class Simulation_sequence_BFER_ite : public Simulation_sequence_BFER<B,R>
{
protected:
	const factory::BFER_ite &params_BFER_ite;

	// communication sequence
	std::unique_ptr<module::Source          <B    >> source;
	std::unique_ptr<module::CRC             <B    >> crc;
	std::unique_ptr<tools ::Codec_SISO      <B,  Q>> codec1;
	std::unique_ptr<tools ::Codec_SISO      <B,  Q>> codec2;
	std::unique_ptr<module::Modem           <B,R,Q>> modem1;
	std::unique_ptr<module::Modem           <B,R,Q>> modem2;
	std::unique_ptr<module::Channel         <R    >> channel;
	std::unique_ptr<module::Quantizer       <R,Q  >> quantizer;
	std::unique_ptr<module::Coset           <B,Q  >> coset_real1;
	std::unique_ptr<module::Coset           <B,Q  >> coset_real2;
	std::unique_ptr<module::Coset           <B,Q  >> coset_real3;
	std::unique_ptr<module::Coset           <B,B  >> coset_bit;
	std::unique_ptr<tools ::Interleaver_core<     >> interleaver_core;
	std::unique_ptr<module::Interleaver     <B    >> interleaver_bit;
	std::unique_ptr<module::Interleaver     <Q    >> interleaver_llr1;
	std::unique_ptr<module::Interleaver     <Q    >> interleaver_llr2;
	std::shared_ptr<module::Loop_predicate  <Q    >> loop_ite;
	std::shared_ptr<module::Loop_CRC        <B,Q  >> loop_crc;

public:
	explicit Simulation_sequence_BFER_ite(const factory::BFER_ite &params_BFER_ite);
	virtual ~Simulation_sequence_BFER_ite() = default;

protected:
	std::unique_ptr<module::Source          <B    >> build_source     (                                            );
	std::unique_ptr<module::CRC             <B    >> build_crc        (                                            );
	std::unique_ptr<tools ::Codec_SISO      <B,Q  >> build_codec      (const module::CRC         <B> *crc          );
	std::unique_ptr<module::Modem           <B,R,Q>> build_modem      (const tools::Distributions<R> *distributions,
	                                                                   const tools::Constellation<R> *constellation);
	std::unique_ptr<module::Channel         <R    >> build_channel    (const tools::Distributions<R> *distributions);
	std::unique_ptr<module::Quantizer       <R,Q  >> build_quantizer  (                                            );
	std::unique_ptr<module::Coset           <B,Q  >> build_coset_real (                                            );
	std::unique_ptr<module::Coset           <B,B  >> build_coset_bit  (                                            );
	std::unique_ptr<tools ::Interleaver_core<     >> build_interleaver(                                            );
	std::unique_ptr<module::Loop_predicate  <Q    >> build_loop_ite   (                                            );
	std::unique_ptr<module::Loop_CRC        <B,Q  >> build_loop_crc   (                                            );

	virtual void create_modules();
	virtual void bind_sockets();
	virtual void create_sequence();
};

}
}

#endif /* SIMULATION_SEQUENCE_BFER_ITE_HPP_ */
