#include <vector>
#include <cstdint>
#include <sstream>
#include <typeinfo>
#include <typeindex>
#include <unordered_map>

#include "Tools/general_utils.h"
#include "Tools/Exception/exception.hpp"
#include "Tools/types.h"
#include "Tools/date.h"
#include "Tools/version.h"

#include "Launcher/Launcher.hpp"
#include "Launcher/Code/BCH/BCH.hpp"
#include "Launcher/Code/LDPC/LDPC.hpp"
#include "Launcher/Code/Polar/Polar.hpp"
#include "Launcher/Code/RA/RA.hpp"
#include "Launcher/Code/Repetition/Repetition.hpp"
#include "Launcher/Code/RSC/RSC.hpp"
#include "Launcher/Code/RSC_DB/RSC_DB.hpp"
#include "Launcher/Code/Turbo/Turbo.hpp"
#include "Launcher/Code/Turbo_DB/Turbo_DB.hpp"
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

factory::Launcher::parameters
::~parameters()
{
}

factory::Launcher::parameters* factory::Launcher::parameters
::clone() const
{
	return new Launcher::parameters(*this);
}

void factory::Launcher::parameters
::get_description(arg_map &req_args, arg_map &opt_args) const
{
	auto p = this->get_prefix();

	req_args[{p+"-cde-type", "C"}] =
		{"string",
		 "select the code type you want to use.",
		 "POLAR, TURBO, TURBO_DB, LDPC, REP, RA, RSC, RSC_DB, BCH, UNCODED"};

	opt_args[{p+"-type"}] =
		{"string",
		 "select the type of simulation to launch (default is BFER).",
		 "BFER, BFERI"};

#if !defined(PREC_8_BIT) && !defined(PREC_16_BIT)
		 opt_args[{p+"-type"}][2] += ", EXIT";
#endif

#ifdef MULTI_PREC
	opt_args[{p+"-prec", "p"}] =
		{"positive_int",
		 "the simulation precision in bit.",
		 "8, 16, 32, 64"};
#endif

	opt_args[{"help", "h"}] =
		{"",
		 "print this help."};

	opt_args[{"version", "v"}] =
		{"",
		 "print informations about the version of the code."};
}

void factory::Launcher::parameters
::store(const arg_val_map &vals)
{
	auto p = this->get_prefix();

	if(exist(vals, {p+"-cde-type", "C"})) this->cde_type        = vals.at({p+"-cde-type", "C"}); // required
	if(exist(vals, {p+"-type"         })) this->sim_type        = vals.at({p+"-type"         });
	if(exist(vals, {"help",        "h"})) this->display_help    = true;
	if(exist(vals, {"version",     "v"})) this->display_version = true;

#ifdef MULTI_PREC
	if(exist(vals, {p+"-prec", "p"})) this->sim_prec = std::stoi(vals.at({p+"-prec", "p"}));
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

#ifdef MULTI_PREC
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
#if defined(MULTI_PREC) || defined(PREC_32_BIT)
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

#if defined(MULTI_PREC) || defined(PREC_64_BIT)
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
		if (this->sim_type == "BFER")
			return new launcher::Polar<launcher::BFER_std<B,R,Q>,B,R,Q>(argc, argv);
		else if (this->sim_type == "BFERI")
			return new launcher::Polar<launcher::BFER_ite<B,R,Q>,B,R,Q>(argc, argv);
	}

	if (this->cde_type == "RSC")
	{
		if (this->sim_type == "BFER")
			return new launcher::RSC<launcher::BFER_std<B,R,Q>,B,R,Q>(argc, argv);
		else if (this->sim_type == "BFERI")
			return new launcher::RSC<launcher::BFER_ite<B,R,Q>,B,R,Q>(argc, argv);
	}

	if (this->cde_type == "RSC_DB")
	{
		if (this->sim_type == "BFER")
			return new launcher::RSC_DB<launcher::BFER_std<B,R,Q>,B,R,Q>(argc, argv);
		else if (this->sim_type == "BFERI")
			return new launcher::RSC_DB<launcher::BFER_ite<B,R,Q>,B,R,Q>(argc, argv);
	}

	if (this->cde_type == "TURBO")
	{
		if (this->sim_type == "BFER")
			return new launcher::Turbo<launcher::BFER_std<B,R,Q>,B,R,Q>(argc, argv);
	}

	if (this->cde_type == "TURBO_DB")
	{
		if (this->sim_type == "BFER")
			return new launcher::Turbo_DB<launcher::BFER_std<B,R,Q>,B,R,Q>(argc, argv);
	}

	if (this->cde_type == "REP")
	{
		if (this->sim_type == "BFER")
			return new launcher::Repetition<launcher::BFER_std<B,R,Q>,B,R,Q>(argc, argv);
	}

	if (this->cde_type == "BCH")
	{
		if (this->sim_type == "BFER")
			return new launcher::BCH<launcher::BFER_std<B,R,Q>,B,R,Q>(argc, argv);
	}

	if (this->cde_type == "RA")
	{
		if (this->sim_type == "BFER")
			return new launcher::RA<launcher::BFER_std<B,R,Q>,B,R,Q>(argc, argv);
	}

	if (this->cde_type == "LDPC")
	{
		if (this->sim_type == "BFER")
			return new launcher::LDPC<launcher::BFER_std<B,R,Q>,B,R,Q>(argc, argv);
		else if (this->sim_type == "BFERI")
			return new launcher::LDPC<launcher::BFER_ite<B,R,Q>,B,R,Q>(argc, argv);
	}

	if (this->cde_type == "UNCODED")
	{
		if (this->sim_type == "BFER")
			return new launcher::Uncoded<launcher::BFER_std<B,R,Q>,B,R,Q>(argc, argv);
		else if (this->sim_type == "BFERI")
			return new launcher::Uncoded<launcher::BFER_ite<B,R,Q>,B,R,Q>(argc, argv);
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
#ifdef MULTI_PREC
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
