#include <sstream>

#include "Tools/Exception/exception.hpp"

#include "Module/Decoder/NO/Decoder_NO.hpp"

#include "Decoder_NO.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

template <typename B, typename Q>
module::Decoder_SISO<B,Q>* Decoder_NO
::build(const parameters &params)
{
	if (params.type == "NONE" && params.implem == "HARD_DECISION") return new module::Decoder_NO<B,Q>(params.K, params.n_frames);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

void Decoder_NO
::build_args(arg_map &req_args, arg_map &opt_args)
{
	Decoder::build_args(req_args, opt_args);

	opt_args[{"dec-type", "D"}].push_back("NONE");
	opt_args[{"dec-implem"}].push_back("HARD_DECISION");
}

void Decoder_NO
::store_args(const tools::Arguments_reader& ar, parameters &params)
{
	params.type   = "NONE";
	params.implem = "HARD_DECISION";

	Decoder::store_args(ar, params);
}

void Decoder_NO
::group_args(arg_grp& ar)
{
	Decoder::group_args(ar);
}

void Decoder_NO
::header(params_list& head_dec, const Decoder::parameters& params)
{
	Decoder::header(head_dec, params);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Decoder_SISO<B_8 ,Q_8 >* aff3ct::factory::Decoder_NO::build<B_8 ,Q_8 >(const aff3ct::factory::Decoder::parameters&);
template aff3ct::module::Decoder_SISO<B_16,Q_16>* aff3ct::factory::Decoder_NO::build<B_16,Q_16>(const aff3ct::factory::Decoder::parameters&);
template aff3ct::module::Decoder_SISO<B_32,Q_32>* aff3ct::factory::Decoder_NO::build<B_32,Q_32>(const aff3ct::factory::Decoder::parameters&);
template aff3ct::module::Decoder_SISO<B_64,Q_64>* aff3ct::factory::Decoder_NO::build<B_64,Q_64>(const aff3ct::factory::Decoder::parameters&);
#else
template aff3ct::module::Decoder_SISO<B,Q>* aff3ct::factory::Decoder_NO::build<B,Q>(const aff3ct::factory::Decoder::parameters&);
#endif
// ==================================================================================== explicit template instantiation
