#include <utility>
#include <sstream>
#include <cstdio>
#include <cmath>
#include <ios>

#include "Tools/Exception/exception.hpp"
#include "Tools/Documentation/documentation.h"
#include "Module/Encoder/RSC/Encoder_RSC_generic_sys.hpp"
#include "Module/Encoder/RSC/Encoder_RSC_generic_json_sys.hpp"
#include "Factory/Module/Encoder/RSC/Encoder_RSC.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Encoder_RSC_name   = "Encoder RSC";
const std::string aff3ct::factory::Encoder_RSC_prefix = "enc";

Encoder_RSC
::Encoder_RSC(const std::string &prefix)
: Encoder(Encoder_RSC_name, prefix)
{
	this->type = "RSC";
}

Encoder_RSC* Encoder_RSC
::clone() const
{
	return new Encoder_RSC(*this);
}

void Encoder_RSC
::get_description(cli::Argument_map_info &args) const
{
	Encoder::get_description(args);

	auto p = this->get_prefix();
	const std::string class_name = "factory::Encoder_RSC::";

	args.erase({p+"-cw-size", "N"});

	cli::add_options(args.at({p+"-type"}), 0, "RSC");

	tools::add_arg(args, p, class_name+"p+no-buff",
		cli::None());

	tools::add_arg(args, p, class_name+"p+poly",
		cli::Text());

	tools::add_arg(args, p, class_name+"p+std",
		cli::Text(cli::Including_set("LTE", "CCSDS")));
}

void Encoder_RSC
::store(const cli::Argument_map_value &vals)
{
	Encoder::store(vals);

	auto p = this->get_prefix();

	if(vals.exist({p+"-no-buff"})) this->buffered = false;
	if(vals.exist({p+"-std"    })) this->standard = vals.at({p+"-std"});

	if (this->standard == "LTE")
		this->poly = {013, 015};

	if (this->standard == "CCSDS")
		this->poly = {023, 033};

	if (vals.exist({p+"-poly"}))
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

void Encoder_RSC
::get_headers(std::map<std::string,tools::header_list>& headers, const bool full) const
{
	Encoder::get_headers(headers, full);

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
module::Encoder_RSC_sys<B>* Encoder_RSC
::build(std::ostream &stream) const
{
	if (this->type == "RSC_JSON") return new module::Encoder_RSC_generic_json_sys<B>(this->K, this->N_cw, this->buffered, this->poly, stream);
	if (this->type == "RSC"     ) return new module::Encoder_RSC_generic_sys     <B>(this->K, this->N_cw, this->buffered, this->poly        );

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::module::Encoder_RSC_sys<B_8 >* aff3ct::factory::Encoder_RSC::build<B_8 >(std::ostream&) const;
template aff3ct::module::Encoder_RSC_sys<B_16>* aff3ct::factory::Encoder_RSC::build<B_16>(std::ostream&) const;
template aff3ct::module::Encoder_RSC_sys<B_32>* aff3ct::factory::Encoder_RSC::build<B_32>(std::ostream&) const;
template aff3ct::module::Encoder_RSC_sys<B_64>* aff3ct::factory::Encoder_RSC::build<B_64>(std::ostream&) const;
#else
template aff3ct::module::Encoder_RSC_sys<B>* aff3ct::factory::Encoder_RSC::build<B>(std::ostream&) const;
#endif
// ==================================================================================== explicit template instantiation
