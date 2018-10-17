#include "Tools/Exception/exception.hpp"
#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator_file.hpp"

#include "Frozenbits_generator_MK.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Frozenbits_generator_MK_name   = "Frozen bits generator MK";
const std::string aff3ct::factory::Frozenbits_generator_MK_prefix = "fbg";

Frozenbits_generator_MK::parameters
::parameters(const std::string &prefix)
: Factory::parameters(Frozenbits_generator_MK_name, Frozenbits_generator_MK_name, prefix)
{
}

Frozenbits_generator_MK::parameters* Frozenbits_generator_MK::parameters
::clone() const
{
	return new Frozenbits_generator_MK::parameters(*this);
}

void Frozenbits_generator_MK::parameters
::get_description(tools::Argument_map_info &args) const
{
	auto p = this->get_prefix();

	args.add(
		{p+"-info-bits", "K"},
		tools::Integer(tools::Positive(), tools::Non_zero()),
		"useful number of bit transmitted (information bits).",
		tools::arg_rank::REQ);

	args.add(
		{p+"-cw-size", "N"},
		tools::Integer(tools::Positive(), tools::Non_zero()),
		"the codeword size.",
		tools::arg_rank::REQ);

	args.add(
		{p+"-sigma"},
		tools::Real(tools::Positive(), tools::Non_zero()),
		"sigma value for the polar codes generation (adaptive frozen bits if sigma is not set).");

	args.add(
		{p+"-gen-method"},
		tools::Text(tools::Including_set("FILE")),
		"select the frozen bits generation method.");

	args.add(
		{p+"-awgn-path"},
		tools::Path(tools::openmode::read),
		"path to a file or a directory containing the best channels to use for information bits.");
}

void Frozenbits_generator_MK::parameters
::store(const tools::Argument_map_value &vals)
{
	auto p = this->get_prefix();

	if(vals.exist({p+"-info-bits", "K"})) this->K       = vals.to_int  ({p+"-info-bits", "K"});
	if(vals.exist({p+"-cw-size",   "N"})) this->N_cw    = vals.to_int  ({p+"-cw-size",   "N"});
	if(vals.exist({p+"-sigma"         })) this->sigma   = vals.to_float({p+"-sigma"         });
	if(vals.exist({p+"-awgn-path"     })) this->path_fb = vals.at      ({p+"-awgn-path"     });
	if(vals.exist({p+"-gen-method"    })) this->type    = vals.at      ({p+"-gen-method"    });
}

void Frozenbits_generator_MK::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	auto p = this->get_prefix();

	headers[p].push_back(std::make_pair("Type", this->type));
	if (full) headers[p].push_back(std::make_pair("Info. bits (K)", std::to_string(this->K)));
	if (full) headers[p].push_back(std::make_pair("Codeword size (N)", std::to_string(this->N_cw)));
	headers[p].push_back(std::make_pair("Sigma", this->sigma == -1.0f ? "adaptive" : std::to_string(this->sigma)));
	if (this->type == "FILE")
		headers[p].push_back(std::make_pair("Path", this->path_fb));
}

tools::Frozenbits_generator* Frozenbits_generator_MK::parameters
::build() const
{
	if (this->type == "FILE") return new tools::Frozenbits_generator_file(this->K, this->N_cw, this->path_fb);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

tools::Frozenbits_generator* Frozenbits_generator_MK
::build(const parameters &params)
{
	return params.build();
}
