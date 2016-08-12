#include <limits>

#include "../../../Tools/bash_tools.h"

#include "hard_coded_matrices.h"
#include "Decoder_LDPC_BP_naive.hpp"

template <typename B, typename R>
Decoder_LDPC_BP_naive<B,R>
::Decoder_LDPC_BP_naive(const int &N, const int &K, const int& n_ite)
: Decoder<B,R>(),
  n_ite    (n_ite                  ), 
  init_flag(0                      ),

  Y_N      (N                      ),
  V_K      (K                      ),
  Lp_N     (N,                    0),
  C_to_V   (this->message_length, 0),
  V_to_C   (this->message_length, 0)
  
{
	// TODO: implement the BP algo in order to remove this assertion
	assert(N == K);
}

template <typename B, typename R>
Decoder_LDPC_BP_naive<B,R>
::~Decoder_LDPC_BP_naive()
{
}

template <typename B, typename R>
void Decoder_LDPC_BP_naive<B,R>
::decode(const mipp::vector<R> &sys, const mipp::vector<R> &par, mipp::vector<R> &ext)
{
	std::cerr << bold_red("(EE) This decoder does not support this interface.") << std::endl;
	std::exit(-1);
}

template <typename B, typename R>
void Decoder_LDPC_BP_naive<B,R>
::decode(const mipp::vector<R> &Y_N1, mipp::vector<R> &Y_N2)
{
	assert(Y_N1.size() == Y_N2.size());

	// actual decoding
	this->BP_decode(Y_N1);

	// prepare for next round by processing extrinsic information
	for(auto i = 0; i < (int)Y_N2.size(); i++)
		Y_N2[i] = this->Lp_N[i] - Y_N1[i];

	this->saturate(Y_N2, 15.f);

	this->init_flag++;
}

template <typename B, typename R>
void Decoder_LDPC_BP_naive<B,R>
::load(const mipp::vector<R>& Y_N)
{
	assert(Y_N.size() == this->Y_N.size());
	this->Y_N = Y_N;
}

template <typename B, typename R>
void Decoder_LDPC_BP_naive<B,R>
::decode()
{
	// actual decoding
	this->BP_decode(this->Y_N);

	// TODO: this is wrong !
	for (unsigned i = 0; i < V_K.size(); i++)
		V_K[i] = this->Lp_N[i] < 0;

	// reset so C_to_V and V_to_C structures can be cleared only at the begining of the loop
	// in iterative decoding
	this->init_flag = 0;
}

template <typename B, typename R>
void Decoder_LDPC_BP_naive<B,R>
::store(mipp::vector<B>& V_K) const
{
	assert(V_K.size() == this->V_K.size());
	V_K = this->V_K;
}

// wrapping making sure of value returned by signbit
template <typename B, typename R>
int Decoder_LDPC_BP_naive<B,R>
::signbit(const R val)
{
	return std::signbit(val) ? -1 : 0;
}

// saturate the whole array with
template <typename B, typename R>
void Decoder_LDPC_BP_naive<B,R>
::saturate(mipp::vector<R> &array, R saturation)
{
	for (auto i = 0; i < (int)array.size(); i++)
		if      (array[i] >  saturation) array[i] =  saturation;
		else if (array[i] < -saturation) array[i] = -saturation;
}

