#if !defined(PREC_8_BIT) && !defined(PREC_16_BIT)

#ifndef SIMULATION_EXIT_HPP_
#define SIMULATION_EXIT_HPP_

#include <vector>
#include <mipp.h>

#include "Module/Source/Source.hpp"
#include "Module/Codec/Codec_SISO.hpp"
#include "Module/Modem/Modem.hpp"
#include "Module/Channel/Channel.hpp"
#include "Module/Decoder/Decoder_SISO.hpp"
#include "Module/Monitor/EXIT/Monitor_EXIT.hpp"

#include "Tools/Display/Terminal/Terminal.hpp"
#include "Tools/Noise/Noise.hpp"

#include "Tools/Display/Reporter/EXIT/Reporter_EXIT.hpp"
#include "Tools/Display/Reporter/Noise/Reporter_noise.hpp"
#include "Tools/Display/Reporter/Throughput/Reporter_throughput.hpp"

#include "Factory/Simulation/EXIT/EXIT.hpp"

#include "../Simulation.hpp"

namespace aff3ct
{
namespace simulation
{
template <typename B = int, typename R = float>
class EXIT : public Simulation
{
protected:
	const factory::EXIT::parameters &params_EXIT; // simulation parameters

	// code specifications
	tools::Sigma<R>  noise;   // current noise simulated
	tools::Sigma<R>  noise_a; // current noise simulated for the "a" part
	R sig_a;

	// communication chain
	std::shared_ptr<module::Source      <B  >> source;
	std::shared_ptr<module::Codec_SISO  <B,R>> codec;
	std::shared_ptr<module::Modem       <B,R>> modem;
	std::shared_ptr<module::Modem       <B,R>> modem_a;
	std::shared_ptr<module::Channel     <  R>> channel;
	std::shared_ptr<module::Channel     <  R>> channel_a;
	std::shared_ptr<module::Decoder_SISO<  R>> siso;
	std::shared_ptr<module::Monitor_EXIT<B,R>> monitor;

	// terminal and reporters (for the output of the code)
	std::vector<std::shared_ptr<tools::Reporter>> reporters;
	std::shared_ptr<tools::Terminal>              terminal;


public:
	explicit EXIT(const factory::EXIT::parameters &params_EXIT);
	virtual ~EXIT();

	void launch();

protected:
	void _build_communication_chain();
	void sockets_binding           ();
	void simulation_loop           ();
	void release_objects           ();

	std::shared_ptr<module::Source      <B  >> build_source   (              );
	std::shared_ptr<module::Codec_SISO  <B,R>> build_codec    (              );
	std::shared_ptr<module::Modem       <B,R>> build_modem    (              );
	std::shared_ptr<module::Modem       <B,R>> build_modem_a  (              );
	std::shared_ptr<module::Channel     <  R>> build_channel  (const int size);
	std::shared_ptr<module::Channel     <  R>> build_channel_a(const int size);
	std::shared_ptr<module::Monitor_EXIT<B,R>> build_monitor  (              );
	std::shared_ptr<tools::Terminal          > build_terminal (              );
};
}
}

#endif /* SIMULATION_EXIT_HPP_ */

#endif