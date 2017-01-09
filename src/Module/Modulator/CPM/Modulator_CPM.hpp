/*
 * Modulator_CPM.hpp
 *
 *  Created on: 14 dec. 2016
 *      Author: ohartmann
 */

#ifndef MODULATOR_CPM_HPP_
#define MODULATOR_CPM_HPP_

#include <fstream>
#include <string>
#include "Tools/Perf/MIPP/mipp.h"
#include "Tools/params.h"
#include "Tools/Math/max.h"

#include "CPM_parameters.hpp"
#include "../Modulator.hpp"
#include "BCJR/BCJR.hpp"

#include "Module/Encoder/CPE/Encoder_CPE_Rimoldi.hpp"

/** Olivier : Warning !!! Working for Rimoldi decomposition only !!! **/

template <typename B, typename R, typename Q, proto_max<Q> MAX>
class Modulator_CPM : public Modulator<B,R,Q>
{
	typedef B S_in;
	typedef B S_out;

public:
	Modulator_CPM(int N, const modulator_parameters &params, const R sigma, const int n_frames = 1,
					const std::string name = "Modulator_CPM");
	virtual ~Modulator_CPM();

	int get_buffer_size_after_modulation(const int N);
	int get_buffer_size_after_filtering (const int N);

	void   modulate(const mipp::vector<B>& X_N1,                              mipp::vector<R>& X_N2);
	void     filter(const mipp::vector<R>& Y_N1,                              mipp::vector<R>& Y_N2);
	void demodulate(const mipp::vector<Q>& Y_N1,                              mipp::vector<Q>& Y_N2);
	void demodulate(const mipp::vector<Q>& Y_N1, const mipp::vector<Q>& Y_N2, mipp::vector<Q>& Y_N3);

protected:
	// input
	const int N; 		// number of bits to send for one frame
	const R   sigma;	// sigma^2 = noise variance

	// modulation data
	CPM_parameters<S_in,S_out> 		cpm; 		// all cpm parameters
	R   							cpm_h;		// modulation index = k/p
	R   							T_samp;		// sample duration  = 1/Sf
	mipp::vector<R>     			baseband;   // translation of base band vectors           (80 complex elmts)
	mipp::vector<R>     			projection; // translation of filtering generator familly (80 complex elmts)
	const int 						Nb_sy;    	// number of symbols for one frame after encoding without tail symbols
	const int 						Nb_sy_tl; 	// number of symbols to send for one frame after encoding with tail symbols
	Encoder_CPE_Rimoldi<S_in,S_out>	cpe; 		// the continuous phase encoder

	// demodulator
	BCJR<S_in,S_out,Q,MAX> bcjr;

	void _modulate(const mipp::vector<B>& X_N1, mipp::vector<R>& X_N2);
	void   _filter(const mipp::vector<R>& Y_N1, mipp::vector<R>& Y_N2);

private :
	void generate_baseband  (                 );
	void generate_projection(                 );
	R calculate_phase_response(const R t_stamp);
};

#include "Modulator_CPM.hxx"



#endif /* MODULATOR_CPM_HPP_ */
