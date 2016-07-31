#include "../../../Tools/bash_tools.h"

#include "Decoder_LDPC_BP_naive.hpp"


//
//  CONSTANTS FOR ITERATIVE DECODING ///////////////////////////////////////////////////////////////////////////////////
//

constexpr int cpm_ldpc_nb_iter_max  = 30;
constexpr int ldpc_nb_iter_max      = 1;
constexpr int simulate_BCH			= 0;	// 1 for yes else for no
constexpr int HistErr				= 0;	// 1 for yes else for no
constexpr int BCH_correction		= 32; 	// how Much bits can be corrected by BCH 


//
//  BP FUNCTIONS FOR DECODING //////////////////////////////////////////////////////////////////////////////////////////
//


/* Math Functions */

// Retrieves Second Minimum
template <typename B, typename Q>
Q Decoder_LDPC_BP_naive<B,Q>
::min_2(const Q a, const Q min1, const Q min2) //const // Changed To allow Coset Approach in IterDEV
{
    Q x = std::max(a, min1);
    return std::min(min2, x);
}

// Wrapping making sure of value returned by signbit
template <typename B, typename Q>
int Decoder_LDPC_BP_naive<B,Q>
::signbit(const Q val) //const // Changed To allow Coset Approach in IterDEV
{
    return (std::signbit(val) ? -1 : 0);
}


