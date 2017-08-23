#include "Tools/Exception/exception.hpp"

#include "Module/Decoder/Repetition/Decoder_repetition_std.hpp"
#include "Module/Decoder/Repetition/Decoder_repetition_fast.hpp"

#include "Decoder_repetition.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Decoder_repetition::name   = "Decoder Repetition";
const std::string aff3ct::factory::Decoder_repetition::prefix = "dec";

template <typename B, typename R>
module::Decoder_SIHO<B,R>* Decoder_repetition::parameters
::build() const
{
	if (this->type == "REPETITION")
	{
		     if (this->implem == "STD" ) return new module::Decoder_repetition_std <B,R>(this->K, this->N_cw, this->buffered, this->n_frames);
		else if (this->implem == "FAST") return new module::Decoder_repetition_fast<B,R>(this->K, this->N_cw, this->buffered, this->n_frames);
	}

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename R>
module::Decoder_SIHO<B,R>* Decoder_repetition
::build(const parameters &params)
{
	return params.template build<B,R>();
}

void Decoder_repetition
::build_args(arg_map &req_args, arg_map &opt_args, const std::string p)
{
	Decoder::build_args(req_args, opt_args, p);

	opt_args[{p+"-type", "D"}].push_back("REPETITION");
	opt_args[{p+"-implem"   }].push_back("STD, FAST");

	opt_args[{p+"-no-buff"}] =
		{"",
		 "does not suppose a buffered encoding."};
}

void Decoder_repetition
::store_args(const arg_val_map &vals, parameters &params, const std::string p)
{
	params.type   = "REPETITION";
	params.implem = "STD";

	Decoder::store_args(vals, params, p);

	if(exist(vals, {p+"-no-buff"})) params.buffered = false;
}

void Decoder_repetition
::make_header(params_list& head_dec, const parameters& params, const bool full)
{
	Decoder::make_header(head_dec, params, full);

	if (full) head_dec.push_back(std::make_pair("Buffered", (params.buffered ? "on" : "off")));
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Decoder_SIHO<B_8 ,Q_8 >* aff3ct::factory::Decoder_repetition::parameters::build<B_8 ,Q_8 >() const;
template aff3ct::module::Decoder_SIHO<B_16,Q_16>* aff3ct::factory::Decoder_repetition::parameters::build<B_16,Q_16>() const;
template aff3ct::module::Decoder_SIHO<B_32,Q_32>* aff3ct::factory::Decoder_repetition::parameters::build<B_32,Q_32>() const;
template aff3ct::module::Decoder_SIHO<B_64,Q_64>* aff3ct::factory::Decoder_repetition::parameters::build<B_64,Q_64>() const;
template aff3ct::module::Decoder_SIHO<B_8 ,Q_8 >* aff3ct::factory::Decoder_repetition::build<B_8 ,Q_8 >(const aff3ct::factory::Decoder_repetition::parameters&);
template aff3ct::module::Decoder_SIHO<B_16,Q_16>* aff3ct::factory::Decoder_repetition::build<B_16,Q_16>(const aff3ct::factory::Decoder_repetition::parameters&);
template aff3ct::module::Decoder_SIHO<B_32,Q_32>* aff3ct::factory::Decoder_repetition::build<B_32,Q_32>(const aff3ct::factory::Decoder_repetition::parameters&);
template aff3ct::module::Decoder_SIHO<B_64,Q_64>* aff3ct::factory::Decoder_repetition::build<B_64,Q_64>(const aff3ct::factory::Decoder_repetition::parameters&);
#else
template aff3ct::module::Decoder_SIHO<B,Q>* aff3ct::factory::Decoder_repetition::parameters::build<B,Q>() const;
template aff3ct::module::Decoder_SIHO<B,Q>* aff3ct::factory::Decoder_repetition::build<B,Q>(const aff3ct::factory::Decoder_repetition::parameters&);
#endif
// ==================================================================================== explicit template instantiation
