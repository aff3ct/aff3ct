#ifndef SIMULATION_BFER_I_HPP_
#define SIMULATION_BFER_I_HPP_

#include <chrono>
#include <vector>

#include "Tools/params.h"
#include "Tools/Threads/Barrier.hpp"

#include "Module/Source/Source.hpp"
#include "Module/CRC/CRC.hpp"
#include "Module/Encoder/Encoder.hpp"
#include "Module/Puncturer/Puncturer.hpp"
#include "Module/Modulator/Modulator.hpp"
#include "Module/Channel/Channel.hpp"
#include "Module/Quantizer/Quantizer.hpp"
#include "Module/Coset/Coset.hpp"
#include "Module/Decoder/Decoder.hpp"
#include "Module/Monitor/Monitor.hpp"

#include "../Simulation.hpp"

template <typename B, typename R, typename Q>
class Simulation_BFER_i : public Simulation
{
protected:
	// simulation parameters
	const parameters &params;

	// number of obj to allocate
	const int n_obj;

	// a barrier to synchronize the threads
	Barrier barrier;

	// code specifications
	float snr;
	float code_rate;
	float sigma;

	// time points and durations
	std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> t_snr;

	// communication chain
	std::vector<Source   <B    >*> source;
	std::vector<CRC      <B    >*> crc;
	std::vector<Encoder  <B    >*> encoder;
	std::vector<Puncturer<B,Q  >*> puncturer;
	std::vector<Modulator<B,R,R>*> modulator;
	std::vector<Channel  <R    >*> channel;
	std::vector<Quantizer<R,Q  >*> quantizer;
	std::vector<Coset    <B,Q  >*> coset_real;
	std::vector<Decoder  <B,Q  >*> decoder;
	std::vector<Coset    <B,B  >*> coset_bit;
	std::vector<Monitor  <B    >*> monitor;

public:
	Simulation_BFER_i(const parameters& params);
	virtual ~Simulation_BFER_i();
	void launch();

protected:
	static void build_communication_chain(Simulation_BFER_i<B,R,Q> *simu, const int tid = 0);

	virtual void launch_precompute ();
	virtual void snr_precompute    ();
	virtual void _launch           () = 0;
	virtual void snr_postcompute   ();
	virtual void release_objects   ();
	virtual void launch_postcompute();

	virtual Source   <B    >* build_source    (                const int tid = 0);
	virtual CRC      <B    >* build_crc       (                const int tid = 0);
	virtual Encoder  <B    >* build_encoder   (                const int tid = 0);
	virtual Puncturer<B,Q  >* build_puncturer (                const int tid = 0);
	virtual Modulator<B,R,R>* build_modulator (                const int tid = 0);
	virtual Channel  <R    >* build_channel   (const int size, const int tid = 0);
	virtual Quantizer<R,Q  >* build_quantizer (const int size, const int tid = 0);
	virtual Coset    <B,Q  >* build_coset_real(                const int tid = 0);
	virtual Decoder  <B,Q  >* build_decoder   (                const int tid = 0) = 0;
	virtual Coset    <B,B  >* build_coset_bit (                const int tid = 0);
	virtual Monitor  <B    >* build_monitor   (                const int tid = 0);
};

#endif /* SIMULATION_BFER_I_HPP_ */
