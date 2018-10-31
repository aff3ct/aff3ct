#include <vector>
#include <cstdint>
#include <sstream>
#include <typeinfo>
#include <typeindex>
#include <unordered_map>
#include <rang.hpp>
#include <date.h>

#include "Tools/general_utils.h"
#include "Tools/Exception/exception.hpp"
#include "Tools/types.h"
#include "Tools/version.h"

#include "Launcher/Launcher.hpp"
#include "Launcher/Code/BCH/BCH.hpp"
#include "Launcher/Code/LDPC/LDPC.hpp"
#include "Launcher/Code/Polar/Polar.hpp"
#include "Launcher/Code/RA/RA.hpp"
#include "Launcher/Code/Repetition/Repetition.hpp"
#include "Launcher/Code/RS/RS.hpp"
#include "Launcher/Code/RSC/RSC.hpp"
#include "Launcher/Code/RSC_DB/RSC_DB.hpp"
#include "Launcher/Code/Turbo/Turbo.hpp"
#include "Launcher/Code/Turbo_DB/Turbo_DB.hpp"
#include "Launcher/Code/Turbo_product/Turbo_product.hpp"
#include "Launcher/Code/Uncoded/Uncoded.hpp"
#include "Launcher/Simulation/BFER_ite.hpp"
#include "Launcher/Simulation/BFER_std.hpp"
#include "Launcher/Simulation/EXIT.hpp"

#include "Factory/Module/Codec/BCH/Codec_BCH.hpp"
#include "Factory/Module/Codec/LDPC/Codec_LDPC.hpp"
#include "Factory/Module/Codec/Polar/Codec_polar.hpp"
#include "Factory/Module/Codec/RA/Codec_RA.hpp"
#include "Factory/Module/Codec/Repetition/Codec_repetition.hpp"
#include "Factory/Module/Codec/RSC/Codec_RSC.hpp"
#include "Factory/Module/Codec/RSC_DB/Codec_RSC_DB.hpp"
#include "Factory/Module/Codec/Turbo/Codec_turbo.hpp"
#include "Factory/Module/Codec/Turbo_DB/Codec_turbo_DB.hpp"
#include "Factory/Module/Codec/Uncoded/Codec_uncoded.hpp"

#include "Launcher.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Launcher_name   = "Launcher";
const std::string aff3ct::factory::Launcher_prefix = "lch";

factory::Launcher::parameters
::parameters(const std::string &prefix)
: Factory::parameters(Launcher_name, Launcher_name, prefix)
{
}

factory::Launcher::parameters
::parameters(const std::string &name, const std::string &short_name, const std::string &prefix)
: Factory::parameters(name, short_name, prefix)
{
}

factory::Launcher::parameters* factory::Launcher::parameters
::clone() const
{
	return new Launcher::parameters(*this);
}

void factory::Launcher::parameters
::get_description(tools::Argument_map_info &args) const
{
	auto p = this->get_prefix();

	args.add(
		{p+"-cde-type", "C"},
		tools::Text(tools::Including_set("POLAR", "TURBO", "TURBO_DB", "TURBO_PROD", "LDPC", "REP", "RA", "RSC", "RSC_DB", "BCH", "UNCODED", "RS")),
		"select the code type you want to use.",
		tools::arg_rank::REQ);

	args.add(
		{p+"-type"},
#if !defined(AFF3CT_8BIT_PREC) && !defined(AFF3CT_16BIT_PREC)
		tools::Text(tools::Including_set("BFER", "BFERI", "EXIT")),
#else
		tools::Text(tools::Including_set("BFER", "BFERI")),
#endif
		"select the type of simulation to launch (default is BFER).");

#ifdef AFF3CT_MULTI_PREC
	args.add(
		{p+"-prec", "p"},
		tools::Integer(tools::Including_set(8, 16, 32, 64)),
		"the simulation precision in bits.");
#endif

	args.add(
		{"help", "h"},
		tools::None(),
		"print this help.");

	args.add(
		{"Help", "H"},
		tools::None(),
		"print this help with the advanced arguments.");

	args.add(
		{"version", "v"},
		tools::None(),
		"print informations about the version of the code.");

#ifdef AFF3CT_BACKTRACE
	args.add(
		{"except-no-bt"},
		tools::None(),
		"do not print the backtrace when displaying exception.",
		tools::arg_rank::ADV);
#endif

#ifndef NDEBUG
	args.add(
		{"except-a2l"},
		tools::None(),
		"enhance the backtrace when displaying exception by changing program addresses into "
		" file names and lines (may take some seconds).",
		tools::arg_rank::ADV);
#endif

	args.add(
		{p+"-no-legend"},
		tools::None(),
		"Do not display any legend when launching the simulation.",
		tools::arg_rank::ADV);


#ifdef AFF3CT_NO_COLOR
	args.add(
		{p+"-no-colors"},
		tools::None(),
		"disable the colors in the shell.");
#endif
}

