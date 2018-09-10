#include "Tools/Exception/exception.hpp"

#include "Module/Channel/NO/Channel_NO.hpp"
#include "Module/Channel/User/Channel_user.hpp"
#include "Module/Channel/AWGN/Channel_AWGN_LLR.hpp"
#include "Module/Channel/Rayleigh/Channel_Rayleigh_LLR.hpp"
#include "Module/Channel/Rayleigh/Channel_Rayleigh_LLR_user.hpp"
#include "Module/Channel/Optical/Channel_optical.hpp"
#include "Module/Channel/Binary_erasure/Channel_binary_erasure.hpp"
#include "Module/Channel/Binary_symmetric/Channel_binary_symmetric.hpp"

#include "Tools/Algo/Draw_generator/Gaussian_noise_generator/Standard/Gaussian_noise_generator_std.hpp"
#include "Tools/Algo/Draw_generator/Gaussian_noise_generator/Fast/Gaussian_noise_generator_fast.hpp"
#include "Tools/Algo/Draw_generator/Event_generator/Standard/Event_generator_std.hpp"
#include "Tools/Algo/Draw_generator/Event_generator/Fast/Event_generator_fast.hpp"
#include "Tools/Algo/Draw_generator/User_pdf_noise_generator/Standard/User_pdf_noise_generator_std.hpp"
#include "Tools/Algo/Draw_generator/User_pdf_noise_generator/Fast/User_pdf_noise_generator_fast.hpp"
#ifdef CHANNEL_MKL
#include "Tools/Algo/Draw_generator/Event_generator/MKL/Event_generator_MKL.hpp"
#include "Tools/Algo/Draw_generator/Gaussian_noise_generator/MKL/Gaussian_noise_generator_MKL.hpp"
#include "Tools/Algo/Draw_generator/User_pdf_noise_generator/MKL/User_pdf_noise_generator_MKL.hpp"
#endif
#ifdef CHANNEL_GSL
#include "Tools/Algo/Draw_generator/Event_generator/GSL/Event_generator_GSL.hpp"
#include "Tools/Algo/Draw_generator/Gaussian_noise_generator/GSL/Gaussian_noise_generator_GSL.hpp"
#include "Tools/Algo/Draw_generator/User_pdf_noise_generator/GSL/User_pdf_noise_generator_GSL.hpp"
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
::get_description(tools::Argument_map_info &args) const
{
	auto p = this->get_prefix();

	args.add(
		{p+"-fra-size", "N"},
		tools::Integer(tools::Positive(), tools::Non_zero()),
		"number of symbols by frame.",
		tools::arg_rank::REQ);

	args.add(
		{p+"-fra", "F"},
		tools::Integer(tools::Positive(), tools::Non_zero()),
		"set the number of inter frame level to process.");

	args.add(
		{p+"-type"},
		tools::Text(tools::Including_set("NO", "USER", "USER_ADD", "AWGN", "RAYLEIGH", "RAYLEIGH_USER", "BEC", "BSC",
		                                 "OPTICAL")),
		"type of the channel to use in the simulation ('USER' has an output got from a file when 'USER_ADD' has an"
		" additive noise got from a file).");

	args.add(
		{p+"-implem"},
		tools::Text(tools::Including_set("STD", "FAST")),
		"select the implementation of the algorithm to generate noise.");

#ifdef CHANNEL_GSL
	tools::add_options(args.at({p+"-implem"}), 0, "GSL");
#endif
#ifdef CHANNEL_MKL
	tools::add_options(args.at({p+"-implem"}), 0, "MKL");
#endif

	args.add(
		{p+"-path"},
		tools::File(tools::openmode::read_write),
		"path to a noisy file, to use with \"--chn-type USER,OPTICAL\" or to a gain file (used with \"--chn-type RAYLEIGH_USER\").");

	args.add(
		{p+"-blk-fad"},
		tools::Text(tools::Including_set("NO", "FRAME", "ONETAP")),
		"block fading policy for the RAYLEIGH channel.");

	args.add(
		{p+"-noise"},
		tools::Real(tools::Positive(), tools::Non_zero()),
		"noise value (for SIGMA, ROP or EP noise type).");

	args.add(
		{p+"-seed", "S"},
		tools::Integer(tools::Positive()),
		"seed used to initialize the pseudo random generators.");

	args.add(
		{p+"-add-users"},
		tools::None(),
		"add all the users (= frames) before generating the noise.");

	args.add(
		{p+"-complex"},
		tools::None(),
		"enable complex noise generation.");

	args.add(
		{p+"-gain-occur"},
		tools::Integer(tools::Positive(), tools::Non_zero()),
		"the number of times a gain is used on consecutive symbols (used with \"--chn-type RAYLEIGH_USER\").");
}