// BP-Like Algorithm (Normalized Offest MinSum)
template <typename B, typename Q>
bool Decoder_LDPC_BP_naive<B,Q>
::BP_decode( 
            const Q       		  var_nodeS[],  
            const int     		  nombre_iterations,  const int     		initFlag, 
            	  mipp::vector<Q> &C2V, 			  		mipp::vector<Q> &V2C,
            	  mipp::vector<Q> &Rprime_fix,				int*    		iters
           ) //const // Changed To allow Coset Approach in IterDEV
{

    //
    // Init Variables
    //
    
    int     syndromes = 0;
    int     iterations;
    int     length;
    
    Q       sum_CtoV;
    Q       msg_in;
    
    Q*      MSG_C_2_V = C2V.data();
    Q* 		MSG_V_2_C = V2C.data();

    Q       temp;
    
    int     sign;
    Q       min1;
    Q       min2;
    
    const unsigned int *_TRANSPOSE  = transpose;
    const unsigned int *_TRANSPOSE_ = transpose;
    
    Q       value;
    int     cSign;
    Q       vAbs;
    Q       vTemp;
    
    Q       cste_1;
    Q       cste_2;
    
    Q       vRes;
    int     vSig;
    Q       v2St;

    Q       satTemp;
    
    //
    // ON INITIALISE LES ZONES MEMOIRES (MESSAGES)
    //

    if (initFlag == 0)
    {
    	fill(C2V.begin(), C2V.end(), 0);
    }

    //
    //  ACTUAL DECODING
    //

    for(iterations = 0; iterations < nombre_iterations; iterations += 1)
    {
        syndromes = 0;

        // DEBUT DE L'ITERATION SUR TOUTES LES LIGNES DE LA MATRICE
        Q *_MSG_C_2_V_  = MSG_C_2_V;
        Q *_MSG_V_2_C   = MSG_V_2_C;

        for (int i = 0; i < VNodesNb; i++) 
        {

            //
            // LE NOEUD VN ACCUMULE TOUS LES MESSAGES ENTRANT
            //
            length = nbPariteParVariable[i];
            sum_CtoV = 0;
            for(int q = 0; q < length; q++)/*while (length--)*/ 
            {
                msg_in      = *(_MSG_C_2_V_ + q);
                sum_CtoV    =  sum_CtoV + msg_in;
            }

            //
            // ON MET A JOUR LA VALEUR INTERNE
            //
            temp = var_nodeS[i] + sum_CtoV;

            //
            // ON GENERE LES MESSAGES SORTANTS VERS LES CNs
            //  
            for(int q = 0; q < length; q++)     //length = nbPariteParVariable[i];
            {
                msg_in              = *(_MSG_C_2_V_ + q);
                *(_MSG_V_2_C + q)   = temp - msg_in;// _MSG_C_2_V_++;
            }
            _MSG_C_2_V_ += length; // ON PASSE AUX MESSAGES DU NOEUD SUIVANT
            _MSG_V_2_C  += length; // ON PASSE AUX MESSAGES DU NOEUD SUIVANT
        }
        // FIN DE L'ITERATION SUR TOUTES LES LIGNES DE LA MATRICE

        //
        //
        //
        _TRANSPOSE  = transpose;
        _TRANSPOSE_ = transpose;
        for (int i = 0; i < CNodesNb; i++) 
        {
            length = nbVariableParParite[i];

            sign    = 0;
            min1    = std::numeric_limits<Q>::infinity();
            min2    = std::numeric_limits<Q>::infinity();

            //
            // ON ACCUMULE LES INFORMATIONS RENTRANT DANS LE CN
            //
            for(int q = 0; q < length; q++)/*while (length--)*/ 
            {
                value       = MSG_V_2_C[*(_TRANSPOSE + q)]; //_TRANSPOSE++;
                cSign       = signbit(value);
                vAbs        = std::abs(value);//std::trunc(std::abs(value));
                sign        = sign ^ cSign;
                vTemp       = min1;
                min1        = (vAbs < min1) ?  vAbs : min1;     // MIN
                min2        = min_2(vAbs, vTemp, min2);     // 2nd MIN
            }

            //
            // ON CALCULE LES VALEURS INTERNE DU CN
            //

            cste_1 = min2 - offset;
            cste_2 = min1 - offset;
            cste_1 = cste_1 * normalize_1;
            cste_2 = cste_2 * normalize_2;
            cste_1 = (cste_1 < 0) ?  0 : cste_1; 
            cste_2 = (cste_2 < 0) ?  0 : cste_2; 

            //
            // ON REGENERE LES VALEURS DE SORTIE DU CN
            //

            //length = nbVariableParParite[i];
            for(int q = 0; q < length; q++)
            {
                value       = MSG_V_2_C[*(_TRANSPOSE_ + q)];
                vAbs        = std::abs(value);//std::trunc(std::abs(value));
                vRes        = ((vAbs == min1) ? cste_1 : cste_2);   // CMOV
                vSig        = sign ^ signbit(value); // XOR bit
                v2St        = std::copysign(vRes, vSig); // Magnitude of vRes, sign of vSig
                MSG_C_2_V[*(_TRANSPOSE_ + q)] = v2St;
            }
            _TRANSPOSE   += length;
            _TRANSPOSE_  += length;
            syndromes    =  syndromes || sign;
        }

        // SATURATION ??? RIGHT PLACE ?
        for (int i = 0; i < message; i++) 
        {
            satTemp = MSG_C_2_V[i];
            if (satTemp > C2V_max)
            {
                MSG_C_2_V[i] = C2V_max;
            }
            else if (satTemp < -C2V_max)
            {
                MSG_C_2_V[i] = -C2V_max;
            }
        }

        //
        // GESTION DU CRITERE D'ARRET
        //
        if ((syndromes == 0) && (fast_stop == 1)) 
        {
            //printf("Stopping Criterion Triggered\n");
            break;
        }
    }

    // DEBUT DE L'ITERATION SUR TOUTES LES LIGNES DE LA MATRICE
    Q *_MSG_C_2_V = MSG_C_2_V;
    for (int i = 0; i < VNodesNb; i++) 
    {
        sum_CtoV = 0;
        length = nbPariteParVariable[i];
        for(int q = 0; q < length; q++)/*while (length--)*/ 
        {
            msg_in      = *(_MSG_C_2_V + q);
            sum_CtoV    = sum_CtoV + msg_in;
        }
        _MSG_C_2_V += length;
        
        // Output Filling
        Rprime_fix[i] = var_nodeS[i] + sum_CtoV;
    }

    //
    // FIN DU PROCESS ITERATIF DE DECODAGE
    //
    
    *iters          =  (iterations + initFlag);
   
    return (syndromes == 0);
}


