#ifndef SIMULATION_CHAIN_BFER_STD_HPP_
#define SIMULATION_CHAIN_BFER_STD_HPP_

#include <memory>
#include <chrono>

#include "Module/Source/Source.hpp"
#include "Module/CRC/CRC.hpp"
#include "Tools/Codec/Codec_SIHO.hpp"
#include "Module/Modem/Modem.hpp"
#include "Module/Channel/Channel.hpp"
#include "Module/Quantizer/Quantizer.hpp"
#include "Module/Coset/Coset.hpp"
#include "Module/Monitor/MI/Monitor_MI.hpp"
#include "Module/Monitor/BFER/Monitor_BFER.hpp"
#include "Tools/Chain/Chain.hpp"
#include "Tools/Constellation/Constellation.hpp"
#include "Tools/Display/Reporter/Reporter.hpp"
#include "Tools/Display/Terminal/Terminal.hpp"
#include "Tools/Display/Dumper/Dumper.hpp"
#include "Tools/Display/Dumper/Dumper_reduction.hpp"
#include "Tools/Math/Distribution/Distributions.hpp"
#include "Tools/Monitor/Monitor_reduction.hpp"
#ifdef AFF3CT_MPI
#include "Tools/Monitor/Monitor_reduction_MPI.hpp"
#endif
#include "Tools/Noise/Noise.hpp"
#include "Factory/Simulation/Simulation.hpp"
#include "Factory/Simulation/BFER/BFER.hpp"
#include "Factory/Simulation/BFER/BFER_std.hpp"
#include "Simulation/Simulation.hpp"

namespace aff3ct
{
namespace simulation
{

template <typename B = int, typename R = float, typename Q = R>
class Simulation_chain_BFER_std : public Simulation
{
protected:
	const factory::Simulation &params;
	const factory::BFER       &params_BFER;
	const factory::BFER_std   &params_BFER_std;

	std::unique_ptr<tools::Noise        < >> noise;
	std::unique_ptr<tools::Distributions<R>> distributions;
	std::unique_ptr<tools::Constellation<R>> constellation;

	// communication chain
	std::unique_ptr<module::Source      <B    >> source;
	std::unique_ptr<module::CRC         <B    >> crc;
	std::unique_ptr<tools ::Codec_SIHO  <B,Q  >> codec;
	std::unique_ptr<module::Modem       <B,R,R>> modem;
	std::unique_ptr<module::Channel     <R    >> channel;
	std::unique_ptr<module::Quantizer   <R,Q  >> quantizer;
	std::unique_ptr<module::Coset       <B,Q  >> coset_real;
	std::unique_ptr<module::Coset       <B,B  >> coset_bit;
	std::unique_ptr<module::Monitor_BFER<B    >> monitor_er;
	std::unique_ptr<module::Monitor_MI  <B,R  >> monitor_mi;
	std::unique_ptr<tools ::Chain              > chain;

	std::vector<std::unique_ptr<tools::Reporter>> reporters;
	std::unique_ptr<tools::Terminal> terminal;

#ifdef AFF3CT_MPI
	std::unique_ptr<tools::Monitor_reduction_MPI<module::Monitor_BFER<B>>> monitor_er_red;
	std::unique_ptr<tools::Monitor_reduction_MPI<module::Monitor_MI<B,R>>> monitor_mi_red;
#else
	std::unique_ptr<tools::Monitor_reduction<module::Monitor_BFER<B>>> monitor_er_red;
	std::unique_ptr<tools::Monitor_reduction<module::Monitor_MI<B,R>>> monitor_mi_red;
#endif

	// dump frames into files
	std::vector<std::unique_ptr<tools::Dumper         >> dumper;
	            std::unique_ptr<tools::Dumper_reduction> dumper_red;

	std::chrono::steady_clock::time_point t_start_noise_point;

public:
	explicit Simulation_chain_BFER_std(const factory::BFER_std &params_BFER_std);

	virtual ~Simulation_chain_BFER_std() = default;

	void launch();

protected:
	std::unique_ptr<module::Source      <B    >> build_source    (                                            );
	std::unique_ptr<module::CRC         <B    >> build_crc       (                                            );
	std::unique_ptr<tools ::Codec_SIHO  <B,Q  >> build_codec     (const module::CRC         <B> *crc          );
	std::unique_ptr<module::Modem       <B,R,R>> build_modem     (const tools::Distributions<R> *distributions,
	                                                              const tools::Constellation<R> *constellation);
	std::unique_ptr<module::Channel     <R    >> build_channel   (const tools::Distributions<R> *distributions);
	std::unique_ptr<module::Quantizer   <R,Q  >> build_quantizer (                                            );
	std::unique_ptr<module::Coset       <B,Q  >> build_coset_real(                                            );
	std::unique_ptr<module::Coset       <B,B  >> build_coset_bit (                                            );
	std::unique_ptr<module::Monitor_MI  <B,R  >> build_monitor_mi(                                            );
	std::unique_ptr<module::Monitor_BFER<B    >> build_monitor_er(                                            );
	std::unique_ptr<tools ::Terminal           > build_terminal  (const std::vector<std::unique_ptr<tools::Reporter>> &reporters);

	std::vector<std::unique_ptr<tools::Reporter>> build_reporters(const tools ::Noise       < > *noise,
	                                                              const module::Monitor_BFER<B> *monitor_er,
	                                                              const module::Monitor_MI<B,R> *monitor_mi);

	void create_modules();
	void bind_sockets();
	void create_chain();
	void configure_chain_tasks();
	void create_monitors_reduction();

	bool stop_time_reached();
	bool stop_condition();
};

}
}

#include "Simulation/Chain/Simulation_chain_BFER_std.hxx"

#endif /* SIMULATION_CHAIN_BFER_STD_HPP_ */