// BP-like algorithm (normalized offest min-sum)
template <typename B, typename R>
bool Decoder_LDPC_BP_naive<B,R>
::BP_decode(const mipp::vector<R> &var_nodes)//const R *var_nodeS) 
{
	// memory zones initialization
	if (this->init_flag == 0) std::fill(this->C_to_V.begin(), this->C_to_V.end(), 0);

	// init variables
	R *MSG_C_2_V = this->C_to_V.data();
	R *MSG_V_2_C = this->V_to_C.data();

	// actual decoding
	auto ite = 0, syndromes = 0;
	for (; ite < this->n_ite; ite++)
	{
		syndromes = 0;

		// begining of the iteration upon all the matrix lines --------------------------------------------------------
		R *_MSG_C_2_V_ = MSG_C_2_V;
		R *_MSG_V_2_C  = MSG_V_2_C;

		for (auto i = 0; i < n_V_nodes; i++) 
		{
			// VN node accumulate all the incoming messages
			const auto length = nbPariteParVariable[i];

			auto sum_C_to_V = (R)0;
			for (auto q = 0; q < length; q++)
				sum_C_to_V += _MSG_C_2_V_[q];

			// update the intern values
			const auto temp = var_nodes[i] + sum_C_to_V;

			// generate the outcoming messages to the CNs  
			for (auto q = 0; q < length; q++)
				_MSG_V_2_C[q] = temp - _MSG_C_2_V_[q];

			_MSG_C_2_V_ += length; // give the messages to the next node
			_MSG_V_2_C  += length; // give the messages to the next node
		}
		// end of the iteration upon all the matrix lines -------------------------------------------------------------

		auto _TRANSPOSE = transpose;
		for (auto i = 0; i < n_C_nodes; i++) 
		{
			const auto length = nbVariableParParite[i];

			auto sign = 0;
			auto min1 = std::numeric_limits<R>::infinity();
			auto min2 = std::numeric_limits<R>::infinity();

			// accumulate the incoming information in CN
			for (auto q = 0; q < length; q++)
			{
				const auto value = MSG_V_2_C[_TRANSPOSE[q]]; // _TRANSPOSE++;
				const auto cSign = this->signbit(value);
				const auto vAbs  = (R)std::abs(value);       // std::trunc(std::abs(value));
				const auto vTemp = min1;

				sign ^= cSign;
				min1  = std::min(min1,          vAbs        ); // 1st min
				min2  = std::min(min2, std::max(vAbs, vTemp)); // 2nd min
			}

			// compute CN inter values
			auto cste1 = (min2 - offset) * normalize1;
			auto cste2 = (min1 - offset) * normalize2;
			cste1 = (cste1 < 0) ? 0 : cste1;
			cste2 = (cste2 < 0) ? 0 : cste2;

			// regenerate the CN outcoming values
			for (auto q = 0; q < length; q++)
			{
				const auto value = MSG_V_2_C[_TRANSPOSE[q]];
				const auto vAbs  = (R)std::abs(value);               // std::trunc(std::abs(value));
				const auto vRes  = ((vAbs == min1) ? cste1 : cste2); // cmov
				const auto vSig  = sign ^ this->signbit(value);      // xor bit
				const auto v2St  = (R)std::copysign(vRes, vSig);     // magnitude of vRes, sign of vSig
				
				MSG_C_2_V[_TRANSPOSE[q]] = v2St;
			}

			_TRANSPOSE += length;
			syndromes = syndromes || sign;
		}

		// is it a saturation? is it the right place to do this?
		for (auto i = 0; i < message_length; i++) 
		{
			const auto satTemp = MSG_C_2_V[i];
			if      (satTemp >  C_to_V_max) MSG_C_2_V[i] =  C_to_V_max;
			else if (satTemp < -C_to_V_max) MSG_C_2_V[i] = -C_to_V_max;
		}

		// management of the stop criterion
		if ((syndromes == 0) && (fast_stop == 1)) 
			break;
	}

	// begining of the iteration upon all the matrix lines
	R *_MSG_C_2_V = MSG_C_2_V;
	for (auto i = 0; i < n_V_nodes; i++) 
	{
		const auto length = nbPariteParVariable[i];

		auto sum_C_to_V = (R)0;
		for(auto q = 0; q < length; q++)
			sum_C_to_V += _MSG_C_2_V[q];

		// filling the output
		this->Lp_N[i] = var_nodes[i] + sum_C_to_V;

		_MSG_C_2_V += length;
	}
	// end of the interative decoding process    

	return (syndromes == 0);
}

