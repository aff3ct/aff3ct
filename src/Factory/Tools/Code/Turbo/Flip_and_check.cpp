#include "Tools/Exception/exception.hpp"

#include "Flip_and_check.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Flip_and_check::name   = "Flip and check";
const std::string aff3ct::factory::Flip_and_check::prefix = "fnc";

template<typename B, typename Q>
tools::Flip_and_check<B,Q>* Flip_and_check
::build(const parameters& params, module::CRC<B> &crc)
{
	if (!params.enable)
		throw tools::cannot_allocate(__FILE__, __LINE__, __func__);

	return new tools::Flip_and_check<B,Q>(params.size, params.n_ite, crc, params.start_crc_check_ite, params.q, params.ite_min, params.ite_max, params.ite_step, params.n_frames);
}

void Flip_and_check
::build_args(arg_map &req_args, arg_map &opt_args, const std::string p)
{
	req_args[{p+"-size", "K"}] =
		{"positive_int",
		 "size (in bit) of the extrinsic for the fnc processing."};

	opt_args[{p}] =
		{"",
		 "enables the flip and check decoder (requires \"--crc-type\")."};

	opt_args[{p+"-q"}] =
		{"positive_int",
		 "set the search's space for the fnc algorithm."};

	opt_args[{p+"-ite-m"}] =
		{"positive_int",
		 "set first iteration at which the fnc is used."};

	opt_args[{p+"-ite-M"}] =
		{"positive_int",
		 "set last iteration at which the fnc is used."};

	opt_args[{p+"-ite-s"}] =
		{"positive_int",
		 "set iteration step for the fnc algorithm."};

	opt_args[{p+"-ite", "i"}] =
		{"positive_int",
		 "maximal number of iterations in the turbo."};

	opt_args[{p+"-crc-ite"}] =
		{"positive_int",
		 "set the iteration to start the CRC checking."};

	opt_args[{p+"-fra", "F"}] =
		{"positive_int",
		 "set the number of inter frame level to process."};
}

void Flip_and_check
::store_args(const arg_val_map &vals, parameters &params, const std::string p)
{
	if(exist(vals, {p             })) params.enable              = true;
	if(exist(vals, {p+"-size"     })) params.size                = std::stoi(vals.at({p+"-size"     }));
	if(exist(vals, {p+"-q"        })) params.q                   = std::stoi(vals.at({p+"-q"        }));
	if(exist(vals, {p+"-ite-s"    })) params.ite_step            = std::stoi(vals.at({p+"-ite-s"    }));
	if(exist(vals, {p+"-ite",  "i"})) params.n_ite               = std::stoi(vals.at({p+"-ite",  "i"}));
	if(exist(vals, {p+"-ite-m"    })) params.ite_min             = std::stoi(vals.at({p+"-ite-m"    }));
	if(exist(vals, {p+"-ite-M"    })) params.ite_max             = std::stoi(vals.at({p+"-ite-M"    }));
	else                              params.ite_max             = params.n_ite;
	if(exist(vals, {p+"-crc-ite"  })) params.start_crc_check_ite = std::stoi(vals.at({p+"-crc-ite"  }));
	if(exist(vals, {p+"-fra",  "F"})) params.n_frames            = std::stoi(vals.at({p+"-fra", "F" }));
}

void Flip_and_check
::make_header(params_list& head_fnc, const parameters& params, const bool full)
{
	head_fnc.push_back(std::make_pair("Flip aNd Check (FNC)", ((params.enable) ? "on" : "off")));
	if (params.enable)
	{
		head_fnc.push_back(std::make_pair("FNC q", std::to_string(params.q)));
		head_fnc.push_back(std::make_pair("FNC ite min", std::to_string(params.ite_min)));
		head_fnc.push_back(std::make_pair("FNC ite max", std::to_string(params.ite_max)));
		head_fnc.push_back(std::make_pair("FNC ite step", std::to_string(params.ite_step)));
	}
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::tools::Flip_and_check<B_8 ,Q_8 >* aff3ct::factory::Flip_and_check::build<B_8 ,Q_8 >(const aff3ct::factory::Flip_and_check::parameters&, module::CRC<B_8 >&);
template aff3ct::tools::Flip_and_check<B_16,Q_16>* aff3ct::factory::Flip_and_check::build<B_16,Q_16>(const aff3ct::factory::Flip_and_check::parameters&, module::CRC<B_16>&);
template aff3ct::tools::Flip_and_check<B_32,Q_32>* aff3ct::factory::Flip_and_check::build<B_32,Q_32>(const aff3ct::factory::Flip_and_check::parameters&, module::CRC<B_32>&);
template aff3ct::tools::Flip_and_check<B_64,Q_64>* aff3ct::factory::Flip_and_check::build<B_64,Q_64>(const aff3ct::factory::Flip_and_check::parameters&, module::CRC<B_64>&);
#else
template aff3ct::tools::Flip_and_check<B,Q>* aff3ct::factory::Flip_and_check::build<B,Q>(const aff3ct::factory::Flip_and_check::parameters&, module::CRC<B>&);
#endif
// ==================================================================================== explicit template instantiation
