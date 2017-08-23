#include <algorithm>

#include "Tools/Exception/exception.hpp"

#include "Module/Encoder/RSC/Encoder_RSC_generic_sys.hpp"
#include "Module/Encoder/RSC/Encoder_RSC_generic_json_sys.hpp"

#include "Encoder_RSC.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Encoder_RSC::name   = "Encoder RSC";
const std::string aff3ct::factory::Encoder_RSC::prefix = "enc";

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

void Encoder_RSC
::build_args(arg_map &req_args, arg_map &opt_args, const std::string p)
{
	Encoder::build_args(req_args, opt_args, p);
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

void Encoder_RSC
::store_args(const arg_val_map &vals, parameters &params, const std::string p)
{
	params.type = "RSC";

	Encoder::store_args(vals, params, p);

	if(exist(vals, {p+"-no-buff"})) params.buffered = false;

	if(exist(vals, {p+"-std"})) params.standard = vals.at({p+"-std"});

	if (params.standard == "LTE")
		params.poly = {013, 015};

	if (params.standard == "CCSDS")
		params.poly = {023, 033};

	if (exist(vals, {p+"-poly"}))
	{
		auto poly_str = vals.at({p+"-poly"});

#ifdef _MSC_VER
		sscanf_s   (poly_str.c_str(), "{%o,%o}", &params.poly[0], &params.poly[1]);
#else
		std::sscanf(poly_str.c_str(), "{%o,%o}", &params.poly[0], &params.poly[1]);
#endif
	}

	params.tail_length = (int)(2 * std::floor(std::log2((float)std::max(params.poly[0], params.poly[1]))));
	params.N_cw        = 2 * params.K + params.tail_length;
	params.R           = (float)params.K / (float)params.N_cw;
}

void Encoder_RSC
::make_header(params_list& head_enc, const parameters& params, const bool full)
{
	Encoder::make_header(head_enc, params, full);

	if (params.tail_length)
		head_enc.push_back(std::make_pair("Tail length", std::to_string(params.tail_length)));

	head_enc.push_back(std::make_pair("Buffered", (params.buffered ? "on" : "off")));

	if (!params.standard.empty())
		head_enc.push_back(std::make_pair("Standard", params.standard));

	std::stringstream poly;
	poly << "{0" << std::oct << params.poly[0] << ",0" << std::oct << params.poly[1] << "}";
	head_enc.push_back(std::make_pair(std::string("Polynomials"), poly.str()));
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
