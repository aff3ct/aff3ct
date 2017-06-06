#include <stdexcept>

#include "Module/Modem/BPSK/Modem_BPSK.hpp"
#include "Module/Modem/BPSK/Modem_BPSK_fast.hpp"
#include "Module/Modem/PAM/Modem_PAM.hpp"
#include "Module/Modem/QAM/Modem_QAM.hpp"
#include "Module/Modem/PSK/Modem_PSK.hpp"
#include "Module/Modem/CPM/Modem_CPM.hpp"
#include "Module/Modem/SCMA/Modem_SCMA.hpp"
#include "Module/Modem/User/Modem_user.hpp"

#include "Tools/Code/SCMA/modem_SCMA_functions.hpp"

#include "Factory_modem.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B, typename R, typename Q>
template <proto_max<Q> MAX>
Modem<B,R,Q>* Factory_modem<B,R,Q>
::_build(const std::string type,
         const int         N,
         const float       sigma,
         const int         bps,
         const std::string path,
         const int         upf,
         const int         cpm_L,
         const int         cpm_k,
         const int         cpm_p,
         const std::string mapping,
         const std::string wave,
         const bool        no_sig2,
         const int         n_frames)
{
	     if (type == "BPSK"     ) return new Modem_BPSK     <B,R,Q    >(N, sigma,                                               no_sig2, n_frames);
	else if (type == "BPSK_FAST") return new Modem_BPSK_fast<B,R,Q    >(N, sigma,                                               no_sig2, n_frames);
	else if (type == "PAM"      ) return new Modem_PAM      <B,R,Q,MAX>(N, sigma, bps,                                          no_sig2, n_frames);
	else if (type == "QAM"      ) return new Modem_QAM      <B,R,Q,MAX>(N, sigma, bps,                                          no_sig2, n_frames);
	else if (type == "PSK"      ) return new Modem_PSK      <B,R,Q,MAX>(N, sigma, bps,                                          no_sig2, n_frames);
	else if (type == "USER"     ) return new Modem_user     <B,R,Q,MAX>(N, sigma, bps, path,                                    no_sig2, n_frames);
	else if (type == "CPM"      ) return new Modem_CPM      <B,R,Q,MAX>(N, sigma, bps, upf, cpm_L, cpm_k, cpm_p, mapping, wave, no_sig2, n_frames);

	throw std::runtime_error("aff3ct::tools::Factory_modem: the factory could not allocate the object.");
}

template <typename B, typename R, typename Q>
Modem<B,R,Q>* Factory_modem<B,R,Q>
::_build_scma(const std::string type,
              const int         N,
              const float       sigma,
              const int         bps,
              const std::string psi_type,
              const bool        no_sig2,
              const int         n_ite,
              const int         n_frames)
{
	     if (psi_type == "PSI0") return new Modem_SCMA <B,R,Q,psi_0<Q>>(N, sigma, bps, no_sig2, n_ite, n_frames);
	else if (psi_type == "PSI1") return new Modem_SCMA <B,R,Q,psi_1<Q>>(N, sigma, bps, no_sig2, n_ite, n_frames);
	else if (psi_type == "PSI2") return new Modem_SCMA <B,R,Q,psi_2<Q>>(N, sigma, bps, no_sig2, n_ite, n_frames);
	else if (psi_type == "PSI3") return new Modem_SCMA <B,R,Q,psi_3<Q>>(N, sigma, bps, no_sig2, n_ite, n_frames);

	throw std::runtime_error("aff3ct::tools::Factory_modem: the factory could not allocate the object.");
}

