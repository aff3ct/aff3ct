#include "Tools/Exception/exception.hpp"

#include "Module/Encoder/RA/Encoder_RA.hpp"

#include "Encoder_RA.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Encoder_RA::name   = "Encoder RA";
const std::string aff3ct::factory::Encoder_RA::prefix = "enc";

template <typename B>
module::Encoder<B>* Encoder_RA
::build(const parameters& params, const module::Interleaver<int> &itl)
{
	if (params.type == "RA") return new module::Encoder_RA<B>(params.K, params.N_cw, itl, params.n_frames);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

void Encoder_RA
::build_args(arg_map &req_args, arg_map &opt_args, const std::string p)
{
	Encoder::build_args(req_args, opt_args);
	Interleaver::build_args(req_args, opt_args, "itl");
	req_args.erase({"itl-size"    });
	opt_args.erase({"itl-fra", "F"});

	opt_args[{p+"-type"}][2] += ", RA";
}

void Encoder_RA
::store_args(const tools::Arguments_reader& ar, parameters &params, const std::string p)
{
	params.type = "RA";

	Encoder::store_args(ar, params);

	params.itl.size     = params.N_cw;
	params.itl.n_frames = params.n_frames;
	Interleaver::store_args(ar, params.itl, "itl");
}

void Encoder_RA
::header(params_list& head_enc, params_list& head_itl, const parameters& params)
{
	Encoder::header(head_enc, params);
	Interleaver::header(head_itl, params.itl);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Encoder<B_8 >* aff3ct::factory::Encoder_RA::build<B_8 >(const aff3ct::factory::Encoder_RA::parameters&, const aff3ct::module::Interleaver<int>&);
template aff3ct::module::Encoder<B_16>* aff3ct::factory::Encoder_RA::build<B_16>(const aff3ct::factory::Encoder_RA::parameters&, const aff3ct::module::Interleaver<int>&);
template aff3ct::module::Encoder<B_32>* aff3ct::factory::Encoder_RA::build<B_32>(const aff3ct::factory::Encoder_RA::parameters&, const aff3ct::module::Interleaver<int>&);
template aff3ct::module::Encoder<B_64>* aff3ct::factory::Encoder_RA::build<B_64>(const aff3ct::factory::Encoder_RA::parameters&, const aff3ct::module::Interleaver<int>&);
#else
template aff3ct::module::Encoder<B>* aff3ct::factory::Encoder_RA::build<B>(const aff3ct::factory::Encoder_RA::parameters&, const aff3ct::module::Interleaver<int>&);
#endif
// ==================================================================================== explicit template instantiation
