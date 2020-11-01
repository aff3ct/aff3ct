#include <utility>

#include "Tools/Exception/exception.hpp"
#include "Tools/Documentation/documentation.h"
#include "Factory/Module/Encoder/RSC_DB/Encoder_RSC_DB.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Encoder_RSC_DB_name   = "Encoder RSC DB";
const std::string aff3ct::factory::Encoder_RSC_DB_prefix = "enc";

Encoder_RSC_DB
::Encoder_RSC_DB(const std::string &prefix)
: Encoder(Encoder_RSC_DB_name, prefix)
{
	this->type = "RSC_DB";
}

Encoder_RSC_DB* Encoder_RSC_DB
::clone() const
{
	return new Encoder_RSC_DB(*this);
}

void Encoder_RSC_DB
::get_description(cli::Argument_map_info &args) const
{
	Encoder::get_description(args);

	auto p = this->get_prefix();
	const std::string class_name = "factory::Encoder_RSC_DB::";

	args.erase({p+"-cw-size", "N"});

	cli::add_options(args.at({p+"-type"}), 0, "RSC_DB");

	tools::add_arg(args, p, class_name+"p+std",
		cli::Text(cli::Including_set("DVB-RCS1", "DVB-RCS2")));

	tools::add_arg(args, p, class_name+"p+no-buff",
		cli::None());
}

void Encoder_RSC_DB
::store(const cli::Argument_map_value &vals)
{
	Encoder::store(vals);

	auto p = this->get_prefix();

	if(vals.exist({p+"-no-buff"})) this->buffered = false;
	if(vals.exist({p+"-std"    })) this->standard = vals.at({p+"-std"});

	this->N_cw = 2 * this->K;
	this->R    = (float)this->K / (float)this->N_cw;
}

void Encoder_RSC_DB
::get_headers(std::map<std::string,tools::header_list>& headers, const bool full) const
{
	Encoder::get_headers(headers, full);

	auto p = this->get_prefix();

	headers[p].push_back(std::make_pair("Buffered", (this->buffered ? "on" : "off")));

	if (!this->standard.empty())
		headers[p].push_back(std::make_pair("Standard", this->standard));
}

template <typename B>
module::Encoder_RSC_DB<B>* Encoder_RSC_DB
::build() const
{
	if (this->type == "RSC_DB") return new module::Encoder_RSC_DB<B>(this->K, this->N_cw, this->standard, this->buffered);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::module::Encoder_RSC_DB<B_8 >* aff3ct::factory::Encoder_RSC_DB::build<B_8 >() const;
template aff3ct::module::Encoder_RSC_DB<B_16>* aff3ct::factory::Encoder_RSC_DB::build<B_16>() const;
template aff3ct::module::Encoder_RSC_DB<B_32>* aff3ct::factory::Encoder_RSC_DB::build<B_32>() const;
template aff3ct::module::Encoder_RSC_DB<B_64>* aff3ct::factory::Encoder_RSC_DB::build<B_64>() const;
#else
template aff3ct::module::Encoder_RSC_DB<B>* aff3ct::factory::Encoder_RSC_DB::build<B>() const;
#endif
// ==================================================================================== explicit template instantiation
