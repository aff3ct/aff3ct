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

#include "Tools/Display/Terminal/EXIT/Terminal_EXIT.hpp"
#include "../../Tools/Noise/Noise.hpp"

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
	tools::Sigma<R> noise;   // current noise simulated
	tools::Sigma<R> noise_a; // current noise simulated for the "a" part
	float sig_a;

	// communication chain
	module::Source      <B  > *source;
	module::Codec_SISO  <B,R> *codec;
	module::Modem       <B,R> *modem;
	module::Modem       <B,R> *modem_a;
	module::Channel     <  R> *channel;
	module::Channel     <  R> *channel_a;
	module::Decoder_SISO<  R> *siso;
	module::Monitor_EXIT<B,R> *monitor;
	tools::Terminal_EXIT<B,R> *terminal;

public:
	explicit EXIT(const factory::EXIT::parameters &params_EXIT);
	virtual ~EXIT();

	void launch();

protected:
	void _build_communication_chain();
	void sockets_binding           ();
	void simulation_loop           ();
	void release_objects           ();

	module::Source      <B  >* build_source   (              );
	module::Codec_SISO  <B,R>* build_codec    (              );
	module::Modem       <B,R>* build_modem    (              );
	module::Modem       <B,R>* build_modem_a  (              );
	module::Channel     <  R>* build_channel  (const int size);
	module::Channel     <  R>* build_channel_a(const int size);
	module::Monitor_EXIT<B,R>* build_monitor  (              );
	tools::Terminal_EXIT<B,R>* build_terminal (              );
};
}
}

#endif /* SIMULATION_EXIT_HPP_ */

#endif