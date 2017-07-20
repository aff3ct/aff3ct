#include "Tools/Exception/exception.hpp"

#include "Module/Decoder/Repetition/Decoder_repetition_std.hpp"
#include "Module/Decoder/Repetition/Decoder_repetition_fast.hpp"

#include "Decoder_repetition.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

template <typename B, typename R>
module::Decoder<B,R>* Decoder_repetition
::build(const parameters &params)
{
	if (params.type == "REPETITION")
	{
		     if (params.implem == "STD" ) return new module::Decoder_repetition_std <B,R>(params.K, params.N_cw, params.buffered, params.n_frames);
		else if (params.implem == "FAST") return new module::Decoder_repetition_fast<B,R>(params.K, params.N_cw, params.buffered, params.n_frames);
	}

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

void Decoder_repetition
::build_args(arg_map &req_args, arg_map &opt_args)
{
	Decoder::build_args(req_args, opt_args);

	opt_args[{"dec-type", "D"}].push_back("REPETITION");
	opt_args[{"dec-implem"}].push_back("STD, FAST");

	opt_args[{"dec-no-buff"}] =
		{"",
		 "does not suppose a buffered encoding."};
}

void Decoder_repetition
::store_args(const tools::Arguments_reader& ar, parameters &params)
{
	params.type   = "REPETITION";
	params.implem = "STD";

	Decoder::store_args(ar, params);

	if(ar.exist_arg({"dec-no-buff"})) params.buffered = false;
}

void Decoder_repetition
::group_args(arg_grp& ar)
{
	Decoder::group_args(ar);
}

void Decoder_repetition
::header(params_list& head_dec, const parameters& params)
{
	Decoder::header(head_dec, params);

	head_dec.push_back(std::make_pair("Buffered", (params.buffered ? "on" : "off")));
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Decoder<B_8 ,Q_8 >* aff3ct::factory::Decoder_repetition::build<B_8 ,Q_8 >(const aff3ct::factory::Decoder_repetition::parameters&);
template aff3ct::module::Decoder<B_16,Q_16>* aff3ct::factory::Decoder_repetition::build<B_16,Q_16>(const aff3ct::factory::Decoder_repetition::parameters&);
template aff3ct::module::Decoder<B_32,Q_32>* aff3ct::factory::Decoder_repetition::build<B_32,Q_32>(const aff3ct::factory::Decoder_repetition::parameters&);
template aff3ct::module::Decoder<B_64,Q_64>* aff3ct::factory::Decoder_repetition::build<B_64,Q_64>(const aff3ct::factory::Decoder_repetition::parameters&);
#else
template aff3ct::module::Decoder<B,Q>* aff3ct::factory::Decoder_repetition::build<B,Q>(const aff3ct::factory::Decoder_repetition::parameters&);
#endif
// ==================================================================================== explicit template instantiation