void Channel::parameters
::store(const tools::Argument_map_value &vals)
{
	auto p = this->get_prefix();

	if(vals.exist({p+"-fra-size", "N"})) this->N            = vals.to_int({p+"-fra-size", "N"});
	if(vals.exist({p+"-fra",      "F"})) this->n_frames     = vals.to_int({p+"-fra",      "F"});
	if(vals.exist({p+"-seed",     "S"})) this->seed         = vals.to_int({p+"-seed",     "S"});
	if(vals.exist({p+"-gain-occur"   })) this->gain_occur   = vals.to_int({p+"-gain-occur"   });
	if(vals.exist({p+"-type"         })) this->type         = vals.at    ({p+"-type"         });
	if(vals.exist({p+"-implem"       })) this->implem       = vals.at    ({p+"-implem"       });
	if(vals.exist({p+"-path"         })) this->path         = vals.at    ({p+"-path"         });
	if(vals.exist({p+"-blk-fad"      })) this->block_fading = vals.at    ({p+"-blk-fad"      });
	if(vals.exist({p+"-add-users"    })) this->add_users    = true;
	if(vals.exist({p+"-complex"      })) this->complex      = true;
	if(vals.exist({p+"-noise"        })) this->noise        = vals.to_float({p+"-noise"      });
}

void Channel::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	auto p = this->get_prefix();

	headers[p].push_back(std::make_pair("Type",           this->type  ));
	headers[p].push_back(std::make_pair("Implementation", this->implem));

	if (full) headers[p].push_back(std::make_pair("Frame size (N)", std::to_string(this->N)));
	if (full) headers[p].push_back(std::make_pair("Inter frame level", std::to_string(this->n_frames)));

	if (this->noise != -1.f)
		headers[p].push_back(std::make_pair("Sigma value", std::to_string(this->noise)));

	if (this->type == "USER" || this->type == "USER_ADD" || this->type == "RAYLEIGH_USER")
		headers[p].push_back(std::make_pair("Path", this->path));

	if (this->type == "RAYLEIGH_USER")
		headers[p].push_back(std::make_pair("Gain occurrences", std::to_string(this->gain_occur)));

	if (this->type.find("RAYLEIGH") != std::string::npos)
		headers[p].push_back(std::make_pair("Block fading policy", this->block_fading));

	if ((this->type != "NO" && this->type != "USER" && this->type != "USER_ADD") && full)
		headers[p].push_back(std::make_pair("Seed", std::to_string(this->seed)));

	headers[p].push_back(std::make_pair("Complex", this->complex ? "on" : "off"));
	headers[p].push_back(std::make_pair("Add users", this->add_users ? "on" : "off"));
}

