#include "Module/Channel/NO/Channel_NO.hpp"
#include "Module/Channel/User/Channel_user.hpp"
#include "Module/Channel/AWGN/Channel_AWGN_LLR.hpp"
#include "Module/Channel/Rayleigh/Channel_Rayleigh_LLR.hpp"

#include "Tools/Algo/Noise/Standard/Noise_std.hpp"
#include "Tools/Algo/Noise/Fast/Noise_fast.hpp"
#ifdef CHANNEL_MKL
#include "Tools/Algo/Noise/MKL/Noise_MKL.hpp"
#endif
#ifdef CHANNEL_GSL
#include "Tools/Algo/Noise/GSL/Noise_GSL.hpp"
#endif

#include "Factory_channel.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename R>	
Channel<R>* Factory_channel<R>
::build(const std::string type,
        const int         N,
        const R           sigma,
        const bool        complex,
        const std::string path,
        const int         seed,
        const int         n_frames)
{
	     if (type == "AWGN"         ) return new Channel_AWGN_LLR    <R>(N, sigma,          new tools::Noise_std <R>(seed), n_frames);
	else if (type == "AWGN_FAST"    ) return new Channel_AWGN_LLR    <R>(N, sigma,          new tools::Noise_fast<R>(seed), n_frames);
	else if (type == "RAYLEIGH"     ) return new Channel_Rayleigh_LLR<R>(N, sigma, complex, new tools::Noise_std <R>(seed), n_frames);
	else if (type == "RAYLEIGH_FAST") return new Channel_Rayleigh_LLR<R>(N, sigma, complex, new tools::Noise_fast<R>(seed), n_frames);
	else if (type == "USER"         ) return new Channel_user        <R>(N, path,                                           n_frames);
	else if (type == "NO"           ) return new Channel_NO          <R>(N,                                                 n_frames);
#ifdef CHANNEL_MKL
	else if (type == "AWGN_MKL"     ) return new Channel_AWGN_LLR    <R>(N, sigma,          new tools::Noise_MKL <R>(seed), n_frames);
	else if (type == "RAYLEIGH_MKL" ) return new Channel_Rayleigh_LLR<R>(N, sigma, complex, new tools::Noise_MKL <R>(seed), n_frames);
#endif
#ifdef CHANNEL_GSL
	else if (type == "AWGN_GSL"     ) return new Channel_AWGN_LLR    <R>(N, sigma,          new tools::Noise_GSL <R>(seed), n_frames);
	else if (type == "RAYLEIGH_GSL" ) return new Channel_Rayleigh_LLR<R>(N, sigma, complex, new tools::Noise_GSL <R>(seed), n_frames);
#endif

	return nullptr;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct aff3ct::tools::Factory_channel<R_32>;
template struct aff3ct::tools::Factory_channel<R_64>;
#else
template struct aff3ct::tools::Factory_channel<R>;
#endif
// ==================================================================================== explicit template instantiation
