#include "Tools/Exception/exception.hpp"

#include "Module/Decoder/Repetition/Decoder_repetition_std.hpp"
#include "Module/Decoder/Repetition/Decoder_repetition_fast.hpp"
#include "Module/Decoder/ML/Decoder_maximum_likelihood.hpp"

#include "Decoder_repetition.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Decoder_repetition_name   = "Decoder Repetition";
const std::string aff3ct::factory::Decoder_repetition_prefix = "dec";

Decoder_repetition::parameters
::parameters(const std::string &prefix)
: Decoder::parameters(Decoder_repetition_name, prefix)
{
	this->type   = "REPETITION";
	this->implem = "STD";
}

Decoder_repetition::parameters
::~parameters()
{
}

Decoder_repetition::parameters* Decoder_repetition::parameters
::clone() const
{
	return new Decoder_repetition::parameters(*this);
}

void Decoder_repetition::parameters
::get_description(arg_map &req_args, arg_map &opt_args) const
{
	Decoder::parameters::get_description(req_args, opt_args);

	auto p = this->get_prefix();

	opt_args[{p+"-type", "D"}][2] += ", REPETITION";
	opt_args[{p+"-implem"   }].push_back("STD, FAST");

	opt_args[{p+"-no-buff"}] =
		{"",
		 "does not suppose a buffered encoding."};
}

void Decoder_repetition::parameters
::store(const arg_val_map &vals)
{
	Decoder::parameters::store(vals);

	auto p = this->get_prefix();

	if(exist(vals, {p+"-no-buff"})) this->buffered = false;
}

void Decoder_repetition::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	Decoder::parameters::get_headers(headers, full);

	if (this->type != "ML")
	{
		auto p = this->get_prefix();

		if (full) headers[p].push_back(std::make_pair("Buffered", (this->buffered ? "on" : "off")));
	}
}

template <typename B, typename Q>
module::Decoder_SIHO<B,Q>* Decoder_repetition::parameters
::build(module::Encoder_repetition_sys<B> *encoder) const
{
	try
	{
		return Decoder::parameters::build<B,Q>(encoder);
	}
	catch (tools::cannot_allocate const&)
	{
		if (this->type == "REPETITION")
		{
			     if (this->implem == "STD" ) return new module::Decoder_repetition_std <B,Q>(this->K, this->N_cw, this->buffered, this->n_frames);
			else if (this->implem == "FAST") return new module::Decoder_repetition_fast<B,Q>(this->K, this->N_cw, this->buffered, this->n_frames);
		}
	}

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q>
module::Decoder_SIHO<B,Q>* Decoder_repetition
::build(const parameters &params, module::Encoder_repetition_sys<B> *encoder)
{
	return params.template build<B,Q>(encoder);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Decoder_SIHO<B_8 ,Q_8 >* aff3ct::factory::Decoder_repetition::parameters::build<B_8 ,Q_8 >(module::Encoder_repetition_sys<B_8 >*) const;
template aff3ct::module::Decoder_SIHO<B_16,Q_16>* aff3ct::factory::Decoder_repetition::parameters::build<B_16,Q_16>(module::Encoder_repetition_sys<B_16>*) const;
template aff3ct::module::Decoder_SIHO<B_32,Q_32>* aff3ct::factory::Decoder_repetition::parameters::build<B_32,Q_32>(module::Encoder_repetition_sys<B_32>*) const;
template aff3ct::module::Decoder_SIHO<B_64,Q_64>* aff3ct::factory::Decoder_repetition::parameters::build<B_64,Q_64>(module::Encoder_repetition_sys<B_64>*) const;
template aff3ct::module::Decoder_SIHO<B_8 ,Q_8 >* aff3ct::factory::Decoder_repetition::build<B_8 ,Q_8 >(const aff3ct::factory::Decoder_repetition::parameters&, module::Encoder_repetition_sys<B_8 >*);
template aff3ct::module::Decoder_SIHO<B_16,Q_16>* aff3ct::factory::Decoder_repetition::build<B_16,Q_16>(const aff3ct::factory::Decoder_repetition::parameters&, module::Encoder_repetition_sys<B_16>*);
template aff3ct::module::Decoder_SIHO<B_32,Q_32>* aff3ct::factory::Decoder_repetition::build<B_32,Q_32>(const aff3ct::factory::Decoder_repetition::parameters&, module::Encoder_repetition_sys<B_32>*);
template aff3ct::module::Decoder_SIHO<B_64,Q_64>* aff3ct::factory::Decoder_repetition::build<B_64,Q_64>(const aff3ct::factory::Decoder_repetition::parameters&, module::Encoder_repetition_sys<B_64>*);
#else
template aff3ct::module::Decoder_SIHO<B,Q>* aff3ct::factory::Decoder_repetition::parameters::build<B,Q>(module::Encoder_repetition_sys<B>*) const;
template aff3ct::module::Decoder_SIHO<B,Q>* aff3ct::factory::Decoder_repetition::build<B,Q>(const aff3ct::factory::Decoder_repetition::parameters&, module::Encoder_repetition_sys<B>*);
#endif
// ==================================================================================== explicit template instantiation