// BP-Like Algorithm (Sum-Product Algorithm)
template <typename B, typename Q>
bool Decoder_LDPC_BP_naive<B,Q>
::SPA_decode( 
            const Q       		  var_nodeS[],  
            const int     		  nombre_iterations,  const int     		initFlag, 
            	  mipp::vector<Q> &C2V, 			  		mipp::vector<Q> &V2C,
            	  mipp::vector<Q> &Rprime_fix,				int*    		iters
           ) //const // Changed To allow Coset Approach in IterDEV
{

    //
    // Init Variables
    //
    
    int     syndromes = 0;
    int     iterations;
    int     length;
    
    Q       sum_CtoV;
    Q       msg_in;
    
    Q*      MSG_C_2_V = C2V.data();
    Q* 		MSG_V_2_C = V2C.data();

    Q       temp;
    
    int     sign;
    
    const unsigned int *_TRANSPOSE  = transpose;
    const unsigned int *_TRANSPOSE_ = transpose;
    
    Q       value;
    int     cSign;
    Q       vAbs;
    
    Q       vRes;
    int     vSig;
    Q       v2St;

    Q       satTemp;
    
    //
    // ON INITIALISE LES ZONES MEMOIRES (MESSAGES)
    //

    if (initFlag == 0)
    {
    	fill(C2V.begin(), C2V.end(), 0.0);
    }

    //
    //  ACTUAL DECODING
    //

    for(iterations = 0; iterations < nombre_iterations; iterations += 1)
    {
        syndromes = 0;

        // DEBUT DE L'ITERATION SUR TOUTES LES LIGNES DE LA MATRICE
        Q *_MSG_C_2_V_  = MSG_C_2_V;
        Q *_MSG_V_2_C   = MSG_V_2_C;

        for (int i = 0; i < VNodesNb; i++) 
        {

            //
            // LE NOEUD VN ACCUMULE TOUS LES MESSAGES ENTRANT
            //
            length = nbPariteParVariable[i];
            sum_CtoV = 0;
            for(int q = 0; q < length; q++)/*while (length--)*/ 
            {
                msg_in      = *(_MSG_C_2_V_ + q);
                sum_CtoV    =  sum_CtoV + msg_in;
            }

            //
            // ON MET A JOUR LA VALEUR INTERNE
            //
            temp = var_nodeS[i] + sum_CtoV;

            //
            // ON GENERE LES MESSAGES SORTANTS VERS LES CNs
            //  
            for(int q = 0; q < length; q++)     //length = nbPariteParVariable[i];
            {
                msg_in              = *(_MSG_C_2_V_ + q);
                *(_MSG_V_2_C + q)   = temp - msg_in;// _MSG_C_2_V_++;
            }
            _MSG_C_2_V_ += length; // ON PASSE AUX MESSAGES DU NOEUD SUIVANT
            _MSG_V_2_C  += length; // ON PASSE AUX MESSAGES DU NOEUD SUIVANT
        }
        // FIN DE L'ITERATION SUR TOUTES LES LIGNES DE LA MATRICE

        //
        //
        //
        _TRANSPOSE  = transpose;
        _TRANSPOSE_ = transpose;
        for (int i = 0; i < CNodesNb; i++) 
        {
            length = nbVariableParParite[i];

            Q 	BPVAL[32];
            	sign    	= 0.0;
            Q 	BPSum		= 1.0;

            //
            // ON ACCUMULE LES INFORMATIONS RENTRANT DANS LE CN
            //
            for(int q = 0; q < length; q++)/*while (length--)*/ 
            {
                value       = MSG_V_2_C[*(_TRANSPOSE + q)]; //_TRANSPOSE++;
                vAbs        = std::abs(value);
                Q mult      = vAbs * 0.5;
                Q result    = std::tanh(mult);
                std::cout << "1. vAbs       = " << vAbs            << ", " << std::endl;
                std::cout << "2. mult       = " << mult            << ", " << std::endl;
                std::cout << "3. Tanh(mult) = " << std::tanh(mult) << ", " << std::endl;
                BPSum		= result * BPSum;
                BPVAL[q]    = result;
                std::cout << "4. BPSum      = " << BPSum           << ", " << std::endl;
                std::cout << "5. BPVAL[q]   = " << BPVAL[q]        << ", " << std::endl;
                cSign       = signbit(value);
                sign        = sign ^ cSign;
            }

            //
            // ON REGENERE LES VALEURS DE SORTIE DU CN
            //

            //length = nbVariableParParite[i];
            for(int q = 0; q < length; q++)
            {
                value       = MSG_V_2_C[*(_TRANSPOSE_ + q)];
                vSig        = sign ^ signbit(value); // XOR bit

                Q rDivis    = BPSum / BPVAL[q];
                Q BPArg     = ((rDivis >= 1) ? 0.9999 : rDivis);   // Saturation pour éviter les infinis
                Q ratanh    = std::atanh(BPArg);

                vRes        = 2.0 * ratanh;   // CMOV
                v2St        = std::copysign(vRes, vSig); // Magnitude of vRes, sign of vSig
                MSG_C_2_V[*(_TRANSPOSE_ + q)] = v2St;
            }
            _TRANSPOSE   += length;
            _TRANSPOSE_  += length;
            syndromes    =  syndromes || sign;
        }

        // SATURATION ??? RIGHT PLACE ?
        for (int i = 0; i < message; i++) 
        {
            satTemp = MSG_C_2_V[i];
            if (satTemp > C2V_max)
            {
                MSG_C_2_V[i] = C2V_max;
            }
            else if (satTemp < -C2V_max)
            {
                MSG_C_2_V[i] = -C2V_max;
            }
        }

        //
        // GESTION DU CRITERE D'ARRET
        //
        if ((syndromes == 0) && (fast_stop == 1)) 
        {
            //printf("Stopping Criterion Triggered\n");
            break;
        }
    }

    // DEBUT DE L'ITERATION SUR TOUTES LES LIGNES DE LA MATRICE
    Q *_MSG_C_2_V = MSG_C_2_V;
    for (int i = 0; i < VNodesNb; i++) 
    {
        sum_CtoV = 0;
        length = nbPariteParVariable[i];
        for(int q = 0; q < length; q++)/*while (length--)*/ 
        {
            msg_in      = *(_MSG_C_2_V + q);
            sum_CtoV    = sum_CtoV + msg_in;
        }
        _MSG_C_2_V += length;
        
        // Output Filling
        Rprime_fix[i] = var_nodeS[i] + sum_CtoV;
    }

    //
    // FIN DU PROCESS ITERATIF DE DECODAGE
    //
    
    *iters          =  (iterations + initFlag);
   
    return (syndromes == 0);
}

