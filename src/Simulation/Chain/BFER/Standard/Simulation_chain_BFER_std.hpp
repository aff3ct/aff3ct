#ifndef SIMULATION_CHAIN_BFER_STD_HPP_
#define SIMULATION_CHAIN_BFER_STD_HPP_

#include <memory>

#include "Module/Source/Source.hpp"
#include "Module/CRC/CRC.hpp"
#include "Module/Modem/Modem.hpp"
#include "Module/Channel/Channel.hpp"
#include "Module/Quantizer/Quantizer.hpp"
#include "Module/Coset/Coset.hpp"
#include "Tools/Codec/Codec_SIHO.hpp"
#include "Tools/Constellation/Constellation.hpp"
#include "Tools/Math/Distribution/Distributions.hpp"
#include "Factory/Simulation/BFER/BFER_std.hpp"
#include "Simulation/Chain/BFER/Simulation_chain_BFER.hpp"

namespace aff3ct
{
namespace simulation
{

template <typename B = int, typename R = float, typename Q = R>
class Simulation_chain_BFER_std : public Simulation_chain_BFER<B,R>
{
protected:
	const factory::BFER_std &params_BFER_std;

	// communication chain
	std::unique_ptr<module::Source    <B    >> source;
	std::unique_ptr<module::CRC       <B    >> crc;
	std::unique_ptr<tools ::Codec_SIHO<B,Q  >> codec;
	std::unique_ptr<module::Modem     <B,R,R>> modem;
	std::unique_ptr<module::Channel   <R    >> channel;
	std::unique_ptr<module::Quantizer <R,Q  >> quantizer;
	std::unique_ptr<module::Coset     <B,Q  >> coset_real;
	std::unique_ptr<module::Coset     <B,B  >> coset_bit;

public:
	explicit Simulation_chain_BFER_std(const factory::BFER_std &params_BFER_std);
	virtual ~Simulation_chain_BFER_std() = default;

protected:
	std::unique_ptr<module::Source    <B    >> build_source    (                                            );
	std::unique_ptr<module::CRC       <B    >> build_crc       (                                            );
	std::unique_ptr<tools ::Codec_SIHO<B,Q  >> build_codec     (const module::CRC         <B> *crc          );
	std::unique_ptr<module::Modem     <B,R,R>> build_modem     (const tools::Distributions<R> *distributions,
	                                                            const tools::Constellation<R> *constellation);
	std::unique_ptr<module::Channel   <R    >> build_channel   (const tools::Distributions<R> *distributions);
	std::unique_ptr<module::Quantizer <R,Q  >> build_quantizer (                                            );
	std::unique_ptr<module::Coset     <B,Q  >> build_coset_real(                                            );
	std::unique_ptr<module::Coset     <B,B  >> build_coset_bit (                                            );

	virtual void create_modules();
	virtual void bind_sockets();
	virtual void create_chain();
};

}
}

#endif /* SIMULATION_CHAIN_BFER_STD_HPP_ */
