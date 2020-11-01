#include <utility>
#include <algorithm>

#include "Tools/Exception/exception.hpp"
#include "Tools/Documentation/documentation.h"
#include "Module/Channel/NO/Channel_NO.hpp"
#include "Module/Channel/User/Channel_user.hpp"
#include "Module/Channel/User/Channel_user_add.hpp"
#include "Module/Channel/User/Channel_user_be.hpp"
#include "Module/Channel/User/Channel_user_bs.hpp"
#include "Module/Channel/AWGN/Channel_AWGN_LLR.hpp"
#include "Module/Channel/Rayleigh/Channel_Rayleigh_LLR.hpp"
#include "Module/Channel/Rayleigh/Channel_Rayleigh_LLR_user.hpp"
#include "Module/Channel/Optical/Channel_optical.hpp"
#include "Module/Channel/Binary_erasure/Channel_binary_erasure.hpp"
#include "Module/Channel/Binary_symmetric/Channel_binary_symmetric.hpp"
#include "Factory/Module/Channel/Channel.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Channel_name   = "Channel";
const std::string aff3ct::factory::Channel_prefix = "chn";

Channel
::Channel(const std::string &prefix)
: Factory(Channel_name, Channel_name, prefix)
{
}

Channel* Channel
::clone() const
{
	return new Channel(*this);
}

void Channel
::get_description(cli::Argument_map_info &args) const
{
	auto p = this->get_prefix();
	const std::string class_name = "factory::Channel::";

	tools::add_arg(args, p, class_name+"p+fra-size,N",
		cli::Integer(cli::Positive(), cli::Non_zero()),
		cli::arg_rank::REQ);

	tools::add_arg(args, p, class_name+"p+type",
		cli::Text(cli::Including_set("NO", "AWGN", "RAYLEIGH", "RAYLEIGH_USER", "BEC", "BSC", "OPTICAL", "USER",
		                             "USER_ADD", "USER_BEC", "USER_BSC")));

	tools::add_arg(args, p, class_name+"p+implem",
		cli::Text(cli::Including_set("STD", "FAST")));

#ifdef AFF3CT_CHANNEL_GSL
	cli::add_options(args.at({p+"-implem"}), 0, "GSL");
#endif
#ifdef AFF3CT_CHANNEL_MKL
	cli::add_options(args.at({p+"-implem"}), 0, "MKL");
#endif

	tools::add_arg(args, p, class_name+"p+path",
		cli::File(cli::openmode::read));

	tools::add_arg(args, p, class_name+"p+blk-fad",
		cli::Text(cli::Including_set("NO", "FRAME", "ONETAP")));

	tools::add_arg(args, p, class_name+"p+seed,S",
		cli::Integer(cli::Positive()));

	tools::add_arg(args, p, class_name+"p+add-users",
		cli::None());

	tools::add_arg(args, p, class_name+"p+complex",
		cli::None());

	tools::add_arg(args, p, class_name+"p+gain-occur",
		cli::Integer(cli::Positive(), cli::Non_zero()));
}

void Channel
::store(const cli::Argument_map_value &vals)
{
	auto p = this->get_prefix();

	if(vals.exist({p+"-fra-size", "N"})) this->N            = vals.to_int ({p+"-fra-size", "N"});
	if(vals.exist({p+"-seed",     "S"})) this->seed         = vals.to_int ({p+"-seed",     "S"});
	if(vals.exist({p+"-gain-occur"   })) this->gain_occur   = vals.to_int ({p+"-gain-occur"   });
	if(vals.exist({p+"-type"         })) this->type         = vals.at     ({p+"-type"         });
	if(vals.exist({p+"-implem"       })) this->implem       = vals.at     ({p+"-implem"       });
	if(vals.exist({p+"-path"         })) this->path         = vals.to_file({p+"-path"         });
	if(vals.exist({p+"-blk-fad"      })) this->block_fading = vals.at     ({p+"-blk-fad"      });
	if(vals.exist({p+"-add-users"    })) this->add_users    = true;
	if(vals.exist({p+"-complex"      })) this->complex      = true;
}

