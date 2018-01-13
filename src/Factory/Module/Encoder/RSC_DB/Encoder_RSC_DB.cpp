#include <algorithm>

#include "Tools/Exception/exception.hpp"

#include "Encoder_RSC_DB.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Encoder_RSC_DB_name   = "Encoder RSC DB";
const std::string aff3ct::factory::Encoder_RSC_DB_prefix = "enc";

Encoder_RSC_DB::parameters
::parameters(const std::string &prefix)
: Encoder::parameters(Encoder_RSC_DB_name, prefix)
{
	this->type = "RSC_DB";
}

Encoder_RSC_DB::parameters
::~parameters()
{
}

Encoder_RSC_DB::parameters* Encoder_RSC_DB::parameters
::clone() const
{
	return new Encoder_RSC_DB::parameters(*this);
}

void Encoder_RSC_DB::parameters
::get_description(arg_map &req_args, arg_map &opt_args) const
{
	Encoder::parameters::get_description(req_args, opt_args);

	auto p = this->get_prefix();

	req_args.erase({p+"-cw-size", "N"});

	opt_args[{p+"-type"}][2] += ", RSC_DB";

	opt_args[{p+"-no-buff"}] =
		{"",
		 "disable the buffered encoding."};

	opt_args[{p+"-std"}] =
		{"string",
		 "select a standard and set automatically some parameters (overwritten with user given arguments).",
		 "DVB-RCS1, DVB-RCS2"};
}

void Encoder_RSC_DB::parameters
::store(const arg_val_map &vals)
{
	Encoder::parameters::store(vals);

	auto p = this->get_prefix();

	if(exist(vals, {p+"-no-buff"})) this->buffered = false;

	if(exist(vals, {p+"-std"})) this->standard = vals.at({p+"-std"});

	this->N_cw = 2 * this->K;
	this->R    = (float)this->K / (float)this->N_cw;
}

void Encoder_RSC_DB::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	Encoder::parameters::get_headers(headers, full);

	auto p = this->get_prefix();

	headers[p].push_back(std::make_pair("Buffered", (this->buffered ? "on" : "off")));

	if (!this->standard.empty())
		headers[p].push_back(std::make_pair("Standard", this->standard));
}

template <typename B>
module::Encoder_RSC_DB<B>* Encoder_RSC_DB::parameters
::build() const
{
	if (this->type == "RSC_DB") return new module::Encoder_RSC_DB<B>(this->K, this->N_cw, this->standard, this->buffered, this->n_frames);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B>
module::Encoder_RSC_DB<B>* Encoder_RSC_DB
::build(const parameters &params)
{
	return params.template build<B>();
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Encoder_RSC_DB<B_8 >* aff3ct::factory::Encoder_RSC_DB::parameters::build<B_8 >() const;
template aff3ct::module::Encoder_RSC_DB<B_16>* aff3ct::factory::Encoder_RSC_DB::parameters::build<B_16>() const;
template aff3ct::module::Encoder_RSC_DB<B_32>* aff3ct::factory::Encoder_RSC_DB::parameters::build<B_32>() const;
template aff3ct::module::Encoder_RSC_DB<B_64>* aff3ct::factory::Encoder_RSC_DB::parameters::build<B_64>() const;
template aff3ct::module::Encoder_RSC_DB<B_8 >* aff3ct::factory::Encoder_RSC_DB::build<B_8 >(const aff3ct::factory::Encoder_RSC_DB::parameters&);
template aff3ct::module::Encoder_RSC_DB<B_16>* aff3ct::factory::Encoder_RSC_DB::build<B_16>(const aff3ct::factory::Encoder_RSC_DB::parameters&);
template aff3ct::module::Encoder_RSC_DB<B_32>* aff3ct::factory::Encoder_RSC_DB::build<B_32>(const aff3ct::factory::Encoder_RSC_DB::parameters&);
template aff3ct::module::Encoder_RSC_DB<B_64>* aff3ct::factory::Encoder_RSC_DB::build<B_64>(const aff3ct::factory::Encoder_RSC_DB::parameters&);
#else
template aff3ct::module::Encoder_RSC_DB<B>* aff3ct::factory::Encoder_RSC_DB::parameters::build<B>() const;
template aff3ct::module::Encoder_RSC_DB<B>* aff3ct::factory::Encoder_RSC_DB::build<B>(const aff3ct::factory::Encoder_RSC_DB::parameters&);
#endif
// ==================================================================================== explicit template instantiation