template <typename B, typename R, typename Q>
Modem<B,R,Q>* Factory_modem<B,R,Q>
::build(const std::string type,
        const int         N,
        const float       sigma,
        const std::string max_type,
        const std::string psi_type,
        const int         bps,
        const std::string path,
        const int         upf,
        const int         cpm_L,
        const int         cpm_k,
        const int         cpm_p,
        const std::string mapping,
        const std::string wave,
        const bool        no_sig2,
        const int         n_ite,
        const int         n_frames)
{
	if (type == "SCMA")
	{
		return _build_scma(type, N, sigma, bps, psi_type, no_sig2, n_ite, n_frames);
	}
	else
	{
		     if (max_type == "MAX"  ) return _build<max          <Q>>(type, N, sigma, bps, path, upf, cpm_L, cpm_k, cpm_p, mapping, wave, no_sig2, n_frames);
		else if (max_type == "MAXL" ) return _build<max_linear   <Q>>(type, N, sigma, bps, path, upf, cpm_L, cpm_k, cpm_p, mapping, wave, no_sig2, n_frames);
		else if (max_type == "MAXS" ) return _build<max_star     <Q>>(type, N, sigma, bps, path, upf, cpm_L, cpm_k, cpm_p, mapping, wave, no_sig2, n_frames);
		else if (max_type == "MAXSS") return _build<max_star_safe<Q>>(type, N, sigma, bps, path, upf, cpm_L, cpm_k, cpm_p, mapping, wave, no_sig2, n_frames);
	}

	throw std::runtime_error("aff3ct::tools::Factory_modem: the factory could not allocate the object.");
}

template <typename B, typename R, typename Q>
int Factory_modem<B,R,Q>
::get_buffer_size_after_modulation(const std::string type,
                                   const int         N,
                                   const int         bps,
                                   const int         upf,
                                   const int         cpm_L)
{
	     if (type == "BPSK"     ) return Modem_BPSK     <B,R,Q>::size_mod(N                 );
	else if (type == "BPSK_FAST") return Modem_BPSK_fast<B,R,Q>::size_mod(N                 );
	else if (type == "SCMA"     ) return Modem_SCMA     <B,R,Q>::size_mod(N, bps            );
	else if (type == "PAM"      ) return Modem_PAM      <B,R,Q>::size_mod(N, bps            );
	else if (type == "QAM"      ) return Modem_QAM      <B,R,Q>::size_mod(N, bps            );
	else if (type == "PSK"      ) return Modem_PSK      <B,R,Q>::size_mod(N, bps            );
	else if (type == "USER"     ) return Modem_user     <B,R,Q>::size_mod(N, bps            );
	else if (type == "CPM"      ) return Modem_CPM      <B,R,Q>::size_mod(N, bps, cpm_L, upf);

	throw std::runtime_error("aff3ct::tools::Factory_modem: unknown type of modem.");
}

template <typename B, typename R, typename Q>
int Factory_modem<B,R,Q>
::get_buffer_size_after_filtering(const std::string type,
                                  const int         N,
                                  const int         bps,
                                  const int         cpm_L,
                                  const int         cpm_p)
{
	     if (type == "BPSK"     ) return Modem_BPSK     <B,R,Q>::size_fil(N                   );
	else if (type == "BPSK_FAST") return Modem_BPSK_fast<B,R,Q>::size_fil(N                   );
	else if (type == "SCMA"     ) return Modem_SCMA     <B,R,Q>::size_fil(N, bps              );
	else if (type == "PAM"      ) return Modem_PAM      <B,R,Q>::size_fil(N, bps              );
	else if (type == "QAM"      ) return Modem_QAM      <B,R,Q>::size_fil(N, bps              );
	else if (type == "PSK"      ) return Modem_PSK      <B,R,Q>::size_fil(N, bps              );
	else if (type == "USER"     ) return Modem_user     <B,R,Q>::size_fil(N, bps              );
	else if (type == "CPM"      ) return Modem_CPM      <B,R,Q>::size_fil(N, bps, cpm_L, cpm_p);

	throw std::runtime_error("aff3ct::tools::Factory_modem: unknown type of modem.");
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct aff3ct::tools::Factory_modem<B_8,R_8,R_8>;
template struct aff3ct::tools::Factory_modem<B_8,R_8,Q_8>;
template struct aff3ct::tools::Factory_modem<B_16,R_16,R_16>;
template struct aff3ct::tools::Factory_modem<B_16,R_16,Q_16>;
template struct aff3ct::tools::Factory_modem<B_32,R_32,R_32>;
template struct aff3ct::tools::Factory_modem<B_64,R_64,R_64>;
#else
template struct aff3ct::tools::Factory_modem<B,R,Q>;
#if !defined(PREC_32_BIT) && !defined(PREC_64_BIT)
template struct aff3ct::tools::Factory_modem<B,R,R>;
#endif
#endif
// ==================================================================================== explicit template instantiation
