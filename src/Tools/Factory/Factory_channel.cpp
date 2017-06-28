#include "Tools/Exception/exception.hpp"

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
        const bool        complex,
        const bool        add_users,
        const std::string path,
        const int         seed,
        const R           sigma,
        const int         n_frames)
{
	     if (type == "AWGN"         ) return new Channel_AWGN_LLR    <R>(N,          new Noise_std <R>(seed), add_users, sigma, n_frames);
	else if (type == "AWGN_FAST"    ) return new Channel_AWGN_LLR    <R>(N,          new Noise_fast<R>(seed), add_users, sigma, n_frames);
	else if (type == "RAYLEIGH"     ) return new Channel_Rayleigh_LLR<R>(N, complex, new Noise_std <R>(seed), add_users, sigma, n_frames);
	else if (type == "RAYLEIGH_FAST") return new Channel_Rayleigh_LLR<R>(N, complex, new Noise_fast<R>(seed), add_users, sigma, n_frames);
	else if (type == "USER"         ) return new Channel_user        <R>(N, path,                             add_users,        n_frames);
	else if (type == "NO"           ) return new Channel_NO          <R>(N,                                   add_users,        n_frames);
#ifdef CHANNEL_MKL
	else if (type == "AWGN_MKL"     ) return new Channel_AWGN_LLR    <R>(N,          new Noise_MKL <R>(seed), add_users, sigma, n_frames);
	else if (type == "RAYLEIGH_MKL" ) return new Channel_Rayleigh_LLR<R>(N, complex, new Noise_MKL <R>(seed), add_users, sigma, n_frames);
#endif
#ifdef CHANNEL_GSL
	else if (type == "AWGN_GSL"     ) return new Channel_AWGN_LLR    <R>(N,          new Noise_GSL <R>(seed), add_users, sigma, n_frames);
	else if (type == "RAYLEIGH_GSL" ) return new Channel_Rayleigh_LLR<R>(N, complex, new Noise_GSL <R>(seed), add_users, sigma, n_frames);
#endif

	throw cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename R>
void Factory_channel<R>
::build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args)
{
	// ------------------------------------------------------------------------------------------------------- channel
	std::string chan_avail = "NO, USER, AWGN, AWGN_FAST, RAYLEIGH, RAYLEIGH_FAST";
#ifdef CHANNEL_GSL
	chan_avail += ", AWGN_GSL, RAYLEIGH_GSL";
#endif
#ifdef CHANNEL_MKL
	chan_avail += ", AWGN_MKL, RAYLEIGH_MKL";
#endif

	opt_args[{"chn-type"}] =
		{"string",
		 "type of the channel to use in the simulation.",
		 chan_avail};

	opt_args[{"chn-path"}] =
		{"string",
		 "path to a noisy file, to use with \"--chn-type USER\"."};

	opt_args[{"chn-blk-fad"}] =
		{"string",
		 "block fading policy.",
		 "NO, FRAME, ONETAP"};
}

template <typename R>
void Factory_channel<R>
::store_args(const Arguments_reader& ar, channel_parameters &params)
{
	// ------------------------------------------------------------------------------------------------------- channel
	if(ar.exist_arg({"chn-type"   })) params.type         = ar.get_arg({"chn-type"   });
	if(ar.exist_arg({"chn-path"   })) params.path         = ar.get_arg({"chn-path"   });
	if(ar.exist_arg({"chn-blk-fad"})) params.block_fading = ar.get_arg({"chn-blk-fad"});
}

template <typename R>
void Factory_channel<R>
::group_args(Arguments_reader::arg_grp& ar)
{
	ar.push_back({"chn", "Channel parameter(s)"});
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