// // dead code
// // BP-Like Algorithm (Sum-Product Algorithm)
// template <typename B, typename R>
// bool Decoder_LDPC_BP_naive<B,R>
// ::SPA_decode( 
//             const R       		  var_nodeS[],  
//             const int     		  nombre_iterations,  const int     		initFlag, 
//             	  mipp::vector<R> &C2V, 			  		mipp::vector<R> &V2C,
//             	  mipp::vector<R> &Rprime_fix,				int*    		iters
//            ) //const // Changed To allow Coset Approach in IterDEV
// {

//     //
//     // Init Variables
//     //
    
//     int     syndromes = 0;
//     int     iterations;
//     int     length;
    
//     R       sum_CtoV;
//     R       msg_in;
    
//     R*      MSG_C_2_V = C2V.data();
//     R* 		MSG_V_2_C = V2C.data();

//     R       temp;
    
//     int     sign;
    
//     const unsigned int *_TRANSPOSE  = transpose;
//     const unsigned int *_TRANSPOSE_ = transpose;
    
//     R       value;
//     int     cSign;
//     R       vAbs;
    
//     R       vRes;
//     int     vSig;
//     R       v2St;

//     R       satTemp;
    
//     //
//     // ON INITIALISE LES ZONES MEMOIRES (MESSAGES)
//     //

//     if (initFlag == 0)
//     {
//     	fill(C2V.begin(), C2V.end(), 0.0);
//     }

//     //
//     //  ACTUAL DECODING
//     //

//     for(iterations = 0; iterations < nombre_iterations; iterations += 1)
//     {
//         syndromes = 0;

//         // DEBUT DE L'ITERATION SUR TOUTES LES LIGNES DE LA MATRICE
//         R *_MSG_C_2_V_  = MSG_C_2_V;
//         R *_MSG_V_2_C   = MSG_V_2_C;

//         for (int i = 0; i < VNodesNb; i++) 
//         {

//             //
//             // LE NOEUD VN ACCUMULE TOUS LES MESSAGES ENTRANT
//             //
//             length = nbPariteParVariable[i];
//             sum_CtoV = 0;
//             for(int q = 0; q < length; q++)/*while (length--)*/ 
//             {
//                 msg_in      = *(_MSG_C_2_V_ + q);
//                 sum_CtoV    =  sum_CtoV + msg_in;
//             }

//             //
//             // ON MET A JOUR LA VALEUR INTERNE
//             //
//             temp = var_nodeS[i] + sum_CtoV;

//             //
//             // ON GENERE LES MESSAGES SORTANTS VERS LES CNs
//             //  
//             for(int q = 0; q < length; q++)     //length = nbPariteParVariable[i];
//             {
//                 msg_in              = *(_MSG_C_2_V_ + q);
//                 *(_MSG_V_2_C + q)   = temp - msg_in;// _MSG_C_2_V_++;
//             }
//             _MSG_C_2_V_ += length; // ON PASSE AUX MESSAGES DU NOEUD SUIVANT
//             _MSG_V_2_C  += length; // ON PASSE AUX MESSAGES DU NOEUD SUIVANT
//         }
//         // FIN DE L'ITERATION SUR TOUTES LES LIGNES DE LA MATRICE

//         //
//         //
//         //
//         _TRANSPOSE  = transpose;
//         _TRANSPOSE_ = transpose;
//         for (int i = 0; i < CNodesNb; i++) 
//         {
//             length = nbVariableParParite[i];

//             R 	BPVAL[32];
//             	sign    	= 0.0;
//             R 	BPSum		= 1.0;