// BP-Like Algorithm (Log Sum-Product Algorithm)
template <typename B, typename Q>
bool Decoder_LDPC_BP_naive<B,Q>
::LogSPA_decode( 
            const Q       		  var_nodeS[],  
            const int     		  nombre_iterations,  const int     		initFlag, 
            	  mipp::vector<Q> &C2V, 			  		mipp::vector<Q> &V2C,
            	  mipp::vector<Q> &Rprime_fix,				int*    		iters
           ) //const // Changed To allow Coset Approach in IterDEV
{

    //
    // Init Variables
    //
    
    int     syndromes = 0;
    int     iterations;
    int     length;
    
    Q       sum_CtoV;
    Q       msg_in;
    
    Q*      MSG_C_2_V = C2V.data();
    Q* 		MSG_V_2_C = V2C.data();

    Q       temp;
    
    int     sign;
    
    const unsigned int *_TRANSPOSE  = transpose;
    const unsigned int *_TRANSPOSE_ = transpose;
    
    Q       value;
    int     cSign;
    Q       vAbs;
    
    Q       vRes;
    int     vSig;
    Q       v2St;

    Q       satTemp;
    
    //
    // ON INITIALISE LES ZONES MEMOIRES (MESSAGES)
    //

    if (initFlag == 0)
    {
    	fill(C2V.begin(), C2V.end(), 0.0);
    }

    //
    //  ACTUAL DECODING
    //

    for(iterations = 0; iterations < nombre_iterations; iterations += 1)
    {
        syndromes = 0;

        // DEBUT DE L'ITERATION SUR TOUTES LES LIGNES DE LA MATRICE
        Q *_MSG_C_2_V_  = MSG_C_2_V;
        Q *_MSG_V_2_C   = MSG_V_2_C;

        for (int i = 0; i < VNodesNb; i++) 
        {

            //
            // LE NOEUD VN ACCUMULE TOUS LES MESSAGES ENTRANT
            //
            length = nbPariteParVariable[i];
            sum_CtoV = 0.0;
            for(int q = 0; q < length; q++)/*while (length--)*/ 
            {
                msg_in      = *(_MSG_C_2_V_ + q);
                sum_CtoV    =  sum_CtoV + msg_in;
            }

            //
            // ON MET A JOUR LA VALEUR INTERNE
            //
            temp = var_nodeS[i] + sum_CtoV;

            //
            // ON GENERE LES MESSAGES SORTANTS VERS LES CNs
            //  
            for(int q = 0; q < length; q++)     //length = nbPariteParVariable[i];
            {
                msg_in              = *(_MSG_C_2_V_ + q);
                *(_MSG_V_2_C + q)   = temp - msg_in;// _MSG_C_2_V_++;
            }
            _MSG_C_2_V_ += length; // ON PASSE AUX MESSAGES DU NOEUD SUIVANT
            _MSG_V_2_C  += length; // ON PASSE AUX MESSAGES DU NOEUD SUIVANT
        }
        // FIN DE L'ITERATION SUR TOUTES LES LIGNES DE LA MATRICE

        //
        //
        //
        _TRANSPOSE  = transpose;
        _TRANSPOSE_ = transpose;
        for (int i = 0; i < CNodesNb; i++) 
        {
            length = nbVariableParParite[i];

            Q 	BPVAL[32];
            	sign    	= 0.0;
            Q 	BPSum		= 0.0;

            //
            // ON ACCUMULE LES INFORMATIONS RENTRANT DANS LE CN
            //
            for(int q = 0; q < length; q++)/*while (length--)*/ 
            {
                value       = MSG_V_2_C[*(_TRANSPOSE + q)]; //_TRANSPOSE++;
                vAbs        = std::abs(value);
                Q mult      = vAbs * 0.5;
                // std::cout << "1. vAbs       = " << vAbs            << ", " << std::endl;
                // std::cout << "2. mult       = " << mult            << ", " << std::endl;
                // std::cout << "3. Tanh(mult) = " << std::tanh(mult) << ", " << std::endl;
                Q result    = std::log(std::tanh(mult));
                BPSum		= result + BPSum;
                BPVAL[q]    = result;
                // std::cout << "4. BPSum      = " << BPSum           << ", " << std::endl;
                // std::cout << "5. BPVAL[q]   = " << BPVAL[q]        << ", " << std::endl;
                cSign       = signbit(value);
                sign        = sign ^ cSign;
            }

            //
            // ON REGENERE LES VALEURS DE SORTIE DU CN
            //

            //length = nbVariableParParite[i];
            for(int q = 0; q < length; q++)
            {
                value       = MSG_V_2_C[*(_TRANSPOSE_ + q)];
                vSig        = sign ^ signbit(value); // XOR bit

                Q rDivis    = BPSum - BPVAL[q];
                //R BPArg     = ((rDivis > 15) ? ( 15) : rDivis);   // Saturation pour éviter les infinis
                //R BPArg2    = ((BPArg < -15) ? (-15) : BPArg);   // Saturation pour éviter les infinis
                Q exp_rDiv	= std::exp(rDivis);

                // if (exp_rDiv > 1.0)
                // 	while(exp_rDiv > 1.0)
                // 		exp_rDiv -= 1.0;

                // if (exp_rDiv < -1.0)
                // 	while(exp_rDiv < -1.0)
                // 		exp_rDiv += 1.0;

              //   if (exp_rDiv >= 1.0)
            		// exp_rDiv = 0.9999999;

              //   if (exp_rDiv <= -1.0)
            		// exp_rDiv = 0.9999999;

                Q ratanh    = std::atanh(exp_rDiv);
                // std::cout << "7. rDivis      = " << rDivis           << ", " << std::endl;                
                // std::cout << "8. exp(rDivis) = " << exp_rDiv << ", " << std::endl;                
                // std::cout << "9. ratanh      = " << ratanh           << ", " << std::endl;                

                vRes        = 2.0 * ratanh;
                v2St        = std::copysign(vRes, vSig); // Magnitude of vRes, sign of vSig
                // std::cout << "10. v2St       = " << v2St            << ", " << std::endl;                
                MSG_C_2_V[*(_TRANSPOSE_ + q)] = v2St;
            }
            _TRANSPOSE   += length;
            _TRANSPOSE_  += length;
            syndromes    =  syndromes || sign;
        }

        // SATURATION ??? RIGHT PLACE ?
        for (int i = 0; i < message; i++) 
        {
            satTemp = MSG_C_2_V[i];
            if (satTemp > C2V_max)
            {
                MSG_C_2_V[i] = C2V_max;
            }
            else if (satTemp < -C2V_max)
            {
                MSG_C_2_V[i] = -C2V_max;
            }
        }

        //
        // GESTION DU CRITERE D'ARRET
        //
        if ((syndromes == 0) && (fast_stop == 1)) 
        {
            //printf("Stopping Criterion Triggered\n");
            break;
        }
    }

    // DEBUT DE L'ITERATION SUR TOUTES LES LIGNES DE LA MATRICE
    Q *_MSG_C_2_V = MSG_C_2_V;
    for (int i = 0; i < VNodesNb; i++) 
    {
        sum_CtoV = 0;
        length = nbPariteParVariable[i];
        for(int q = 0; q < length; q++)/*while (length--)*/ 
        {
            msg_in      = *(_MSG_C_2_V + q);
            sum_CtoV    = sum_CtoV + msg_in;
        }
        _MSG_C_2_V += length;
        
        // Output Filling
        Rprime_fix[i] = var_nodeS[i] + sum_CtoV;
    }

    //
    // FIN DU PROCESS ITERATIF DE DECODAGE
    //
    
    *iters          =  (iterations + initFlag);
   
    return (syndromes == 0);
}


