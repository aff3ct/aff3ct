#include "Tools/Exception/exception.hpp"
#include "Factory_flip_and_check.hpp"

using namespace aff3ct::tools;

template<typename B, typename Q>
Flip_and_check<B,Q>* Factory_flip_and_check
::build(const parameters& params, module::CRC<B> &crc)
{
	if (!params.enable)
		throw cannot_allocate(__FILE__, __LINE__, __func__);

	return new Flip_and_check<B,Q>(params.size, params.n_ite, crc, params.start_crc_check_ite, params.q, params.ite_min, params.ite_max, params.ite_step, params.n_frames);
}

void Factory_flip_and_check
::build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args)
{
	req_args[{"dec-fnc-size", "K"}] =
		{"positive_int",
		 "size (in bit) of the extrinsic for the fnc processing."};

	opt_args[{"dec-fnc"}] =
		{"",
		 "enables the flip and check decoder (requires \"--crc-type\")."};

	opt_args[{"dec-fnc-q"}] =
		{"positive_int",
		 "set the search's space for the fnc algorithm."};

	opt_args[{"dec-fnc-ite-m"}] =
		{"positive_int",
		 "set first iteration at which the fnc is used."};

	opt_args[{"dec-fnc-ite-M"}] =
		{"positive_int",
		 "set last iteration at which the fnc is used."};

	opt_args[{"dec-fnc-ite-s"}] =
		{"positive_int",
		 "set iteration step for the fnc algorithm."};

	opt_args[{"dec-ite", "i"}] =
		{"positive_int",
		 "maximal number of iterations in the turbo."};

	opt_args[{"dec-crc-ite"}] =
		{"positive_int",
		 "set the iteration to start the CRC checking."};

	opt_args[{"dec-fra", "F"}] =
		{"positive_int",
		 "set the number of inter frame level to process."};
}

void Factory_flip_and_check
::store_args(const Arguments_reader& ar, parameters &params)
{
	if(ar.exist_arg({"dec-fnc"})) params.enable = true;
	if(ar.exist_arg({"dec-fnc-size"})) params.size = ar.get_arg_int({"dec-fnc-size"});
	if(ar.exist_arg({"dec-fnc-q"})) params.q = ar.get_arg_int({"dec-fnc-q"});
	if(ar.exist_arg({"dec-fnc-ite-s"})) params.ite_step = ar.get_arg_int({"dec-fnc-ite-s"});
	if(ar.exist_arg({"dec-ite", "i"})) params.n_ite = ar.get_arg_int({"dec-ite", "i"});
	if(ar.exist_arg({"dec-fnc-ite-m"})) params.ite_min = ar.get_arg_int({"dec-fnc-ite-m"});
	if(ar.exist_arg({"dec-fnc-ite-M"})) params.ite_max = ar.get_arg_int({"dec-fnc-ite-M"});
	else                                params.ite_max = params.n_ite;
	if(ar.exist_arg({"dec-crc-ite"})) params.start_crc_check_ite = ar.get_arg_int({"dec-crc-ite"});
	if(ar.exist_arg({"dec-fra", "F"})) params.n_frames = ar.get_arg_int({"dec-fra", "F"});
}

void Factory_flip_and_check
::group_args(Arguments_reader::arg_grp& ar)
{
}

void Factory_flip_and_check
::header(params_list& head_fnc, const parameters& params)
{
	if (params.enable)
	{
		head_fnc.push_back(std::make_pair("Flip aNd Check (FNC)", ((params.enable) ? "on" : "off")));
		head_fnc.push_back(std::make_pair("FNC q", std::to_string(params.q)));
		head_fnc.push_back(std::make_pair("FNC ite min", std::to_string(params.ite_min)));
		head_fnc.push_back(std::make_pair("FNC ite max", std::to_string(params.ite_max)));
		head_fnc.push_back(std::make_pair("FNC ite step", std::to_string(params.ite_step)));
	}
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::tools::Flip_and_check<B_8 ,Q_8 >* aff3ct::tools::Factory_flip_and_check::build<B_8 ,Q_8 >(const aff3ct::tools::Factory_flip_and_check::parameters&, module::CRC<B_8 >&);
template aff3ct::tools::Flip_and_check<B_16,Q_16>* aff3ct::tools::Factory_flip_and_check::build<B_16,Q_16>(const aff3ct::tools::Factory_flip_and_check::parameters&, module::CRC<B_16>&);
template aff3ct::tools::Flip_and_check<B_32,Q_32>* aff3ct::tools::Factory_flip_and_check::build<B_32,Q_32>(const aff3ct::tools::Factory_flip_and_check::parameters&, module::CRC<B_32>&);
template aff3ct::tools::Flip_and_check<B_64,Q_64>* aff3ct::tools::Factory_flip_and_check::build<B_64,Q_64>(const aff3ct::tools::Factory_flip_and_check::parameters&, module::CRC<B_64>&);
#else
template aff3ct::tools::Flip_and_check<B,Q>* aff3ct::tools::Factory_flip_and_check::build<B,Q>(const aff3ct::tools::Factory_flip_and_check::parameters&, module::CRC<B>&);
#endif
// ==================================================================================== explicit template instantiation