void factory::Launcher::parameters
::store(const tools::Argument_map_value &vals)
{
	auto p = this->get_prefix();

	if(vals.exist({p+"-cde-type", "C"})) this->cde_type        = vals.at({p+"-cde-type", "C"}); // required
	if(vals.exist({p+"-type"         })) this->sim_type        = vals.at({p+"-type"         });
	if(vals.exist({"version",     "v"})) this->display_version = true;
	if(vals.exist({p+"-no-legend"    })) this->display_legend  = false;

	if(vals.exist({"help", "h"}))
	{
		this->display_help     = true;
		this->display_adv_help = false;
	}

	if(vals.exist({"Help", "H"}))
	{
		this->display_help     = true;
		this->display_adv_help = true;
	}

#ifdef AFF3CT_MULTI_PREC
	if(vals.exist({p+"-prec", "p"})) this->sim_prec = vals.to_int({p+"-prec", "p"});
#endif

	tools::exception::no_backtrace    =  vals.exist({"except-no-bt"});
	tools::exception::no_addr_to_line = !vals.exist({"except-a2l"  });

#ifdef AFF3CT_NO_COLOR
	if (vals.exist({p+"-no-colors"})) rang::setControlMode(rang::control::Off);
#else
	rang::setControlMode(rang::control::Off);
#endif
}

void factory::Launcher::parameters
::get_headers(std::map<std::string,header_list>& params_headers, const bool full) const
{
	auto p = this->get_prefix();

	std::unordered_map<std::type_index,std::string> type_names;
	// define type names
	type_names[typeid(int8_t )] = "int8";
	type_names[typeid(int16_t)] = "int16";
	type_names[typeid(int32_t)] = "int32";
	type_names[typeid(int64_t)] = "int64";
	type_names[typeid(float  )] = "float32";
	type_names[typeid(double )] = "float64";

	params_headers[p].push_back(std::make_pair("Type", this->sim_type));

#ifdef AFF3CT_MULTI_PREC
	std::type_index id_B = typeid(int32_t), id_R = typeid(float), id_Q = typeid(float);
	switch (this->sim_prec)
	{
		case 8:
			id_B = typeid(B_8);
			id_R = typeid(R_8);
			id_Q = typeid(Q_8);
		break;

		case 16:
			id_B = typeid(B_16);
			id_R = typeid(R_16);
			id_Q = typeid(Q_16);
		break;

		case 32:
			id_B = typeid(B_32);
			id_R = typeid(R_32);
			id_Q = typeid(Q_32);
		break;

		case 64:
			id_B = typeid(B_64);
			id_R = typeid(R_64);
			id_Q = typeid(Q_64);
		break;

		default :
			std::stringstream message;
			message << "Unsupported bit precision: " << this->sim_prec << ").";
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
		break;
	}
#else
	std::type_index id_B = typeid(B), id_R = typeid(R), id_Q = typeid(Q);
#endif

	params_headers[p].push_back(std::make_pair("Type of bits",  type_names[id_B]));
	params_headers[p].push_back(std::make_pair("Type of reals", type_names[id_R]));
//	if (std::is_integral<Q>::value) // do not works for int8_t, int16_t, etc.
	if (id_Q == typeid(int8_t) || id_Q == typeid(int16_t) || id_Q == typeid(int32_t) || id_Q == typeid(int64_t))
		params_headers[p].push_back(std::make_pair("Type of quant. reals", type_names[id_Q]));

	using namespace date;
	std::stringstream date;
	date << std::chrono::system_clock::now();
	auto split_date = tools::split(date.str(), '.');
	params_headers[p].push_back(std::make_pair("Date (UTC)", split_date[0]));
	if (version() != "GIT-NOTFOUND")
		params_headers[p].push_back(std::make_pair("Git version", version()));

	params_headers[p].push_back(std::make_pair("Code type (C)", this->cde_type));
}

