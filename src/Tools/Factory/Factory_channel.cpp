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
Channel<R>* Factory_channel
::build(const parameters &params,
        const int         seed,
        const R           sigma)
{
	     if (params.type == "AWGN"         ) return new Channel_AWGN_LLR    <R>(params.N,                 new Noise_std <R>(seed), params.add_users, sigma, params.n_frames);
	else if (params.type == "AWGN_FAST"    ) return new Channel_AWGN_LLR    <R>(params.N,                 new Noise_fast<R>(seed), params.add_users, sigma, params.n_frames);
	else if (params.type == "RAYLEIGH"     ) return new Channel_Rayleigh_LLR<R>(params.N, params.complex, new Noise_std <R>(seed), params.add_users, sigma, params.n_frames);
	else if (params.type == "RAYLEIGH_FAST") return new Channel_Rayleigh_LLR<R>(params.N, params.complex, new Noise_fast<R>(seed), params.add_users, sigma, params.n_frames);
	else if (params.type == "USER"         ) return new Channel_user        <R>(params.N, params.path,                             params.add_users,        params.n_frames);
	else if (params.type == "NO"           ) return new Channel_NO          <R>(params.N,                                          params.add_users,        params.n_frames);
#ifdef CHANNEL_MKL
	else if (params.type == "AWGN_MKL"     ) return new Channel_AWGN_LLR    <R>(params.N,                 new Noise_MKL <R>(seed), params.add_users, sigma, params.n_frames);
	else if (params.type == "RAYLEIGH_MKL" ) return new Channel_Rayleigh_LLR<R>(params.N, params.complex, new Noise_MKL <R>(seed), params.add_users, sigma, params.n_frames);
#endif
#ifdef CHANNEL_GSL
	else if (params.type == "AWGN_GSL"     ) return new Channel_AWGN_LLR    <R>(params.N,                 new Noise_GSL <R>(seed), params.add_users, sigma, params.n_frames);
	else if (params.type == "RAYLEIGH_GSL" ) return new Channel_Rayleigh_LLR<R>(params.N, params.complex, new Noise_GSL <R>(seed), params.add_users, sigma, params.n_frames);
#endif

	throw cannot_allocate(__FILE__, __LINE__, __func__);
}

void Factory_channel
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

void Factory_channel
::store_args(const Arguments_reader& ar, parameters &params,
             const int N, const bool complex, const bool add_users, const int n_frames)
{
	params.N         = N;
	params.n_frames  = n_frames;
	params.add_users = add_users;
	params.complex   = complex;
	// ------------------------------------------------------------------------------------------------------- channel
	if(ar.exist_arg({"chn-type"   })) params.type         = ar.get_arg({"chn-type"   });
	if(ar.exist_arg({"chn-path"   })) params.path         = ar.get_arg({"chn-path"   });
	if(ar.exist_arg({"chn-blk-fad"})) params.block_fading = ar.get_arg({"chn-blk-fad"});
}

void Factory_channel
::group_args(Arguments_reader::arg_grp& ar)
{
	ar.push_back({"chn", "Channel parameter(s)"});
}

void Factory_channel
::header(params_list& head_chn, const parameters& params)
{
	// ------------------------------------------------------------------------------------------------------- channel
	head_chn.push_back(std::make_pair("Type", params.type));

	if (params.type == "USER")
		head_chn.push_back(std::make_pair("Path", params.path));

	if (params.type.find("RAYLEIGH") != std::string::npos)
		head_chn.push_back(std::make_pair("Block fading policy", params.block_fading));
}
// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Channel<R_32>* aff3ct::tools::Factory_channel::build<R_32>(const aff3ct::tools::Factory_channel::parameters&, const int, const R_32);
template aff3ct::module::Channel<R_64>* aff3ct::tools::Factory_channel::build<R_64>(const aff3ct::tools::Factory_channel::parameters&, const int, const R_64);
#else
template struct aff3ct::tools::Factory_channel<R>;
template aff3ct::module::Channel<R>* aff3ct::tools::Factory_channel::build<R>(const aff3ct::tools::Factory_channel::parameters&, const int, const R);
#endif
// ==================================================================================== explicit template instantiation
