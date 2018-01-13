#include "Tools/Exception/exception.hpp"

#include "Module/Encoder/Repetition/Encoder_repetition_sys.hpp"

#include "Encoder_repetition.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Encoder_repetition_name   = "Encoder Repetiton";
const std::string aff3ct::factory::Encoder_repetition_prefix = "enc";

Encoder_repetition::parameters
::parameters(const std::string &prefix)
: Encoder::parameters(Encoder_repetition_name, prefix)
{
	this->type = "REPETITION";
}

Encoder_repetition::parameters
::~parameters()
{
}

Encoder_repetition::parameters* Encoder_repetition::parameters
::clone() const
{
	return new Encoder_repetition::parameters(*this);
}

void Encoder_repetition::parameters
::get_description(arg_map &req_args, arg_map &opt_args) const
{
	Encoder::parameters::get_description(req_args, opt_args);

	auto p = this->get_prefix();

	opt_args[{p+"-type"}][2] += ", REPETITION";

	opt_args[{p+"-no-buff"}] =
		{"",
		 "disable the buffered encoding."};
}

void Encoder_repetition::parameters
::store(const arg_val_map &vals)
{
	Encoder::parameters::store(vals);

	auto p = this->get_prefix();

	if(exist(vals, {p+"-no-buff"})) this->buffered = false;
}

void Encoder_repetition::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	Encoder::parameters::get_headers(headers, full);

	auto p = this->get_prefix();

	headers[p].push_back(std::make_pair("Buffered", (this->buffered ? "on" : "off")));
}

template <typename B>
module::Encoder<B>* Encoder_repetition::parameters
::build() const
{
	if (this->type == "REPETITION") return new module::Encoder_repetition_sys<B>(this->K, this->N_cw, this->buffered, this->n_frames);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B>
module::Encoder<B>* Encoder_repetition
::build(const parameters &params)
{
	return params.template build<B>();
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Encoder<B_8 >* aff3ct::factory::Encoder_repetition::parameters::build<B_8 >() const;
template aff3ct::module::Encoder<B_16>* aff3ct::factory::Encoder_repetition::parameters::build<B_16>() const;
template aff3ct::module::Encoder<B_32>* aff3ct::factory::Encoder_repetition::parameters::build<B_32>() const;
template aff3ct::module::Encoder<B_64>* aff3ct::factory::Encoder_repetition::parameters::build<B_64>() const;
template aff3ct::module::Encoder<B_8 >* aff3ct::factory::Encoder_repetition::build<B_8 >(const aff3ct::factory::Encoder_repetition::parameters&);
template aff3ct::module::Encoder<B_16>* aff3ct::factory::Encoder_repetition::build<B_16>(const aff3ct::factory::Encoder_repetition::parameters&);
template aff3ct::module::Encoder<B_32>* aff3ct::factory::Encoder_repetition::build<B_32>(const aff3ct::factory::Encoder_repetition::parameters&);
template aff3ct::module::Encoder<B_64>* aff3ct::factory::Encoder_repetition::build<B_64>(const aff3ct::factory::Encoder_repetition::parameters&);
#else
template aff3ct::module::Encoder<B>* aff3ct::factory::Encoder_repetition::parameters::build<B>() const;
template aff3ct::module::Encoder<B>* aff3ct::factory::Encoder_repetition::build<B>(const aff3ct::factory::Encoder_repetition::parameters&);
#endif
// ==================================================================================== explicit template instantiation