template <typename B, typename R, typename Q>
launcher::Launcher* factory::Launcher::parameters
::build_exit(const int argc, const char **argv) const
{
	throw tools::cannot_allocate(__FILE__, __LINE__, __func__, "Unsupported code/simulation pair.");
}

namespace aff3ct
{
namespace factory
{
#if defined(AFF3CT_MULTI_PREC) || defined(AFF3CT_32BIT_PREC)
template <>
launcher::Launcher* Launcher::parameters
::build_exit<int32_t, float, float>(const int argc, const char **argv) const
{
	if (this->cde_type == "POLAR")
		if (this->sim_type == "EXIT")
			return new launcher::Polar<launcher::EXIT<int32_t,float>,int32_t,float>(argc, argv);

	if (this->cde_type == "RSC")
		if (this->sim_type == "EXIT")
			return new launcher::RSC<launcher::EXIT<int32_t,float>,int32_t,float>(argc, argv);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__, "Unsupported code/simulation pair.");
}
#endif

#if defined(AFF3CT_MULTI_PREC) || defined(AFF3CT_64BIT_PREC)
template <>
launcher::Launcher* Launcher::parameters
::build_exit<int64_t, double, double>(const int argc, const char **argv) const
{
	if (this->cde_type == "POLAR")
		if (this->sim_type == "EXIT")
			return new launcher::Polar<launcher::EXIT<int64_t,double>,int64_t,double>(argc, argv);

	if (this->cde_type == "RSC")
		if (this->sim_type == "EXIT")
			return new launcher::RSC<launcher::EXIT<int64_t,double>,int64_t,double>(argc, argv);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__, "Unsupported code/simulation pair.");
}
#endif
}
}

