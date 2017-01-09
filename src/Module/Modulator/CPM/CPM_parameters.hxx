/*
 * CPM_parameters.hxx
 *
 *  Created on: 14 dec. 2016
 *      Author: ohartmann
 */

#include <cmath>
#include <ctgmath>
#include "CPM_parameters.hpp"

template </*typename R, */typename S_in, typename S_out>
CPM_parameters</*R, */S_in, S_out>
::CPM_parameters(const std::string& _cpm_standard,
				 const int _L,
				 const int _k,
				 const int _p,
				 const int _Nb_b_per_s,
				 const int _Sf,
				 const std::string& _filters_type,
				 const std::string& _wave_shape)
: 	cpm_standard(_cpm_standard             ),
	L           (_L                        ),
	k           (_k                        ),
	p           (_p                        ),
	Nb_b_per_s  (_Nb_b_per_s               ),
	Sf          (_Sf                       ),
	filters_type(_filters_type             ),
	wave_shape  (_wave_shape               ),
											 //    gsm		chain 1  chain 2
	Mo          ((int)1 << Nb_b_per_s      ),// 	2			2		4
	//h           ((R)k/(R)p                 ),// 	1/2			1/2		1/3
	//T_samp      ((R)1.0/(R)Sf              ),// 	1/5			1/5		1/5
	Nb_bits_p   ((int)ceil(log2((double)p))),//		1			1		2
	tl          (L                         ),// 	3			2		2

/** Olivier : Warning !!! From here parameters are working for Rimoldi decomposition only !!! **/

	Nb_wa       (p * pow(Mo, L)            ),// 	16			8		48
	Nb_bits_wa  ((int)ceil(log2(Nb_wa))    ),//		4			3		6
	max_wa_id   (((int)1 << Nb_bits_wa)    ),//		16		 	8		64

	Nb_st       (p*pow(Mo, L-1)            ),//		8			4		12
	Nb_bits_st  ((int)ceil(log2(Nb_st))    ),//		3			2		4
	max_st_id   (((int)1 << Nb_bits_st)    ),//		8			4		16

//	baseband					 (max_wa_id *Sf *2  ,  0),
//	projection					 (max_wa_id *Sf *2  ,  0),
	transition_to_binary         (Nb_b_per_s    * Mo, -1),
	binary_to_transition         (                Mo, -1),
	allowed_states         		 (Nb_st             , -1),
	allowed_wave_forms           (Nb_wa             , -1),
	trellis_next_state           (max_st_id     * Mo, -1),
	trellis_related_wave_form    (max_st_id     * Mo, -1),
	anti_trellis_original_state  (max_st_id     * Mo, -1),
	anti_trellis_input_transition(max_st_id     * Mo, -1)
{
}
