#include "Tools/Exception/exception.hpp"

#include "Module/Encoder/RA/Encoder_RA.hpp"

#include "Encoder_RA.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

template <typename B>
module::Encoder<B>* Encoder_RA
::build(const parameters& params, const module::Interleaver<int> &itl)
{
	if (params.type == "RA") return new module::Encoder_RA<B>(params.K, params.N_cw, itl, params.n_frames);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

void Encoder_RA
::build_args(arg_map &req_args, arg_map &opt_args)
{
	Encoder::build_args(req_args, opt_args);

	opt_args[{"enc-type"}][2] += ", RA";
}

void Encoder_RA
::store_args(const tools::Arguments_reader& ar, Encoder::parameters &params)
{
	params.type = "RA";

	Encoder::store_args(ar, params);
}

void Encoder_RA
::group_args(arg_grp& ar)
{
	Encoder::group_args(ar);
}

void Encoder_RA
::header(params_list& head_enc, const Encoder::parameters& params)
{
	Encoder::header(head_enc, params);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Encoder<B_8 >* aff3ct::factory::Encoder_RA::build<B_8 >(const aff3ct::factory::Encoder::parameters&, const aff3ct::module::Interleaver<int>&);
template aff3ct::module::Encoder<B_16>* aff3ct::factory::Encoder_RA::build<B_16>(const aff3ct::factory::Encoder::parameters&, const aff3ct::module::Interleaver<int>&);
template aff3ct::module::Encoder<B_32>* aff3ct::factory::Encoder_RA::build<B_32>(const aff3ct::factory::Encoder::parameters&, const aff3ct::module::Interleaver<int>&);
template aff3ct::module::Encoder<B_64>* aff3ct::factory::Encoder_RA::build<B_64>(const aff3ct::factory::Encoder::parameters&, const aff3ct::module::Interleaver<int>&);
#else
template aff3ct::module::Encoder<B>* aff3ct::factory::Encoder_RA::build<B>(const aff3ct::factory::Encoder::parameters&, const aff3ct::module::Interleaver<int>&);
#endif
// ==================================================================================== explicit template instantiation
