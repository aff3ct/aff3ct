#ifndef SIMULATION_BFER_STD_HPP_
#define SIMULATION_BFER_STD_HPP_

#include <chrono>
#include <vector>
#include <random>
#include <memory>

#include "Module/Source/Source.hpp"
#include "Module/CRC/CRC.hpp"
#include "Module/Codec/Codec_SIHO.hpp"
#include "Module/Modem/Modem.hpp"
#include "Module/Channel/Channel.hpp"
#include "Module/Quantizer/Quantizer.hpp"
#include "Module/Coset/Coset.hpp"

#include "Factory/Simulation/BFER/BFER_std.hpp"

#include "Simulation/BFER/BFER.hpp"

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
	std::vector<std::unique_ptr<module::Source    <B    >>> source;
	std::vector<std::unique_ptr<module::CRC       <B    >>> crc;
	std::vector<std::unique_ptr<module::Codec_SIHO<B,Q  >>> codec;
	std::vector<std::unique_ptr<module::Modem     <B,R,R>>> modem;
	std::vector<std::unique_ptr<module::Channel   <R    >>> channel;
	std::vector<std::unique_ptr<module::Quantizer <R,Q  >>> quantizer;
	std::vector<std::unique_ptr<module::Coset     <B,Q  >>> coset_real;
	std::vector<std::unique_ptr<module::Coset     <B,B  >>> coset_bit;

	// a vector of random generator to generate the seeds
	std::vector<std::mt19937> rd_engine_seed;

public:
	explicit BFER_std(const factory::BFER_std::parameters &params_BFER_std);
	virtual ~BFER_std() = default;

protected:
	virtual void __build_communication_chain(const int tid = 0);
	virtual void _launch();

	std::unique_ptr<module::Source    <B    >> build_source    (const int tid = 0);
	std::unique_ptr<module::CRC       <B    >> build_crc       (const int tid = 0);
	std::unique_ptr<module::Codec_SIHO<B,Q  >> build_codec     (const int tid = 0);
	std::unique_ptr<module::Modem     <B,R,R>> build_modem     (const int tid = 0);
	std::unique_ptr<module::Channel   <R    >> build_channel   (const int tid = 0);
	std::unique_ptr<module::Quantizer <R,Q  >> build_quantizer (const int tid = 0);
	std::unique_ptr<module::Coset     <B,Q  >> build_coset_real(const int tid = 0);
	std::unique_ptr<module::Coset     <B,B  >> build_coset_bit (const int tid = 0);
};
}
}

#endif /* SIMULATION_BFER_STD_HPP_ */
