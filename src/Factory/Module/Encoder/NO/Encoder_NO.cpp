#include <utility>

#include "Tools/Exception/exception.hpp"
#include "Tools/Documentation/documentation.h"
#include "Module/Encoder/NO/Encoder_NO.hpp"
#include "Factory/Module/Encoder/NO/Encoder_NO.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Encoder_NO_name   = "Encoder NO";
const std::string aff3ct::factory::Encoder_NO_prefix = "enc";

Encoder_NO
::Encoder_NO(const std::string &prefix)
: Encoder(Encoder_NO_name, prefix)
{
	this->type = "NO";
}

Encoder_NO* Encoder_NO
::clone() const
{
	return new Encoder_NO(*this);
}

void Encoder_NO
::get_description(cli::Argument_map_info &args) const
{
	auto p = this->get_prefix();
	const std::string class_name = "factory::Encoder_NO::";

	tools::add_arg(args, p, class_name+"p+info-bits,K",
		cli::Integer(cli::Positive(), cli::Non_zero()));

	tools::add_arg(args, p, class_name+"p+fra,F",
		cli::Integer(cli::Positive(), cli::Non_zero()));

	tools::add_arg(args, p, class_name+"p+type",
		cli::Text(cli::Including_set("NO")));
}

void Encoder_NO
::store(const cli::Argument_map_value &vals)
{
	auto p = this->get_prefix();

	if(vals.exist({p+"-info-bits", "K"})) this->K        = vals.to_int({p+"-info-bits", "K"});
	if(vals.exist({p+"-fra",       "F"})) this->n_frames = vals.to_int({p+"-fra",       "F"});
	if(vals.exist({p+"-type"          })) this->type     = vals.at    ({p+"-type"          });

	this->N_cw = this->K;
	this->R = 1.f;
}

void Encoder_NO
::get_headers(std::map<std::string,tools::header_list>& headers, const bool full) const
{
	auto p = this->get_prefix();

	headers[p].push_back(std::make_pair("Type", this->type));
	if (full) headers[p].push_back(std::make_pair("Info. bits (K)", std::to_string(this->K)));
	if (full) headers[p].push_back(std::make_pair("Code rate (R)", std::to_string(this->R)));
	if (full) headers[p].push_back(std::make_pair("Inter frame level", std::to_string(this->n_frames)));
}

template <typename B>
module::Encoder_NO<B>* Encoder_NO
::build() const
{
	if (this->type == "NO") return new module::Encoder_NO<B>(this->K, this->n_frames);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::module::Encoder_NO<B_8 >* aff3ct::factory::Encoder_NO::build<B_8 >() const;
template aff3ct::module::Encoder_NO<B_16>* aff3ct::factory::Encoder_NO::build<B_16>() const;
template aff3ct::module::Encoder_NO<B_32>* aff3ct::factory::Encoder_NO::build<B_32>() const;
template aff3ct::module::Encoder_NO<B_64>* aff3ct::factory::Encoder_NO::build<B_64>() const;
#else
template aff3ct::module::Encoder_NO<B>* aff3ct::factory::Encoder_NO::build<B>() const;
#endif
// ==================================================================================== explicit template instantiation