void Channel
::get_headers(std::map<std::string,tools::header_list>& headers, const bool full) const
{
	auto p = this->get_prefix();

	headers[p].push_back(std::make_pair("Type",           this->type  ));
	headers[p].push_back(std::make_pair("Implementation", this->implem));

	if (full) headers[p].push_back(std::make_pair("Frame size (N)", std::to_string(this->N)));

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
module::Channel<R>* Channel
::build_event() const
{
	tools::Event_generator_implem impl;
	     if (this->implem == "STD" ) impl = tools::Event_generator_implem::STD;
	else if (this->implem == "FAST") impl = tools::Event_generator_implem::FAST;
#ifdef AFF3CT_CHANNEL_MKL
	else if (this->implem == "MKL" ) impl = tools::Event_generator_implem::MKL;
#endif
#ifdef AFF3CT_CHANNEL_GSL
	else if (this->implem == "GSL" ) impl = tools::Event_generator_implem::GSL;
#endif
	else
		throw tools::cannot_allocate(__FILE__, __LINE__, __func__);

	if (type == "BEC") return new module::Channel_binary_erasure  <R>(this->N, impl, this->seed);
	if (type == "BSC") return new module::Channel_binary_symmetric<R>(this->N, impl, this->seed);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename R>
module::Channel<R>* Channel
::build_gaussian() const
{
	tools::Gaussian_noise_generator_implem impl;
	     if (this->implem == "STD" ) impl = tools::Gaussian_noise_generator_implem::STD;
	else if (this->implem == "FAST") impl = tools::Gaussian_noise_generator_implem::FAST;
#ifdef AFF3CT_CHANNEL_MKL
	else if (this->implem == "MKL" ) impl = tools::Gaussian_noise_generator_implem::MKL;
#endif
#ifdef AFF3CT_CHANNEL_GSL
	else if (this->implem == "GSL" ) impl = tools::Gaussian_noise_generator_implem::GSL;
#endif
	else
		throw tools::cannot_allocate(__FILE__, __LINE__, __func__);

	if (type == "AWGN"         ) return new module::Channel_AWGN_LLR         <R>(this->N,                            impl, this->seed,                   this->add_users);
	if (type == "RAYLEIGH"     ) return new module::Channel_Rayleigh_LLR     <R>(this->N, this->complex,             impl, this->seed,                   this->add_users);
	if (type == "RAYLEIGH_USER") return new module::Channel_Rayleigh_LLR_user<R>(this->N, this->complex, this->path, impl, this->seed, this->gain_occur, this->add_users);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename R>
module::Channel<R>* Channel
::build_userpdf(const tools::Distributions<R>& dist) const
{
	tools::User_pdf_noise_generator_implem impl;
	     if (implem == "STD" ) impl = tools::User_pdf_noise_generator_implem::STD;
	else if (implem == "FAST") impl = tools::User_pdf_noise_generator_implem::FAST;
#ifdef AFF3CT_CHANNEL_MKL
	else if (implem == "MKL" ) impl = tools::User_pdf_noise_generator_implem::MKL;
#endif
#ifdef AFF3CT_CHANNEL_GSL
	else if (implem == "GSL" ) impl = tools::User_pdf_noise_generator_implem::GSL;
#endif
	else
		throw tools::cannot_allocate(__FILE__, __LINE__, __func__);

	if (type == "OPTICAL") return new module::Channel_optical<R>(this->N, dist, impl, this->seed);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename R>
module::Channel<R>* Channel
::build() const
{
	try	{
		return build_gaussian<R>();
	} catch (tools::cannot_allocate&) {}

	try	{
		return build_event<R>();
	} catch (tools::cannot_allocate&) {}

	if (type == "USER"    ) return new module::Channel_user    <R>(this->N, this->path, this->add_users);
	if (type == "USER_ADD") return new module::Channel_user_add<R>(this->N, this->path, this->add_users);
	if (type == "USER_BEC") return new module::Channel_user_be <R>(this->N, this->path                 );
	if (type == "USER_BSC") return new module::Channel_user_bs <R>(this->N, this->path                 );
	if (type == "NO"      ) return new module::Channel_NO      <R>(this->N,             this->add_users);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename R>
module::Channel<R>* Channel
::build(const tools::Distributions<R>& dist) const
{
	try	{
		return build_userpdf<R>(dist);
	} catch (tools::cannot_allocate&) {}

	return build<R>();
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::module::Channel<R_32>* aff3ct::factory::Channel::build<R_32>() const;
template aff3ct::module::Channel<R_64>* aff3ct::factory::Channel::build<R_64>() const;
template aff3ct::module::Channel<R_32>* aff3ct::factory::Channel::build<R_32>(const tools::Distributions<R_32>&) const;
template aff3ct::module::Channel<R_64>* aff3ct::factory::Channel::build<R_64>(const tools::Distributions<R_64>&) const;
#else
template aff3ct::module::Channel<R>* aff3ct::factory::Channel::build<R>() const;
template aff3ct::module::Channel<R>* aff3ct::factory::Channel::build<R>(const tools::Distributions<R>&) const;
#endif
// ==================================================================================== explicit template instantiation
