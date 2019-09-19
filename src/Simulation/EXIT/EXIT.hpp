#if !defined(AFF3CT_8BIT_PREC) && !defined(AFF3CT_16BIT_PREC)

#ifndef SIMULATION_EXIT_HPP_
#define SIMULATION_EXIT_HPP_

#include <vector>
#include <memory>

#include "Tools/Display/Terminal/Terminal.hpp"
#include "Tools/Noise/Sigma.hpp"
#include "Tools/Display/Reporter/Reporter.hpp"
#include "Module/Source/Source.hpp"
#include "Module/Codec/Codec_SISO.hpp"
#include "Module/Modem/Modem.hpp"
#include "Module/Channel/Channel.hpp"
#include "Module/Decoder/Decoder_SISO.hpp"
#include "Module/Monitor/EXIT/Monitor_EXIT.hpp"

#include "Factory/Simulation/EXIT/EXIT.hpp"

#include "Simulation/Simulation.hpp"

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
	std::unique_ptr<module::Source      <B  >> source;
	std::unique_ptr<module::Codec_SISO  <B,R>> codec;
	std::unique_ptr<module::Modem       <B,R>> modem;
	std::unique_ptr<module::Modem       <B,R>> modem_a;
	std::unique_ptr<module::Channel     <  R>> channel;
	std::unique_ptr<module::Channel     <  R>> channel_a;
	std::unique_ptr<module::Decoder_SISO<  R>> siso;
	std::unique_ptr<module::Monitor_EXIT<B,R>> monitor;

	// terminal and reporters (for the output of the code)
	std::vector<std::unique_ptr<tools::Reporter>> reporters;
	std::unique_ptr<tools::Terminal>              terminal;

public:
	explicit EXIT(const factory::EXIT::parameters &params_EXIT);
	virtual ~EXIT() = default;

	void launch();

protected:
	void _build_communication_chain();
	void sockets_binding           ();
	void simulation_loop           ();

	std::unique_ptr<module::Source      <B  >> build_source   (              );
	std::unique_ptr<module::Codec_SISO  <B,R>> build_codec    (              );
	std::unique_ptr<module::Modem       <B,R>> build_modem    (              );
	std::unique_ptr<module::Modem       <B,R>> build_modem_a  (              );
	std::unique_ptr<module::Channel     <  R>> build_channel  (const int size);
	std::unique_ptr<module::Channel     <  R>> build_channel_a(const int size);
	std::unique_ptr<module::Monitor_EXIT<B,R>> build_monitor  (              );
	std::unique_ptr<tools::Terminal          > build_terminal (              );
};
}
}

#endif /* SIMULATION_EXIT_HPP_ */

#endif