//
//  SATURATION FUNCTION ////////////////////////////////////////////////////////////////////////////////////////////
//

//
template <typename B, typename Q>
void Decoder_LDPC_BP_naive<B,Q>
::saturate(mipp::vector<Q> &array, Q saturation) //const // Changed To allow Coset Approach in IterDEV
{
    unsigned int ii;
    for(ii = 0; ii < array.size(); ii++)
    {
        if(array[ii] > saturation)
        {
            array[ii] = saturation;
        }
        if (array[ii] < -saturation)
        {
            array[ii] = -saturation;
        }
    }
}


//
//  PUBLIC METHODS /////////////////////////////////////////////////////////////////////////////////////////////////////
//

template <typename B, typename Q>
Decoder_LDPC_BP_naive<B,Q>
::Decoder_LDPC_BP_naive(const int &N, const int &K, const int& n_ite)
: Decoder<B,Q>(), 
  Y_N(N), 
  V_K(K),   
  n_ite(n_ite),
  APPllr(N, 0),
  CtoV(MESSAGE, 0),
  VtoC(MESSAGE, 0)
{
	// TODO: implement the BP algo in order to remove this assertion
	assert(N == K);
}

template <typename B, typename Q>
Decoder_LDPC_BP_naive<B,Q>
::~Decoder_LDPC_BP_naive()
{
}