template <typename R>
module::Channel<R>* Channel::parameters
::build_event() const
{

	tools::Event_generator<R>* n = nullptr;
	     if (implem == "STD" ) n = new tools::Event_generator_std <R>(seed);
	else if (implem == "FAST") n = new tools::Event_generator_fast<R>(seed);
#ifdef CHANNEL_MKL
	else if (implem == "MKL" ) n = new tools::Event_generator_MKL<R>(seed);
#endif
#ifdef CHANNEL_GSL
	else if (implem == "GSL" ) n = new tools::Event_generator_GSL<R>(seed);
#endif
	else
		throw tools::cannot_allocate(__FILE__, __LINE__, __func__);

	     if (type == "BEC"     ) return new module::Channel_binary_erasure  <R>(N, n, tools::Event_probability<R>((R)noise), n_frames);
	else if (type == "BSC"     ) return new module::Channel_binary_symmetric<R>(N, n, tools::Event_probability<R>((R)noise), n_frames);

	delete n;

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename R>
module::Channel<R>* Channel::parameters
::build_gaussian() const
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

	     if (type == "AWGN"         ) return new module::Channel_AWGN_LLR         <R>(N,                            n, add_users, tools::Sigma<R>((R)noise), n_frames);
	else if (type == "RAYLEIGH"     ) return new module::Channel_Rayleigh_LLR     <R>(N, complex,                   n, add_users, tools::Sigma<R>((R)noise), n_frames);
	else if (type == "RAYLEIGH_USER") return new module::Channel_Rayleigh_LLR_user<R>(N, complex, path, gain_occur, n, add_users, tools::Sigma<R>((R)noise), n_frames);

	delete n;

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename R>
module::Channel<R>* Channel::parameters
::build_userpdf(std::shared_ptr<const tools::Distributions<R>> dist) const
{
	if (dist == nullptr)
		throw tools::cannot_allocate(__FILE__, __LINE__, __func__);

	tools::User_pdf_noise_generator<R>* n = nullptr;
	     if (implem == "STD" ) n = new tools::User_pdf_noise_generator_std <R>(*dist, seed);
	else if (implem == "FAST") n = new tools::User_pdf_noise_generator_fast<R>(*dist, seed);
#ifdef CHANNEL_MKL
	else if (implem == "MKL" ) n = new tools::User_pdf_noise_generator_MKL <R>(*dist, seed);
#endif
#ifdef CHANNEL_GSL
	else if (implem == "GSL" ) n = new tools::User_pdf_noise_generator_GSL <R>(*dist, seed);
#endif
	else
		throw tools::cannot_allocate(__FILE__, __LINE__, __func__);

	     if (type == "OPTICAL") return new module::Channel_optical<R>(N, n, tools::Received_optical_power<R>((R) this->noise), n_frames);

	delete n;

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename R>
module::Channel<R>* Channel::parameters
::build(std::shared_ptr<const tools::Distributions<R>> dist) const
{
	try	{
		return build_gaussian<R>();
	} catch (tools::cannot_allocate&) {}

	try	{
		return build_event<R>();
	} catch (tools::cannot_allocate&) {}

	try	{
		return build_userpdf<R>(dist);
	} catch (tools::cannot_allocate&) {}

	if (type == "USER"    ) return new module::Channel_user<R>(N, path, add_users, false, n_frames);
	if (type == "USER_ADD") return new module::Channel_user<R>(N, path, add_users,  true, n_frames);
	if (type == "NO"      ) return new module::Channel_NO  <R>(N,       add_users,        n_frames);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename R>
module::Channel<R>* Channel
::build(const parameters &params, std::shared_ptr<const tools::Distributions<R>> dist)
{
	return params.template build<R>(dist);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Channel<R_32>* aff3ct::factory::Channel::parameters::build<R_32>(std::shared_ptr<const tools::Distributions<R_32>>) const;
template aff3ct::module::Channel<R_64>* aff3ct::factory::Channel::parameters::build<R_64>(std::shared_ptr<const tools::Distributions<R_64>>) const;
template aff3ct::module::Channel<R_32>* aff3ct::factory::Channel::build<R_32>(const aff3ct::factory::Channel::parameters&, std::shared_ptr<const tools::Distributions<R_32>>);
template aff3ct::module::Channel<R_64>* aff3ct::factory::Channel::build<R_64>(const aff3ct::factory::Channel::parameters&, std::shared_ptr<const tools::Distributions<R_64>>);
#else
template aff3ct::module::Channel<R>* aff3ct::factory::Channel::parameters::build<R>(std::shared_ptr<const tools::Distributions<R>>) const;
template aff3ct::module::Channel<R>* aff3ct::factory::Channel::build<R>(const aff3ct::factory::Channel::parameters&, std::shared_ptr<const tools::Distributions<R>>);
#endif
// ==================================================================================== explicit template instantiation
