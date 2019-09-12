#ifndef SIMULATION_BFER_ITE_HPP_
#define SIMULATION_BFER_ITE_HPP_

#include <vector>
#include <random>
#include <memory>

#include "Tools/Interleaver/Interleaver_core.hpp"
#include "Module/Source/Source.hpp"
#include "Module/CRC/CRC.hpp"
#include "Module/Codec/Codec_SISO_SIHO.hpp"
#include "Module/Modem/Modem.hpp"
#include "Module/Channel/Channel.hpp"
#include "Module/Quantizer/Quantizer.hpp"
#include "Module/Coset/Coset.hpp"
#include "Module/Interleaver/Interleaver.hpp"
#include "Factory/Simulation/BFER/BFER_ite.hpp"
#include "Simulation/BFER/BFER.hpp"

namespace aff3ct
{
namespace simulation
{
template <typename B = int, typename R = float, typename Q = R>
class BFER_ite : public BFER<B,R,Q>
{
protected:
	const factory::BFER_ite::parameters &params_BFER_ite;

	// communication chain
	std::vector<std::unique_ptr<module::Source          <B    >>> source;
	std::vector<std::unique_ptr<module::CRC             <B    >>> crc;
	std::vector<std::unique_ptr<module::Codec_SISO_SIHO <B,  Q>>> codec;
	std::vector<std::unique_ptr<module::Modem           <B,R,Q>>> modem;
	std::vector<std::unique_ptr<module::Channel         <R    >>> channel;
	std::vector<std::unique_ptr<module::Quantizer       <R,Q  >>> quantizer;
	std::vector<std::unique_ptr<module::Coset           <B,Q  >>> coset_real;
	std::vector<std::unique_ptr<module::Coset           <B,B  >>> coset_bit;
	std::vector<std::unique_ptr<tools ::Interleaver_core<     >>> interleaver_core;
	std::vector<std::unique_ptr<module::Interleaver     <B    >>> interleaver_bit;
	std::vector<std::unique_ptr<module::Interleaver     <Q    >>> interleaver_llr;

	// a vector of random generator to generate the seeds
	std::vector<std::mt19937> rd_engine_seed;

public:
	explicit BFER_ite(const factory::BFER_ite::parameters &params_BFER_ite);
	virtual ~BFER_ite() = default;

protected:
	virtual void __build_communication_chain(const int tid = 0);
	virtual void _launch();

	virtual std::unique_ptr<module::Source          <B    >> build_source     (const int tid = 0);
	virtual std::unique_ptr<module::CRC             <B    >> build_crc        (const int tid = 0);
	virtual std::unique_ptr<module::Codec_SISO_SIHO <B,Q  >> build_codec      (const int tid = 0);
	virtual std::unique_ptr<module::Modem           <B,R,Q>> build_modem      (const int tid = 0);
	virtual std::unique_ptr<module::Channel         <R    >> build_channel    (const int tid = 0);
	virtual std::unique_ptr<module::Quantizer       <R,Q  >> build_quantizer  (const int tid = 0);
	virtual std::unique_ptr<module::Coset           <B,Q  >> build_coset_real (const int tid = 0);
	virtual std::unique_ptr<module::Coset           <B,B  >> build_coset_bit  (const int tid = 0);
	virtual std::unique_ptr<tools ::Interleaver_core<     >> build_interleaver(const int tid = 0);
};
}
}

#endif /* SIMULATION_BFER_ITE_HPP_ */
