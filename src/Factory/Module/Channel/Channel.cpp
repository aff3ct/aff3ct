#include "Tools/Exception/exception.hpp"

#include "Module/Channel/NO/Channel_NO.hpp"
#include "Module/Channel/User/Channel_user.hpp"
#include "Module/Channel/AWGN/Channel_AWGN_LLR.hpp"
#include "Module/Channel/Rayleigh/Channel_Rayleigh_LLR.hpp"
#include "Module/Channel/Rayleigh/Channel_Rayleigh_LLR_user.hpp"

#include "Tools/Algo/Gaussian_noise_generator/Standard/Gaussian_noise_generator_std.hpp"
#include "Tools/Algo/Gaussian_noise_generator/Fast/Gaussian_noise_generator_fast.hpp"
#ifdef CHANNEL_MKL
#include "Tools/Algo/Gaussian_noise_generator/MKL/Gaussian_noise_generator_MKL.hpp"
#endif
#ifdef CHANNEL_GSL
#include "Tools/Algo/Gaussian_noise_generator/GSL/Gaussian_noise_generator_GSL.hpp"
#endif

#include "Channel.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Channel_name   = "Channel";
const std::string aff3ct::factory::Channel_prefix = "chn";

Channel::parameters
::parameters(const std::string &prefix)
: Factory::parameters(Channel_name, Channel_name, prefix)
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
		{"strictly_positive_int",
		 "number of symbols by frame."};

	opt_args[{p+"-fra", "F"}] =
		{"strictly_positive_int",
		 "set the number of inter frame level to process."};

	opt_args[{p+"-type"}] =
		{"string",
		 "type of the channel to use in the simulation.",
		 "NO, USER, AWGN, RAYLEIGH, RAYLEIGH_USER"};

	std::string implem_avail = "STD, FAST";
#ifdef CHANNEL_GSL
	implem_avail += ", GSL";
#endif
#ifdef CHANNEL_MKL
	implem_avail += ", MKL";
#endif

	opt_args[{p+"-implem"}] =
		{"string",
		 "select the implementation of the algorithm to generate noise.",
		 implem_avail};

	opt_args[{p+"-path"}] =
		{"string",
		 "path to a noisy file, to use with \"--chn-type USER\" or to a gains file (used with \"--chn-type RAYLEIGH_USER\")."};

	opt_args[{p+"-blk-fad"}] =
		{"string",
		 "block fading policy for the RAYLEIGH channel.",
		 "NO, FRAME, ONETAP"};

	opt_args[{p+"-sigma"}] =
		{"strictly_positive_float",
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
		{"strictly_positive_int",
		 "the number of times a gain is used on consecutive symbols (used with \"--chn-type RAYLEIGH_USER\")."};
}

void Channel::parameters
::store(const arg_val_map &vals)
{
	auto p = this->get_prefix();

	if(exist(vals, {p+"-fra-size", "N"})) this->N            = std::stoi(vals.at({p+"-fra-size", "N"}));
	if(exist(vals, {p+"-fra",      "F"})) this->n_frames     = std::stoi(vals.at({p+"-fra",      "F"}));
	if(exist(vals, {p+"-type"         })) this->type         =           vals.at({p+"-type"         });
	if(exist(vals, {p+"-implem"       })) this->implem       =           vals.at({p+"-implem"       });
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

	headers[p].push_back(std::make_pair("Type",           this->type  ));
	headers[p].push_back(std::make_pair("Implementation", this->implem));

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
	tools::Gaussian_noise_generator<R>* n = nullptr;
	     if (implem == "STD" ) n = new tools::Gaussian_noise_generator_std <R>(seed);
	else if (implem == "FAST") n = new tools::Gaussian_noise_generator_fast<R>(seed);
#ifdef CHANNEL_MKL
	else if (implem == "MKL" ) n = new tools::Gaussian_noise_generator_MKL <R>(seed);
#endif
#ifdef CHANNEL_GSL
	else if (implem == "GSL" ) n = new tools::Gaussian_noise_generator_GSL <R>(seed);
#endif
	else
		throw tools::cannot_allocate(__FILE__, __LINE__, __func__);

	     if (type == "AWGN"         ) return new module::Channel_AWGN_LLR         <R>(N,                            n, add_users, sigma, n_frames);
	else if (type == "RAYLEIGH"     ) return new module::Channel_Rayleigh_LLR     <R>(N, complex,                   n, add_users, sigma, n_frames);
	else if (type == "RAYLEIGH_USER") return new module::Channel_Rayleigh_LLR_user<R>(N, complex, path, gain_occur, n, add_users, sigma, n_frames);
	else
	{
		module::Channel<R>* c = nullptr;
		     if (type == "USER") c = new module::Channel_user<R>(N, path, add_users, n_frames);
		else if (type == "NO"  ) c = new module::Channel_NO  <R>(N,       add_users, n_frames);

		delete n;

		if (c) return c;

		throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
	}
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
