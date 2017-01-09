/*
 * CPM_parameters.hpp
 *
 *  Created on: 14 dec. 2016
 *      Author: ohartmann
 */

#ifndef CPM_PARAMETERS_HPP_
#define CPM_PARAMETERS_HPP_

#include <fstream>
#include <string>
#include "Tools/Perf/MIPP/mipp.h"

template </*typename R, */typename S_in, typename S_out>
class CPM_parameters
{
public :

	CPM_parameters(	 const std::string& _cpm_standard,
					 const int _L,
					 const int _k,
					 const int _p,
					 const int _Nb_b_per_s,
					 const int _Sf,
					 const std::string& _filters_type,
					 const std::string& _wave_shape);
	virtual ~CPM_parameters(){}

	const std::string cpm_standard; // the selection of a default cpm standard hardly implemented {gsm, chain1, chain2}

	int L;			// cpm pulse width or cpm memory
	int k;			// modulation index numerator
	int p;			// modulation index denumerator
	int Nb_b_per_s; // number of bits per symbol
	int Sf;			// sampling factor

	const std::string filters_type; // Choose receiver filters bank: {TOTAL}
	const std::string wave_shape;	// Choose wave shape : {GMSK, REC, RCOS}

	int Mo;			// modulation order of the cpm
	//R   h;			// modulation index
	//R   T_samp;		// sample duration
	int Nb_bits_p;  // number of bits used to code p
	int tl; 		// tail length for the modulation

	int Nb_wa;		// number of wave forms used by the CPM
	int Nb_bits_wa;	// number of bits used to code Nb_wa
	int max_wa_id;  // maximum possible index of a trellis state


	int Nb_st;		// number of states in the trellis
	int Nb_bits_st;	// number of bits used to code Nb_st
	int max_st_id;  // maximum possible index of a trellis state

//	mipp::vector<R>     baseband;   // translation of base band vectors           (80 complex elmts)
//	mipp::vector<R>     projection; // translation of filtering generator familly (80 complex elmts)


	mipp::vector<S_in>  transition_to_binary; // gives the binary code for a transition. for each transition there are Nb_b_per_s bits separately given (separate memory cases), in msb to lsb order
	mipp::vector<S_in>  binary_to_transition; // gives the transition related to the binary code

	mipp::vector<int>   allowed_states;		// possible used state indexes (there are jumps in the indexes because of p)
	mipp::vector<S_out> allowed_wave_forms;	// possible used wave forms indexes (there are jumps in the indexes because of p)

	mipp::vector<int>   trellis_next_state;	// from a given state and transition, gives the next state
	mipp::vector<S_out> trellis_related_wave_form; // from a given state and transition, gives the related wave form

	mipp::vector<int>   anti_trellis_original_state; // from a given state and transition index, gives the original state from where comes the transition
	mipp::vector<S_in>  anti_trellis_input_transition;// from a given state and transition index, gives the related transition value
};

#include "CPM_parameters.hxx"

#endif /* CPM_PARAMETERS_HPP_ */
