#include <stdexcept>

#include "Module/Modulator/BPSK/Modulator_BPSK.hpp"
#include "Module/Modulator/BPSK/Modulator_BPSK_fast.hpp"
#include "Module/Modulator/PAM/Modulator_PAM.hpp"
#include "Module/Modulator/QAM/Modulator_QAM.hpp"
#include "Module/Modulator/PSK/Modulator_PSK.hpp"
#include "Module/Modulator/CPM/Modulator_CPM.hpp"
#include "Module/Modulator/User/Modulator_user.hpp"

#include "Factory_modulator.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B, typename R, typename Q>
template <proto_max<Q> MAX>
Modulator<B,R,Q>* Factory_modulator<B,R,Q>
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
	     if (type == "BPSK"     ) return new Modulator_BPSK     <B,R,Q    >(N, sigma,                                               no_sig2, n_frames);
	else if (type == "BPSK_FAST") return new Modulator_BPSK_fast<B,R,Q    >(N, sigma,                                               no_sig2, n_frames);
	else if (type == "PAM"      ) return new Modulator_PAM      <B,R,Q,MAX>(N, sigma, bps,                                          no_sig2, n_frames);
	else if (type == "QAM"      ) return new Modulator_QAM      <B,R,Q,MAX>(N, sigma, bps,                                          no_sig2, n_frames);
	else if (type == "PSK"      ) return new Modulator_PSK      <B,R,Q,MAX>(N, sigma, bps,                                          no_sig2, n_frames);
	else if (type == "USER"     ) return new Modulator_user     <B,R,Q,MAX>(N, sigma, bps, path,                                    no_sig2, n_frames);
	else if (type == "CPM"      ) return new Modulator_CPM      <B,R,Q,MAX>(N, sigma, bps, upf, cpm_L, cpm_k, cpm_p, mapping, wave, no_sig2, n_frames);

	return nullptr;
}

template <typename B, typename R, typename Q>
Modulator<B,R,Q>* Factory_modulator<B,R,Q>
::build(const std::string type,
        const int         N,
        const float       sigma,
        const std::string max_type,
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
	     if (max_type == "MAX"  ) return _build<max          <Q>>(type, N, sigma, bps, path, upf, cpm_L, cpm_k, cpm_p, mapping, wave, no_sig2, n_frames);
	else if (max_type == "MAXL" ) return _build<max_linear   <Q>>(type, N, sigma, bps, path, upf, cpm_L, cpm_k, cpm_p, mapping, wave, no_sig2, n_frames);
	else if (max_type == "MAXS" ) return _build<max_star     <Q>>(type, N, sigma, bps, path, upf, cpm_L, cpm_k, cpm_p, mapping, wave, no_sig2, n_frames);
	else if (max_type == "MAXSS") return _build<max_star_safe<Q>>(type, N, sigma, bps, path, upf, cpm_L, cpm_k, cpm_p, mapping, wave, no_sig2, n_frames);

	return nullptr;
}

template <typename B, typename R, typename Q>
int Factory_modulator<B,R,Q>
::get_buffer_size_after_modulation(const std::string type,
                                   const int         N,
                                   const int         bps,
                                   const int         upf,
                                   const int         cpm_L)
{
	     if (type == "BPSK"     ) return Modulator_BPSK     <B,R,Q>::size_mod(N                 );
	else if (type == "BPSK_FAST") return Modulator_BPSK_fast<B,R,Q>::size_mod(N                 );
	else if (type == "PAM"      ) return Modulator_PAM      <B,R,Q>::size_mod(N, bps            );
	else if (type == "QAM"      ) return Modulator_QAM      <B,R,Q>::size_mod(N, bps            );
	else if (type == "PSK"      ) return Modulator_PSK      <B,R,Q>::size_mod(N, bps            );
	else if (type == "USER"     ) return Modulator_user     <B,R,Q>::size_mod(N, bps            );
	else if (type == "CPM"      ) return Modulator_CPM      <B,R,Q>::size_mod(N, bps, cpm_L, upf);

	throw std::runtime_error("aff3ct::tools::Factory_modulator: unknown type of modulator.");
}

template <typename B, typename R, typename Q>
int Factory_modulator<B,R,Q>
::get_buffer_size_after_filtering(const std::string type,
                                  const int         N,
                                  const int         bps,
                                  const int         cpm_L,
                                  const int         cpm_p)
{
	     if (type == "BPSK"     ) return Modulator_BPSK     <B,R,Q>::size_fil(N                   );
	else if (type == "BPSK_FAST") return Modulator_BPSK_fast<B,R,Q>::size_fil(N                   );
	else if (type == "PAM"      ) return Modulator_PAM      <B,R,Q>::size_fil(N, bps              );
	else if (type == "QAM"      ) return Modulator_QAM      <B,R,Q>::size_fil(N, bps              );
	else if (type == "PSK"      ) return Modulator_PSK      <B,R,Q>::size_fil(N, bps              );
	else if (type == "USER"     ) return Modulator_user     <B,R,Q>::size_fil(N, bps              );
	else if (type == "CPM"      ) return Modulator_CPM      <B,R,Q>::size_fil(N, bps, cpm_L, cpm_p);

	throw std::runtime_error("aff3ct::tools::Factory_modulator: unknown type of modulator.");
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct aff3ct::tools::Factory_modulator<B_8,R_8,R_8>;
template struct aff3ct::tools::Factory_modulator<B_8,R_8,Q_8>;
template struct aff3ct::tools::Factory_modulator<B_16,R_16,R_16>;
template struct aff3ct::tools::Factory_modulator<B_16,R_16,Q_16>;
template struct aff3ct::tools::Factory_modulator<B_32,R_32,R_32>;
template struct aff3ct::tools::Factory_modulator<B_64,R_64,R_64>;
#else
template struct aff3ct::tools::Factory_modulator<B,R,Q>;
#if !defined(PREC_32_BIT) && !defined(PREC_64_BIT)
template struct aff3ct::tools::Factory_modulator<B,R,R>;
#endif
#endif
// ==================================================================================== explicit template instantiation