//             //
//             // ON ACCUMULE LES INFORMATIONS RENTRANT DANS LE CN
//             //
//             for(int q = 0; q < length; q++)/*while (length--)*/ 
//             {
//                 value       = MSG_V_2_C[*(_TRANSPOSE + q)]; //_TRANSPOSE++;
//                 vAbs        = std::abs(value);
//                 R mult      = vAbs * 0.5;
//                 R result    = std::tanh(mult);
//                 std::cout << "1. vAbs       = " << vAbs            << ", " << std::endl;
//                 std::cout << "2. mult       = " << mult            << ", " << std::endl;
//                 std::cout << "3. Tanh(mult) = " << std::tanh(mult) << ", " << std::endl;
//                 BPSum		= result * BPSum;
//                 BPVAL[q]    = result;
//                 std::cout << "4. BPSum      = " << BPSum           << ", " << std::endl;
//                 std::cout << "5. BPVAL[q]   = " << BPVAL[q]        << ", " << std::endl;
//                 cSign       = signbit(value);
//                 sign        = sign ^ cSign;
//             }

//             //
//             // ON REGENERE LES VALEURS DE SORTIE DU CN
//             //

//             //length = nbVariableParParite[i];
//             for(int q = 0; q < length; q++)
//             {
//                 value       = MSG_V_2_C[*(_TRANSPOSE_ + q)];
//                 vSig        = sign ^ signbit(value); // XOR bit

//                 R rDivis    = BPSum / BPVAL[q];
//                 R BPArg     = ((rDivis >= 1) ? 0.9999 : rDivis);   // Saturation pour éviter les infinis
//                 R ratanh    = std::atanh(BPArg);

//                 vRes        = 2.0 * ratanh;   // CMOV
//                 v2St        = std::copysign(vRes, vSig); // Magnitude of vRes, sign of vSig
//                 MSG_C_2_V[*(_TRANSPOSE_ + q)] = v2St;
//             }
//             _TRANSPOSE   += length;
//             _TRANSPOSE_  += length;
//             syndromes    =  syndromes || sign;
//         }

//         // SATURATION ??? RIGHT PLACE ?
//         for (int i = 0; i < message; i++) 
//         {
//             satTemp = MSG_C_2_V[i];
//             if (satTemp > C2V_max)
//             {
//                 MSG_C_2_V[i] = C2V_max;
//             }
//             else if (satTemp < -C2V_max)
//             {
//                 MSG_C_2_V[i] = -C2V_max;
//             }
//         }

//         //
//         // GESTION DU CRITERE D'ARRET
//         //
//         if ((syndromes == 0) && (fast_stop == 1)) 
//         {
//             //printf("Stopping Criterion Triggered\n");
//             break;
//         }
//     }

//     // DEBUT DE L'ITERATION SUR TOUTES LES LIGNES DE LA MATRICE
//     R *_MSG_C_2_V = MSG_C_2_V;
//     for (int i = 0; i < VNodesNb; i++) 
//     {
//         sum_CtoV = 0;
//         length = nbPariteParVariable[i];
//         for(int q = 0; q < length; q++)/*while (length--)*/ 
//         {
//             msg_in      = *(_MSG_C_2_V + q);
//             sum_CtoV    = sum_CtoV + msg_in;
//         }
//         _MSG_C_2_V += length;
        
//         // Output Filling
//         Rprime_fix[i] = var_nodeS[i] + sum_CtoV;
//     }

//     //
//     // FIN DU PROCESS ITERATIF DE DECODAGE
//     //
    
//     *iters          =  (iterations + initFlag);
   
//     return (syndromes == 0);
// }

// // BP-Like Algorithm (Log Sum-Product Algorithm)
// template <typename B, typename R>
// bool Decoder_LDPC_BP_naive<B,R>
// ::LogSPA_decode( 
//             const R       		  var_nodeS[],  
//             const int     		  nombre_iterations,  const int     		initFlag, 
//             	  mipp::vector<R> &C2V, 			  		mipp::vector<R> &V2C,
//             	  mipp::vector<R> &Rprime_fix,				int*    		iters
//            ) //const // Changed To allow Coset Approach in IterDEV
// {

//     //
//     // Init Variables
//     //
    
//     int     syndromes = 0;
//     int     iterations;
//     int     length;
    
//     R       sum_CtoV;
//     R       msg_in;
    
//     R*      MSG_C_2_V = C2V.data();
//     R* 		MSG_V_2_C = V2C.data();

//     R       temp;
    
