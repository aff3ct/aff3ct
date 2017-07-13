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
::build(const parameters &params)
{
	     if (params.type == "AWGN"         ) return new Channel_AWGN_LLR    <R>(params.N,                 new Noise_std <R>(params.seed), params.add_users, params.sigma, params.n_frames);
	else if (params.type == "AWGN_FAST"    ) return new Channel_AWGN_LLR    <R>(params.N,                 new Noise_fast<R>(params.seed), params.add_users, params.sigma, params.n_frames);
	else if (params.type == "RAYLEIGH"     ) return new Channel_Rayleigh_LLR<R>(params.N, params.complex, new Noise_std <R>(params.seed), params.add_users, params.sigma, params.n_frames);
	else if (params.type == "RAYLEIGH_FAST") return new Channel_Rayleigh_LLR<R>(params.N, params.complex, new Noise_fast<R>(params.seed), params.add_users, params.sigma, params.n_frames);
	else if (params.type == "USER"         ) return new Channel_user        <R>(params.N, params.path,                                    params.add_users,               params.n_frames);
	else if (params.type == "NO"           ) return new Channel_NO          <R>(params.N,                                                 params.add_users,               params.n_frames);
#ifdef CHANNEL_MKL
	else if (params.type == "AWGN_MKL"     ) return new Channel_AWGN_LLR    <R>(params.N,                 new Noise_MKL <R>(params.seed), params.add_users, params.sigma, params.n_frames);
	else if (params.type == "RAYLEIGH_MKL" ) return new Channel_Rayleigh_LLR<R>(params.N, params.complex, new Noise_MKL <R>(params.seed), params.add_users, params.sigma, params.n_frames);
#endif
#ifdef CHANNEL_GSL
	else if (params.type == "AWGN_GSL"     ) return new Channel_AWGN_LLR    <R>(params.N,                 new Noise_GSL <R>(params.seed), params.add_users, params.sigma, params.n_frames);
	else if (params.type == "RAYLEIGH_GSL" ) return new Channel_Rayleigh_LLR<R>(params.N, params.complex, new Noise_GSL <R>(params.seed), params.add_users, params.sigma, params.n_frames);
#endif

	throw cannot_allocate(__FILE__, __LINE__, __func__);
}

void Factory_channel
::build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args)
{
	req_args[{"chn-fra-size", "N"}] =
		{"positive_int",
		 "number of symbols by frame."};

	opt_args[{"chn-fra", "F"}] =
		{"positive_int",
		 "set the number of inter frame level to process."};

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

	opt_args[{"chn-sigma"}] =
		{"positive_float",
		 "noise variance value."};

	opt_args[{"chn-seed", "S"}] =
		{"positive_int",
		 "seed used to initialize the pseudo random generators."};

	opt_args[{"chn-add-users"}] =
		{"",
		 "add all the users (= frames) before generating the noise."};

	opt_args[{"chn-complex"}] =
		{"",
		 "enable complex noise generation."};
}

void Factory_channel
::store_args(const Arguments_reader& ar, parameters &params)
{
	params.N = ar.get_arg_int({"chn-fra-size", "N"});
	if(ar.exist_arg({"chn-fra", "F"})) params.n_frames = ar.get_arg_int({"chn-fra", "F"});
	if(ar.exist_arg({"chn-type"})) params.type = ar.get_arg({"chn-type"});
	if(ar.exist_arg({"chn-path"})) params.path = ar.get_arg({"chn-path"});
	if(ar.exist_arg({"chn-blk-fad"})) params.block_fading = ar.get_arg({"chn-blk-fad"});
	if(ar.exist_arg({"chn-sigma"})) params.sigma = ar.get_arg_float({"chn-sigma"});
	if(ar.exist_arg({"chn-seed", "S"})) params.seed = ar.get_arg_int({"chn-seed", "S"});
	if(ar.exist_arg({"chn-add-users"})) params.add_users = true;
	if(ar.exist_arg({"chn-complex"})) params.complex = true;
}

void Factory_channel
::group_args(Arguments_reader::arg_grp& ar)
{
	ar.push_back({"chn", "Channel parameter(s)"});
}

void Factory_channel
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
template aff3ct::module::Channel<R_32>* aff3ct::tools::Factory_channel::build<R_32>(const aff3ct::tools::Factory_channel::parameters&);
template aff3ct::module::Channel<R_64>* aff3ct::tools::Factory_channel::build<R_64>(const aff3ct::tools::Factory_channel::parameters&);
#else
template struct aff3ct::tools::Factory_channel<R>;
template aff3ct::module::Channel<R>* aff3ct::tools::Factory_channel::build<R>(const aff3ct::tools::Factory_channel::parameters&);
#endif
// ==================================================================================== explicit template instantiation
