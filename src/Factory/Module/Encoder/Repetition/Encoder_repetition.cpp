#include <utility>

#include "Tools/Exception/exception.hpp"
#include "Tools/Documentation/documentation.h"
#include "Module/Encoder/Repetition/Encoder_repetition_sys.hpp"
#include "Factory/Module/Encoder/Repetition/Encoder_repetition.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Encoder_repetition_name   = "Encoder Repetiton";
const std::string aff3ct::factory::Encoder_repetition_prefix = "enc";

Encoder_repetition
::Encoder_repetition(const std::string &prefix)
: Encoder(Encoder_repetition_name, prefix)
{
	this->type = "REP";
}

Encoder_repetition* Encoder_repetition
::clone() const
{
	return new Encoder_repetition(*this);
}

void Encoder_repetition
::get_description(cli::Argument_map_info &args) const
{
	Encoder::get_description(args);

	auto p = this->get_prefix();
	const std::string class_name = "factory::Encoder_repetition::";

	cli::add_options(args.at({p+"-type"}), 0, "REP");

	tools::add_arg(args, p, class_name+"p+no-buff",
		cli::None());
}

void Encoder_repetition
::store(const cli::Argument_map_value &vals)
{
	Encoder::store(vals);

	auto p = this->get_prefix();

	if(vals.exist({p+"-no-buff"})) this->buffered = false;
}

void Encoder_repetition
::get_headers(std::map<std::string,tools::header_list>& headers, const bool full) const
{
	Encoder::get_headers(headers, full);

	auto p = this->get_prefix();

	headers[p].push_back(std::make_pair("Buffered", (this->buffered ? "on" : "off")));
}

template <typename B>
module::Encoder_repetition_sys<B>* Encoder_repetition
::build() const
{
	if (this->type == "REP") return new module::Encoder_repetition_sys<B>(this->K, this->N_cw, this->buffered);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::module::Encoder_repetition_sys<B_8 >* aff3ct::factory::Encoder_repetition::build<B_8 >() const;
template aff3ct::module::Encoder_repetition_sys<B_16>* aff3ct::factory::Encoder_repetition::build<B_16>() const;
template aff3ct::module::Encoder_repetition_sys<B_32>* aff3ct::factory::Encoder_repetition::build<B_32>() const;
template aff3ct::module::Encoder_repetition_sys<B_64>* aff3ct::factory::Encoder_repetition::build<B_64>() const;
#else
template aff3ct::module::Encoder_repetition_sys<B>* aff3ct::factory::Encoder_repetition::build<B>() const;
#endif
// ==================================================================================== explicit template instantiation