//     int     sign;
    
//     const unsigned int *_TRANSPOSE  = transpose;
//     const unsigned int *_TRANSPOSE_ = transpose;
    
//     R       value;
//     int     cSign;
//     R       vAbs;
    
//     R       vRes;
//     int     vSig;
//     R       v2St;

//     R       satTemp;
    
//     //
//     // ON INITIALISE LES ZONES MEMOIRES (MESSAGES)
//     //

//     if (initFlag == 0)
//     {
//     	fill(C2V.begin(), C2V.end(), 0.0);
//     }

//     //
//     //  ACTUAL DECODING
//     //

//     for(iterations = 0; iterations < nombre_iterations; iterations += 1)
//     {
//         syndromes = 0;

//         // DEBUT DE L'ITERATION SUR TOUTES LES LIGNES DE LA MATRICE
//         R *_MSG_C_2_V_  = MSG_C_2_V;
//         R *_MSG_V_2_C   = MSG_V_2_C;

//         for (int i = 0; i < VNodesNb; i++) 
//         {

//             //
//             // LE NOEUD VN ACCUMULE TOUS LES MESSAGES ENTRANT
//             //
//             length = nbPariteParVariable[i];
//             sum_CtoV = 0.0;
//             for(int q = 0; q < length; q++)/*while (length--)*/ 
//             {
//                 msg_in      = *(_MSG_C_2_V_ + q);
//                 sum_CtoV    =  sum_CtoV + msg_in;
//             }

//             //
//             // ON MET A JOUR LA VALEUR INTERNE
//             //
//             temp = var_nodeS[i] + sum_CtoV;

//             //
//             // ON GENERE LES MESSAGES SORTANTS VERS LES CNs
//             //  
//             for(int q = 0; q < length; q++)     //length = nbPariteParVariable[i];
//             {
//                 msg_in              = *(_MSG_C_2_V_ + q);
//                 *(_MSG_V_2_C + q)   = temp - msg_in;// _MSG_C_2_V_++;
//             }
//             _MSG_C_2_V_ += length; // ON PASSE AUX MESSAGES DU NOEUD SUIVANT
//             _MSG_V_2_C  += length; // ON PASSE AUX MESSAGES DU NOEUD SUIVANT
//         }
//         // FIN DE L'ITERATION SUR TOUTES LES LIGNES DE LA MATRICE

//         //
//         //
//         //
//         _TRANSPOSE  = transpose;
//         _TRANSPOSE_ = transpose;
//         for (int i = 0; i < CNodesNb; i++) 
//         {
//             length = nbVariableParParite[i];

//             R 	BPVAL[32];
//             	sign    	= 0.0;
//             R 	BPSum		= 0.0;

//             //
//             // ON ACCUMULE LES INFORMATIONS RENTRANT DANS LE CN
//             //
//             for(int q = 0; q < length; q++)/*while (length--)*/ 
//             {
//                 value       = MSG_V_2_C[*(_TRANSPOSE + q)]; //_TRANSPOSE++;
//                 vAbs        = std::abs(value);
//                 R mult      = vAbs * 0.5;
//                 // std::cout << "1. vAbs       = " << vAbs            << ", " << std::endl;
//                 // std::cout << "2. mult       = " << mult            << ", " << std::endl;
//                 // std::cout << "3. Tanh(mult) = " << std::tanh(mult) << ", " << std::endl;
//                 R result    = std::log(std::tanh(mult));
//                 BPSum		= result + BPSum;
//                 BPVAL[q]    = result;
//                 // std::cout << "4. BPSum      = " << BPSum           << ", " << std::endl;
//                 // std::cout << "5. BPVAL[q]   = " << BPVAL[q]        << ", " << std::endl;
//                 cSign       = signbit(value);
//                 sign        = sign ^ cSign;
//             }

//             //
//             // ON REGENERE LES VALEURS DE SORTIE DU CN
//             //

