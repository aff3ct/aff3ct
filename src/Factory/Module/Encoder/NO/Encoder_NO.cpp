#include "Tools/Exception/exception.hpp"

#include "Module/Encoder/NO/Encoder_NO.hpp"

#include "Encoder_NO.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Encoder_NO_name   = "Encoder NO";
const std::string aff3ct::factory::Encoder_NO_prefix = "enc";

Encoder_NO::parameters
::parameters(const std::string &prefix)
: Encoder::parameters(Encoder_NO_name, prefix)
{
	this->type = "NO";
}

Encoder_NO::parameters* Encoder_NO::parameters
::clone() const
{
	return new Encoder_NO::parameters(*this);
}

void Encoder_NO::parameters
::get_description(tools::Argument_map_info &args) const
{
	auto p = this->get_prefix();
	const std::string class_name = "factory::Encoder_NO::parameters::";

	// args.add(
	// 	{p+"-info-bits", "K"},
	// 	tools::Integer(tools::Positive(), tools::Non_zero()),
	// 	"useful number of bit transmitted (information bits).",
	// 	tools::arg_rank::REQ);
	add_arg(args, p, class_name+"p+info-bits,K",
		tools::Integer(tools::Positive(), tools::Non_zero()));

	// args.add(
	// 	{p+"-fra", "F"},
	// 	tools::Integer(tools::Positive(), tools::Non_zero()),
	// 	"set the number of inter frame level to process.");
	add_arg(args, p, class_name+"p+fra,F",
		tools::Integer(tools::Positive(), tools::Non_zero()));

	// args.add(
	// 	{p+"-type"},
	// 	tools::Text(tools::Including_set("NO")),
	// 	"type of the encoder to use in the simulation.");
	add_arg(args, p, class_name+"p+type",
		tools::Text(tools::Including_set("NO")));
}

void Encoder_NO::parameters
::store(const tools::Argument_map_value &vals)
{
	auto p = this->get_prefix();

	if(vals.exist({p+"-info-bits", "K"})) this->K        = vals.to_int({p+"-info-bits", "K"});
	if(vals.exist({p+"-fra",       "F"})) this->n_frames = vals.to_int({p+"-fra",       "F"});
	if(vals.exist({p+"-type"          })) this->type     = vals.at    ({p+"-type"          });

	this->N_cw = this->K;
	this->R = 1.f;
}

void Encoder_NO::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	auto p = this->get_prefix();

	headers[p].push_back(std::make_pair("Type", this->type));
	if (full) headers[p].push_back(std::make_pair("Info. bits (K)", std::to_string(this->K)));
	if (full) headers[p].push_back(std::make_pair("Code rate (R)", std::to_string(this->R)));
	if (full) headers[p].push_back(std::make_pair("Inter frame level", std::to_string(this->n_frames)));
}

template <typename B>
module::Encoder_NO<B>* Encoder_NO::parameters
::build() const
{
	if (this->type == "NO") return new module::Encoder_NO<B>(this->K, this->n_frames);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B>
module::Encoder_NO<B>* Encoder_NO
::build(const parameters &params)
{
	return params.template build<B>();
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::module::Encoder_NO<B_8 >* aff3ct::factory::Encoder_NO::parameters::build<B_8 >() const;
template aff3ct::module::Encoder_NO<B_16>* aff3ct::factory::Encoder_NO::parameters::build<B_16>() const;
template aff3ct::module::Encoder_NO<B_32>* aff3ct::factory::Encoder_NO::parameters::build<B_32>() const;
template aff3ct::module::Encoder_NO<B_64>* aff3ct::factory::Encoder_NO::parameters::build<B_64>() const;
template aff3ct::module::Encoder_NO<B_8 >* aff3ct::factory::Encoder_NO::build<B_8 >(const aff3ct::factory::Encoder_NO::parameters&);
template aff3ct::module::Encoder_NO<B_16>* aff3ct::factory::Encoder_NO::build<B_16>(const aff3ct::factory::Encoder_NO::parameters&);
template aff3ct::module::Encoder_NO<B_32>* aff3ct::factory::Encoder_NO::build<B_32>(const aff3ct::factory::Encoder_NO::parameters&);
template aff3ct::module::Encoder_NO<B_64>* aff3ct::factory::Encoder_NO::build<B_64>(const aff3ct::factory::Encoder_NO::parameters&);
#else
template aff3ct::module::Encoder_NO<B>* aff3ct::factory::Encoder_NO::parameters::build<B>() const;
template aff3ct::module::Encoder_NO<B>* aff3ct::factory::Encoder_NO::build<B>(const aff3ct::factory::Encoder_NO::parameters&);
#endif
// ==================================================================================== explicit template instantiation

