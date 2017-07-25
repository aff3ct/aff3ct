#include "Tools/Exception/exception.hpp"

#include "Module/Encoder/Repetition/Encoder_repetition_sys.hpp"

#include "Encoder_repetition.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Encoder_repetition::name   = "Encoder Repetiton";
const std::string aff3ct::factory::Encoder_repetition::prefix = "enc";

template <typename B>
module::Encoder_sys<B>* Encoder_repetition
::build(const parameters &params)
{
	if (params.type == "REPETITION") return new module::Encoder_repetition_sys<B>(params.K, params.N_cw, params.buffered, params.n_frames);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

void Encoder_repetition
::build_args(arg_map &req_args, arg_map &opt_args, const std::string p)
{
	Encoder::build_args(req_args, opt_args, p);

	opt_args[{p+"-type"}][2] += ", REPETITION";

	opt_args[{p+"-no-buff"}] =
		{"",
		 "disable the buffered encoding."};
}

void Encoder_repetition
::store_args(const arg_val_map &vals, parameters &params, const std::string p)
{
	params.type = "REPETITION";

	Encoder::store_args(vals, params, p);

	if(exist(vals, {p+"-no-buff"})) params.buffered = false;
}

void Encoder_repetition
::make_header(params_list& head_enc, const parameters& params)
{
	Encoder::make_header(head_enc, params);

	head_enc.push_back(std::make_pair("Buffered", (params.buffered ? "on" : "off")));
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Encoder_sys<B_8 >* aff3ct::factory::Encoder_repetition::build<B_8 >(const aff3ct::factory::Encoder_repetition::parameters&);
template aff3ct::module::Encoder_sys<B_16>* aff3ct::factory::Encoder_repetition::build<B_16>(const aff3ct::factory::Encoder_repetition::parameters&);
template aff3ct::module::Encoder_sys<B_32>* aff3ct::factory::Encoder_repetition::build<B_32>(const aff3ct::factory::Encoder_repetition::parameters&);
template aff3ct::module::Encoder_sys<B_64>* aff3ct::factory::Encoder_repetition::build<B_64>(const aff3ct::factory::Encoder_repetition::parameters&);
#else
template aff3ct::module::Encoder_sys<B>* aff3ct::factory::Encoder_repetition::build<B>(const aff3ct::factory::Encoder_repetition::parameters&);
#endif
// ==================================================================================== explicit template instantiation