//             //length = nbVariableParParite[i];
//             for(int q = 0; q < length; q++)
//             {
//                 value       = MSG_V_2_C[*(_TRANSPOSE_ + q)];
//                 vSig        = sign ^ signbit(value); // XOR bit

//                 R rDivis    = BPSum - BPVAL[q];
//                 //R BPArg     = ((rDivis > 15) ? ( 15) : rDivis);   // Saturation pour éviter les infinis
//                 //R BPArg2    = ((BPArg < -15) ? (-15) : BPArg);   // Saturation pour éviter les infinis
//                 R exp_rDiv	= std::exp(rDivis);

//                 // if (exp_rDiv > 1.0)
//                 // 	while(exp_rDiv > 1.0)
//                 // 		exp_rDiv -= 1.0;

//                 // if (exp_rDiv < -1.0)
//                 // 	while(exp_rDiv < -1.0)
//                 // 		exp_rDiv += 1.0;

//               //   if (exp_rDiv >= 1.0)
//             		// exp_rDiv = 0.9999999;

//               //   if (exp_rDiv <= -1.0)
//             		// exp_rDiv = 0.9999999;

//                 R ratanh    = std::atanh(exp_rDiv);
//                 // std::cout << "7. rDivis      = " << rDivis           << ", " << std::endl;                
//                 // std::cout << "8. exp(rDivis) = " << exp_rDiv << ", " << std::endl;                
//                 // std::cout << "9. ratanh      = " << ratanh           << ", " << std::endl;                

//                 vRes        = 2.0 * ratanh;
//                 v2St        = std::copysign(vRes, vSig); // Magnitude of vRes, sign of vSig
//                 // std::cout << "10. v2St       = " << v2St            << ", " << std::endl;                
//                 MSG_C_2_V[*(_TRANSPOSE_ + q)] = v2St;
//             }
//             _TRANSPOSE   += length;
//             _TRANSPOSE_  += length;
//             syndromes    =  syndromes || sign;
//         }

//         // SATURATION ??? RIGHT PLACE ?
//         for (int i = 0; i < message; i++) 
//         {
//             satTemp = MSG_C_2_V[i];
//             if (satTemp > C2V_max)
//             {
//                 MSG_C_2_V[i] = C2V_max;
//             }
//             else if (satTemp < -C2V_max)
//             {
//                 MSG_C_2_V[i] = -C2V_max;
//             }
//         }

//         //
//         // GESTION DU CRITERE D'ARRET
//         //
//         if ((syndromes == 0) && (fast_stop == 1)) 
//         {
//             //printf("Stopping Criterion Triggered\n");
//             break;
//         }
//     }

//     // DEBUT DE L'ITERATION SUR TOUTES LES LIGNES DE LA MATRICE
//     R *_MSG_C_2_V = MSG_C_2_V;
//     for (int i = 0; i < VNodesNb; i++) 
//     {
//         sum_CtoV = 0;
//         length = nbPariteParVariable[i];
//         for(int q = 0; q < length; q++)/*while (length--)*/ 
//         {
//             msg_in      = *(_MSG_C_2_V + q);
//             sum_CtoV    = sum_CtoV + msg_in;
//         }
//         _MSG_C_2_V += length;
        
//         // Output Filling
//         Rprime_fix[i] = var_nodeS[i] + sum_CtoV;
//     }

//     //
//     // FIN DU PROCESS ITERATIF DE DECODAGE
//     //
    
//     *iters          =  (iterations + initFlag);
   
//     return (syndromes == 0);
// }

// ==================================================================================== explicit template instantiation 
#include "../../../Tools/types.h"
#ifdef MULTI_PREC
template class Decoder_LDPC_BP_naive<B_8,Q_8>;
template class Decoder_LDPC_BP_naive<B_16,Q_16>;
template class Decoder_LDPC_BP_naive<B_32,Q_32>;
template class Decoder_LDPC_BP_naive<B_64,Q_64>;
#else
template class Decoder_LDPC_BP_naive<B,Q>;
#endif
// ==================================================================================== explicit template instantiation