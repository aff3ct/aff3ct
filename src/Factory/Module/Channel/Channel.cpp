#include "Tools/Exception/exception.hpp"

#include "Module/Channel/NO/Channel_NO.hpp"
#include "Module/Channel/User/Channel_user.hpp"
#include "Module/Channel/AWGN/Channel_AWGN_LLR.hpp"
#include "Module/Channel/Rayleigh/Channel_Rayleigh_LLR.hpp"
#include "Module/Channel/Rayleigh/Channel_Rayleigh_LLR_user.hpp"

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

Channel::parameters
::parameters(const std::string prefix)
: Factory::parameters(Channel::name, Channel::name, prefix)
{
}

Channel::parameters
::~parameters()
{
}

Channel::parameters* Channel::parameters
::clone() const
{
	return new Channel::parameters(*this);
}

void Channel::parameters
::get_description(arg_map &req_args, arg_map &opt_args) const
{
	auto p = this->get_prefix();

	req_args[{p+"-fra-size", "N"}] =
		{"positive_int",
		 "number of symbols by frame."};

	opt_args[{p+"-fra", "F"}] =
		{"positive_int",
		 "set the number of inter frame level to process."};

	std::string chan_avail = "NO, USER, AWGN, AWGN_FAST, RAYLEIGH, RAYLEIGH_FAST, RAYLEIGH_USER";
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
		 "path to a noisy file, to use with \"--chn-type USER\" or to a gains file (used with \"--chn-type RAYLEIGH_USER\")."};

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

	opt_args[{p+"-gain-occur"}] =
		{"positive_int",
		 "the number of times a gain is used on consecutive symbols (used with \"--chn-type RAYLEIGH_USER\")."};
}

void Channel::parameters
::store(const arg_val_map &vals)
{
	auto p = this->get_prefix();

	if(exist(vals, {p+"-fra-size", "N"})) this->N            = std::stoi(vals.at({p+"-fra-size", "N"}));
	if(exist(vals, {p+"-fra",      "F"})) this->n_frames     = std::stoi(vals.at({p+"-fra",      "F"}));
	if(exist(vals, {p+"-type"         })) this->type         =           vals.at({p+"-type"         });
	if(exist(vals, {p+"-path"         })) this->path         =           vals.at({p+"-path"         });
	if(exist(vals, {p+"-blk-fad"      })) this->block_fading =           vals.at({p+"-blk-fad"      });
	if(exist(vals, {p+"-sigma"        })) this->sigma        = std::stof(vals.at({p+"-sigma"        }));
	if(exist(vals, {p+"-seed",     "S"})) this->seed         = std::stoi(vals.at({p+"-seed",     "S"}));
	if(exist(vals, {p+"-add-users"    })) this->add_users    = true;
	if(exist(vals, {p+"-complex"      })) this->complex      = true;
	if(exist(vals, {p+"-gain-occur"   })) this->gain_occur   = std::stoi(vals.at({p+"-gain-occur"   }));
}

void Channel::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	auto p = this->get_prefix();

	headers[p].push_back(std::make_pair("Type", this->type));

	if (full) headers[p].push_back(std::make_pair("Frame size (N)", std::to_string(this->N)));
	if (full) headers[p].push_back(std::make_pair("Inter frame level", std::to_string(this->n_frames)));

	if (this->sigma != -1.f)
		headers[p].push_back(std::make_pair("Sigma value", std::to_string(this->sigma)));

	if (this->type == "USER" || this->type == "RAYLEIGH_USER")
		headers[p].push_back(std::make_pair("Path", this->path));

	if (this->type == "RAYLEIGH_USER")
		headers[p].push_back(std::make_pair("Gain occurrences", std::to_string(this->gain_occur)));

	if (this->type.find("RAYLEIGH") != std::string::npos)
		headers[p].push_back(std::make_pair("Block fading policy", this->block_fading));

	if ((this->type != "NO" && this->type != "USER") && full)
		headers[p].push_back(std::make_pair("Seed", std::to_string(this->seed)));

	headers[p].push_back(std::make_pair("Complex", this->complex ? "on" : "off"));
	headers[p].push_back(std::make_pair("Add users", this->add_users ? "on" : "off"));
}

template <typename R>
module::Channel<R>* Channel::parameters
::build() const
{
	     if (type == "AWGN"         ) return new module::Channel_AWGN_LLR    <R>(N,          new tools::Noise_std <R>(seed), add_users, sigma, n_frames);
	else if (type == "AWGN_FAST"    ) return new module::Channel_AWGN_LLR    <R>(N,          new tools::Noise_fast<R>(seed), add_users, sigma, n_frames);
	else if (type == "RAYLEIGH"     ) return new module::Channel_Rayleigh_LLR<R>(N, complex, new tools::Noise_std <R>(seed), add_users, sigma, n_frames);
	else if (type == "RAYLEIGH_FAST") return new module::Channel_Rayleigh_LLR<R>(N, complex, new tools::Noise_fast<R>(seed), add_users, sigma, n_frames);
	else if (type == "USER"         ) return new module::Channel_user        <R>(N, path,                                    add_users,        n_frames);
	else if (type == "NO"           ) return new module::Channel_NO          <R>(N,                                          add_users,        n_frames);
#ifdef CHANNEL_MKL
	else if (type == "AWGN_MKL"     ) return new module::Channel_AWGN_LLR    <R>(N,          new tools::Noise_MKL <R>(seed), add_users, sigma, n_frames);
	else if (type == "RAYLEIGH_MKL" ) return new module::Channel_Rayleigh_LLR<R>(N, complex, new tools::Noise_MKL <R>(seed), add_users, sigma, n_frames);
#endif
#ifdef CHANNEL_GSL
	else if (type == "AWGN_GSL"     ) return new module::Channel_AWGN_LLR    <R>(N,          new tools::Noise_GSL <R>(seed), add_users, sigma, n_frames);
	else if (type == "RAYLEIGH_GSL" ) return new module::Channel_Rayleigh_LLR<R>(N, complex, new tools::Noise_GSL <R>(seed), add_users, sigma, n_frames);
#endif
	else if (type == "RAYLEIGH_USER") return new module::Channel_Rayleigh_LLR_user<R>(N, complex, path, gain_occur, new tools::Noise_fast<R>(seed), add_users, sigma, n_frames);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename R>
module::Channel<R>* Channel
::build(const parameters &params)
{
	return params.template build<R>();
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Channel<R_32>* aff3ct::factory::Channel::parameters::build<R_32>() const;
template aff3ct::module::Channel<R_64>* aff3ct::factory::Channel::parameters::build<R_64>() const;
template aff3ct::module::Channel<R_32>* aff3ct::factory::Channel::build<R_32>(const aff3ct::factory::Channel::parameters&);
template aff3ct::module::Channel<R_64>* aff3ct::factory::Channel::build<R_64>(const aff3ct::factory::Channel::parameters&);
#else
template aff3ct::module::Channel<R>* aff3ct::factory::Channel::parameters::build<R>() const;
template aff3ct::module::Channel<R>* aff3ct::factory::Channel::build<R>(const aff3ct::factory::Channel::parameters&);
#endif
// ==================================================================================== explicit template instantiation
