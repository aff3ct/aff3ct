#include <algorithm>
#include <cmath>

#include "Tools/Exception/exception.hpp"

#include "Module/Encoder/RSC/Encoder_RSC_generic_sys.hpp"
#include "Module/Encoder/RSC/Encoder_RSC_generic_json_sys.hpp"

#include "Encoder_RSC.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Encoder_RSC_name   = "Encoder RSC";
const std::string aff3ct::factory::Encoder_RSC_prefix = "enc";

Encoder_RSC::parameters
::parameters(const std::string &prefix)
: Encoder::parameters(Encoder_RSC_name, prefix)
{
	this->type = "RSC";
}

Encoder_RSC::parameters
::~parameters()
{
}

Encoder_RSC::parameters* Encoder_RSC::parameters
::clone() const
{
	return new Encoder_RSC::parameters(*this);
}

void Encoder_RSC::parameters
::get_description(arg_map &req_args, arg_map &opt_args) const
{
	Encoder::parameters::get_description(req_args, opt_args);

	auto p = this->get_prefix();

	req_args.erase({p+"-cw-size", "N"});

	opt_args[{p+"-type"}][2] += ", RSC";

	opt_args[{p+"-no-buff"}] =
		{"",
		 "disable the buffered encoding."};

	opt_args[{p+"-poly"}] =
		{"string",
		 "the polynomials describing RSC code, should be of the form \"{A,B}\"."};

	opt_args[{p+"-std"}] =
		{"string",
		 "select a standard and set automatically some parameters (overwritten with user given arguments).",
		 "LTE, CCSDS"};
}

void Encoder_RSC::parameters
::store(const arg_val_map &vals)
{
	Encoder::parameters::store(vals);

	auto p = this->get_prefix();

	if(exist(vals, {p+"-no-buff"})) this->buffered = false;

	if(exist(vals, {p+"-std"})) this->standard = vals.at({p+"-std"});

	if (this->standard == "LTE")
		this->poly = {013, 015};

	if (this->standard == "CCSDS")
		this->poly = {023, 033};

	if (exist(vals, {p+"-poly"}))
	{
		this->standard = "";
		auto poly_str = vals.at({p+"-poly"});

#ifdef _MSC_VER
		sscanf_s   (poly_str.c_str(), "{%o,%o}", &this->poly[0], &this->poly[1]);
#else
		std::sscanf(poly_str.c_str(), "{%o,%o}", &this->poly[0], &this->poly[1]);
#endif
	}

	if (this->poly[0] == 013 && this->poly[1] == 015)
		this->standard = "LTE";

	if (this->poly[0] == 023 && this->poly[1] == 033)
		this->standard = "CCSDS";

	this->tail_length = (int)(2 * std::floor(std::log2((float)std::max(this->poly[0], this->poly[1]))));
	this->N_cw        = 2 * this->K + this->tail_length;
	this->R           = (float)this->K / (float)this->N_cw;
}

void Encoder_RSC::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	Encoder::parameters::get_headers(headers, full);

	auto p = this->get_prefix();

	if (this->tail_length)
		headers[p].push_back(std::make_pair("Tail length", std::to_string(this->tail_length)));

	headers[p].push_back(std::make_pair("Buffered", (this->buffered ? "on" : "off")));

	if (!this->standard.empty())
		headers[p].push_back(std::make_pair("Standard", this->standard));

	std::stringstream poly;
	poly << "{0" << std::oct << this->poly[0] << ",0" << std::oct << this->poly[1] << "}";
	headers[p].push_back(std::make_pair(std::string("Polynomials"), poly.str()));
}

template <typename B>
module::Encoder_RSC_sys<B>* Encoder_RSC::parameters
::build(std::ostream &stream) const
{
	     if (this->type == "RSC_JSON") return new module::Encoder_RSC_generic_json_sys<B>(this->K, this->N_cw, this->buffered, this->poly, stream, this->n_frames);
	else if (this->type == "RSC"     ) return new module::Encoder_RSC_generic_sys     <B>(this->K, this->N_cw, this->buffered, this->poly,         this->n_frames);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B>
module::Encoder_RSC_sys<B>* Encoder_RSC
::build(const parameters &params, std::ostream &stream)
{
	return params.template build<B>(stream);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Encoder_RSC_sys<B_8 >* aff3ct::factory::Encoder_RSC::parameters::build<B_8 >(std::ostream&) const;
template aff3ct::module::Encoder_RSC_sys<B_16>* aff3ct::factory::Encoder_RSC::parameters::build<B_16>(std::ostream&) const;
template aff3ct::module::Encoder_RSC_sys<B_32>* aff3ct::factory::Encoder_RSC::parameters::build<B_32>(std::ostream&) const;
template aff3ct::module::Encoder_RSC_sys<B_64>* aff3ct::factory::Encoder_RSC::parameters::build<B_64>(std::ostream&) const;
template aff3ct::module::Encoder_RSC_sys<B_8 >* aff3ct::factory::Encoder_RSC::build<B_8 >(const aff3ct::factory::Encoder_RSC::parameters&, std::ostream&);
template aff3ct::module::Encoder_RSC_sys<B_16>* aff3ct::factory::Encoder_RSC::build<B_16>(const aff3ct::factory::Encoder_RSC::parameters&, std::ostream&);
template aff3ct::module::Encoder_RSC_sys<B_32>* aff3ct::factory::Encoder_RSC::build<B_32>(const aff3ct::factory::Encoder_RSC::parameters&, std::ostream&);
template aff3ct::module::Encoder_RSC_sys<B_64>* aff3ct::factory::Encoder_RSC::build<B_64>(const aff3ct::factory::Encoder_RSC::parameters&, std::ostream&);
#else
template aff3ct::module::Encoder_RSC_sys<B>* aff3ct::factory::Encoder_RSC::parameters::build<B>(std::ostream&) const;
template aff3ct::module::Encoder_RSC_sys<B>* aff3ct::factory::Encoder_RSC::build<B>(const aff3ct::factory::Encoder_RSC::parameters&, std::ostream&);
#endif
// ==================================================================================== explicit template instantiation
