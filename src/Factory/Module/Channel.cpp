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

#include "Channel.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Channel::name   = "Channel";
const std::string aff3ct::factory::Channel::prefix = "chn";

template <typename R>
module::Channel<R>* Channel
::build(const parameters &params)
{
	     if (params.type == "AWGN"         ) return new module::Channel_AWGN_LLR    <R>(params.N,                 new tools::Noise_std <R>(params.seed), params.add_users, params.sigma, params.n_frames);
	else if (params.type == "AWGN_FAST"    ) return new module::Channel_AWGN_LLR    <R>(params.N,                 new tools::Noise_fast<R>(params.seed), params.add_users, params.sigma, params.n_frames);
	else if (params.type == "RAYLEIGH"     ) return new module::Channel_Rayleigh_LLR<R>(params.N, params.complex, new tools::Noise_std <R>(params.seed), params.add_users, params.sigma, params.n_frames);
	else if (params.type == "RAYLEIGH_FAST") return new module::Channel_Rayleigh_LLR<R>(params.N, params.complex, new tools::Noise_fast<R>(params.seed), params.add_users, params.sigma, params.n_frames);
	else if (params.type == "USER"         ) return new module::Channel_user        <R>(params.N, params.path,                                           params.add_users,               params.n_frames);
	else if (params.type == "NO"           ) return new module::Channel_NO          <R>(params.N,                                                        params.add_users,               params.n_frames);
#ifdef CHANNEL_MKL
	else if (params.type == "AWGN_MKL"     ) return new module::Channel_AWGN_LLR    <R>(params.N,                 new tools::Noise_MKL <R>(params.seed), params.add_users, params.sigma, params.n_frames);
	else if (params.type == "RAYLEIGH_MKL" ) return new module::Channel_Rayleigh_LLR<R>(params.N, params.complex, new tools::Noise_MKL <R>(params.seed), params.add_users, params.sigma, params.n_frames);
#endif
#ifdef CHANNEL_GSL
	else if (params.type == "AWGN_GSL"     ) return new module::Channel_AWGN_LLR    <R>(params.N,                 new tools::Noise_GSL <R>(params.seed), params.add_users, params.sigma, params.n_frames);
	else if (params.type == "RAYLEIGH_GSL" ) return new module::Channel_Rayleigh_LLR<R>(params.N, params.complex, new tools::Noise_GSL <R>(params.seed), params.add_users, params.sigma, params.n_frames);
#endif

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

void Channel
::build_args(arg_map &req_args, arg_map &opt_args, const std::string p)
{
	req_args[{p+"-fra-size", "N"}] =
		{"positive_int",
		 "number of symbols by frame."};

	opt_args[{p+"-fra", "F"}] =
		{"positive_int",
		 "set the number of inter frame level to process."};

	std::string chan_avail = "NO, USER, AWGN, AWGN_FAST, RAYLEIGH, RAYLEIGH_FAST";
#ifdef CHANNEL_GSL
	chan_avail += ", AWGN_GSL, RAYLEIGH_GSL";
#endif
#ifdef CHANNEL_MKL
	chan_avail += ", AWGN_MKL, RAYLEIGH_MKL";
#endif

	opt_args[{p+"-type"}] =
		{"string",
		 "type of the channel to use in the simulation.",
		 chan_avail};

	opt_args[{p+"-path"}] =
		{"string",
		 "path to a noisy file, to use with \"--chn-type USER\"."};

	opt_args[{p+"-blk-fad"}] =
		{"string",
		 "block fading policy for the RAYLEIGH channel.",
		 "NO, FRAME, ONETAP"};

	opt_args[{p+"-sigma"}] =
		{"positive_float",
		 "noise variance value."};

	opt_args[{p+"-seed", "S"}] =
		{"positive_int",
		 "seed used to initialize the pseudo random generators."};

	opt_args[{p+"-add-users"}] =
		{"",
		 "add all the users (= frames) before generating the noise."};

	opt_args[{p+"-complex"}] =
		{"",
		 "enable complex noise generation."};
}

void Channel
::store_args(const tools::Arguments_reader& ar, parameters &params, const std::string p)
{
	if(ar.exist_arg({p+"-fra-size", "N"})) params.N            = ar.get_arg_int  ({p+"-fra-size", "N"});
	if(ar.exist_arg({p+"-fra",      "F"})) params.n_frames     = ar.get_arg_int  ({p+"-fra",      "F"});
	if(ar.exist_arg({p+"-type"         })) params.type         = ar.get_arg      ({p+"-type"         });
	if(ar.exist_arg({p+"-path"         })) params.path         = ar.get_arg      ({p+"-path"         });
	if(ar.exist_arg({p+"-blk-fad"      })) params.block_fading = ar.get_arg      ({p+"-blk-fad"      });
	if(ar.exist_arg({p+"-sigma"        })) params.sigma        = ar.get_arg_float({p+"-sigma"        });
	if(ar.exist_arg({p+"-seed",     "S"})) params.seed         = ar.get_arg_int  ({p+"-seed",     "S"});
	if(ar.exist_arg({p+"-add-users"    })) params.add_users    = true;
	if(ar.exist_arg({p+"-complex"      })) params.complex      = true;
}

void Channel
::header(params_list& head_chn, const parameters& params)
{
	head_chn.push_back(std::make_pair("Type", params.type));
	head_chn.push_back(std::make_pair("Frame size (N)", std::to_string(params.N)));
	head_chn.push_back(std::make_pair("Inter frame level", std::to_string(params.n_frames)));
	if (params.sigma != -1.f)
		head_chn.push_back(std::make_pair("Sigma value", std::to_string(params.sigma)));
	if (params.type == "USER")
		head_chn.push_back(std::make_pair("Path", params.path));
	if (params.type.find("RAYLEIGH") != std::string::npos)
		head_chn.push_back(std::make_pair("Block fading policy", params.block_fading));
	if (params.type != "NO" && params.type != "USER")
		head_chn.push_back(std::make_pair("Seed", std::to_string(params.seed)));
	head_chn.push_back(std::make_pair("Complex", params.complex ? "on" : "off"));
	head_chn.push_back(std::make_pair("Add users", params.add_users ? "on" : "off"));
}
// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Channel<R_32>* aff3ct::factory::Channel::build<R_32>(const aff3ct::factory::Channel::parameters&);
template aff3ct::module::Channel<R_64>* aff3ct::factory::Channel::build<R_64>(const aff3ct::factory::Channel::parameters&);
#else
template aff3ct::module::Channel<R>* aff3ct::factory::Channel::build<R>(const aff3ct::factory::Channel::parameters&);
#endif
// ==================================================================================== explicit template instantiation