template <typename B, typename Q>
void Decoder_LDPC_BP_naive<B,Q>
::decode(const mipp::vector<Q> &sys, const mipp::vector<Q> &par, mipp::vector<Q> &ext)
{
	std::cerr << bold_red("(EE) This decoder does not support this interface.") << std::endl;
	std::exit(-1);
}

template <typename B, typename Q>
void Decoder_LDPC_BP_naive<B,Q>
::decode(const mipp::vector<Q> &Y_N1, mipp::vector<Q> &Y_N2 )
{
	assert(Y_N1.size() == Y_N2.size());


 	// // Before BP Values Testing:
	// std::cout << "================ BEFORE BP ================" << std::endl << std::endl;
	// std::cout << std::setprecision(8);
	// for(unsigned int rr = 0; rr < this->APPllr.size(); rr++)
	// {
	// 	std::cout << this->Y_N1[rr] << ", " << std::endl;
	// }
 	//  std::cout << std::endl;


 	//    // CtoV Values Testing:
	// std::cout << "================ CtoV Before ================" << std::endl << std::endl;
	// std::cout << std::setprecision(8);
	// for(unsigned int rr = 0; rr < this->CtoV.size(); rr++)
	// {
	// 	std::cout << this->CtoV[rr] << ", " << std::endl;
	// }
 	//  std::cout << std::endl;


    /* Actual Decoding */
    this->BP_decode(  
            /* Inputs */    Y_N1.data(),       
                            ldpc_nb_iter_max,         this->iter_cnt, 
            /* Outputs */   this->CtoV,      		  this->VtoC,
                            this->APPllr,      		  &this->ldpc_iter
           );


	// // CtoV Values Testing:
  	// std::cout << "================ CtoV After ================" << std::endl << std::endl;
	// std::cout << std::setprecision(8);
	// for(unsigned int rr = 0; rr < this->CtoV.size(); rr++)
	// {
	// 	std::cout << this->CtoV[rr] << ", " << std::endl;
	// }
	// std::cout << std::endl;


	// // BP Values Testing:
  	// std::cout << "================ BP ================" << std::endl << std::endl;
	// std::cout << std::setprecision(8);
	// for(unsigned int rr = 0; rr < this->APPllr.size(); rr++)
	// {
	// 	std::cout << this->APPllr[rr] << ", " << std::endl;
	// }
	// std::cout << std::endl;


    // Prepare for next round by processing extrinsic information
    for(unsigned int tt = 0; tt < Y_N2.size(); tt++)
    {
        Y_N2[tt] = this->APPllr[tt] - Y_N1[tt];
    }

    this->saturate(Y_N2, 15.f);

    this->iter_cnt = this->iter_cnt + 1;
}

template <typename B, typename Q>
void Decoder_LDPC_BP_naive<B,Q>
::load(const mipp::vector<Q>& Y_N)
{
	assert(Y_N.size() == this->Y_N.size());
	this->Y_N = Y_N;
}

template <typename B, typename Q>
void Decoder_LDPC_BP_naive<B,Q>
::decode()
{
    // Process updating APPllr in this function
    this->decode(Y_N, Y_Nbis);

	for (unsigned i = 0; i < V_K.size(); i++)
		V_K[i] = this->APPllr[i] < 0;
}

template <typename B, typename Q>
void Decoder_LDPC_BP_naive<B,Q>
::store(mipp::vector<B>& V_K) const
{
	assert(V_K.size() == this->V_K.size());
	V_K = this->V_K;
}

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