template <typename B, typename R, typename Q>
launcher::Launcher* factory::Launcher::parameters
::build(const int argc, const char **argv) const
{
	if (this->cde_type == "POLAR")
	{
		if (this->sim_type == "BFER" ) return new launcher::Polar<launcher::BFER_std<B,R,Q>,B,R,Q>(argc, argv);
		if (this->sim_type == "BFERI") return new launcher::Polar<launcher::BFER_ite<B,R,Q>,B,R,Q>(argc, argv);
	}

	if (this->cde_type == "RSC")
	{
		if (this->sim_type == "BFER" ) return new launcher::RSC<launcher::BFER_std<B,R,Q>,B,R,Q>(argc, argv);
		if (this->sim_type == "BFERI") return new launcher::RSC<launcher::BFER_ite<B,R,Q>,B,R,Q>(argc, argv);
	}

	if (this->cde_type == "RSC_DB")
	{
		if (this->sim_type == "BFER" ) return new launcher::RSC_DB<launcher::BFER_std<B,R,Q>,B,R,Q>(argc, argv);
		if (this->sim_type == "BFERI") return new launcher::RSC_DB<launcher::BFER_ite<B,R,Q>,B,R,Q>(argc, argv);
	}

	if (this->cde_type == "TURBO")
	{
		if (this->sim_type == "BFER") return new launcher::Turbo<launcher::BFER_std<B,R,Q>,B,R,Q>(argc, argv);
	}

	if (this->cde_type == "TURBO_DB")
	{
		if (this->sim_type == "BFER") return new launcher::Turbo_DB<launcher::BFER_std<B,R,Q>,B,R,Q>(argc, argv);
	}

	if (this->cde_type == "TURBO_PROD")
	{
		if (this->sim_type == "BFER") return new launcher::Turbo_product<launcher::BFER_std<B,R,Q>,B,R,Q>(argc, argv);
	}

	if (this->cde_type == "REP")
	{
		if (this->sim_type == "BFER") return new launcher::Repetition<launcher::BFER_std<B,R,Q>,B,R,Q>(argc, argv);
	}

	if (this->cde_type == "BCH")
	{
		if (this->sim_type == "BFER") return new launcher::BCH<launcher::BFER_std<B,R,Q>,B,R,Q>(argc, argv);
	}

	if (this->cde_type == "RS")
	{
		if (this->sim_type == "BFER") return new launcher::RS<launcher::BFER_std<B,R,Q>,B,R,Q>(argc, argv);
	}

	if (this->cde_type == "RA")
	{
		if (this->sim_type == "BFER") return new launcher::RA<launcher::BFER_std<B,R,Q>,B,R,Q>(argc, argv);
	}

	if (this->cde_type == "LDPC")
	{
		if (this->sim_type == "BFER" ) return new launcher::LDPC<launcher::BFER_std<B,R,Q>,B,R,Q>(argc, argv);
		if (this->sim_type == "BFERI") return new launcher::LDPC<launcher::BFER_ite<B,R,Q>,B,R,Q>(argc, argv);
	}

	if (this->cde_type == "UNCODED")
	{
		if (this->sim_type == "BFER" ) return new launcher::Uncoded<launcher::BFER_std<B,R,Q>,B,R,Q>(argc, argv);
		if (this->sim_type == "BFERI") return new launcher::Uncoded<launcher::BFER_ite<B,R,Q>,B,R,Q>(argc, argv);
	}

	return build_exit<B,R,Q>(argc, argv);
}

template <typename B, typename R, typename Q>
launcher::Launcher* factory::Launcher
::build(const parameters &params, const int argc, const char **argv)
{
	return params.template build<B,R,Q>(argc, argv);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::launcher::Launcher* aff3ct::factory::Launcher::parameters::build<B_8 ,R_8 ,Q_8 >(const int, const char**) const;
template aff3ct::launcher::Launcher* aff3ct::factory::Launcher::parameters::build<B_16,R_16,Q_16>(const int, const char**) const;
template aff3ct::launcher::Launcher* aff3ct::factory::Launcher::parameters::build<B_32,R_32,Q_32>(const int, const char**) const;
template aff3ct::launcher::Launcher* aff3ct::factory::Launcher::parameters::build<B_64,R_64,Q_64>(const int, const char**) const;
template aff3ct::launcher::Launcher* aff3ct::factory::Launcher::build<B_8 ,R_8 ,Q_8 >(const aff3ct::factory::Launcher::parameters&, const int, const char**);
template aff3ct::launcher::Launcher* aff3ct::factory::Launcher::build<B_16,R_16,Q_16>(const aff3ct::factory::Launcher::parameters&, const int, const char**);
template aff3ct::launcher::Launcher* aff3ct::factory::Launcher::build<B_32,R_32,Q_32>(const aff3ct::factory::Launcher::parameters&, const int, const char**);
template aff3ct::launcher::Launcher* aff3ct::factory::Launcher::build<B_64,R_64,Q_64>(const aff3ct::factory::Launcher::parameters&, const int, const char**);
#else
template aff3ct::launcher::Launcher* aff3ct::factory::Launcher::parameters::build<B,R,Q>(const int, const char**) const;
template aff3ct::launcher::Launcher* aff3ct::factory::Launcher::build<B,R,Q>(const aff3ct::factory::Launcher::parameters&, const int, const char**);
#endif
// ==================================================================================== explicit template instantiation
