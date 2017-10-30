#include "Tools/Exception/exception.hpp"
#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator_file.hpp"
#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator_TV.hpp"
#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator_GA.hpp"

#include "Frozenbits_generator.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Frozenbits_generator::name   = "Frozen bits generator";
const std::string aff3ct::factory::Frozenbits_generator::prefix = "fbg";

Frozenbits_generator::parameters
::parameters(const std::string prefix)
: Factory::parameters(Frozenbits_generator::name, Frozenbits_generator::name, prefix)
{
}

Frozenbits_generator::parameters
::~parameters()
{
}

Frozenbits_generator::parameters* Frozenbits_generator::parameters
::clone() const
{
	return new Frozenbits_generator::parameters(*this);
}

void Frozenbits_generator::parameters
::get_description(arg_map &req_args, arg_map &opt_args) const
{
	auto p = this->get_prefix();

	req_args[{p+"-info-bits", "K"}] =
		{"positive_int",
		 "useful number of bit transmitted (information bits)."};

	req_args[{p+"-cw-size", "N"}] =
		{"positive_int",
		 "the codeword size."};

	opt_args[{p+"-sigma"}] =
		{"positive_float",
		 "sigma value for the polar codes generation (adaptive frozen bits if sigma is not set)."};

	opt_args[{p+"-gen-method"}] =
		{"string",
		 "select the frozen bits generation method.",
		 "GA, FILE, TV"};

	opt_args[{p+"-awgn-path"}] =
		{"string",
		 "path to a file or a directory containing the best channels to use for information bits."};

#ifdef ENABLE_POLAR_BOUNDS
	opt_args[{p+"-pb-path"}] =
		{"string",
		 "path of the polar bounds code generator (generates best channels to use)."};
#endif
}

void Frozenbits_generator::parameters
::store(const arg_val_map &vals)
{
	auto p = this->get_prefix();

	if(exist(vals, {p+"-info-bits", "K"})) this->K       = std::stoi(vals.at({p+"-info-bits", "K"}));
	if(exist(vals, {p+"-cw-size",   "N"})) this->N_cw    = std::stoi(vals.at({p+"-cw-size",   "N"}));
	if(exist(vals, {p+"-sigma"         })) this->sigma   = std::stof(vals.at({p+"-sigma"         }));
	if(exist(vals, {p+"-awgn-path"     })) this->path_fb =           vals.at({p+"-awgn-path"     });
	if(exist(vals, {p+"-gen-method"    })) this->type    =           vals.at({p+"-gen-method"    });

#ifdef ENABLE_POLAR_BOUNDS
	if(exist(vals, {p+"-pb-path"})) this->path_pb = vals.at({p+"-pb-path"});
#endif
}

void Frozenbits_generator::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	auto p = this->get_prefix();

	headers[p].push_back(std::make_pair("Type", this->type));
	if (full) headers[p].push_back(std::make_pair("Info. bits (K)", std::to_string(this->K)));
	if (full) headers[p].push_back(std::make_pair("Codeword size (N)", std::to_string(this->N_cw)));
	headers[p].push_back(std::make_pair("Sigma", this->sigma == -1.0f ? "adaptive" : std::to_string(this->sigma)));
#ifdef ENABLE_POLAR_BOUNDS
	if (this->type == "TV")
		headers[p].push_back(std::make_pair("PB path", this->path_pb));
#endif
	if (this->type == "TV" || this->type == "FILE")
		headers[p].push_back(std::make_pair("Path", this->path_fb));
}

tools::Frozenbits_generator* Frozenbits_generator::parameters
::build() const
{
	     if (this->type == "GA"  ) return new tools::Frozenbits_generator_GA  (this->K, this->N_cw,                               this->sigma);
	else if (this->type == "TV"  ) return new tools::Frozenbits_generator_TV  (this->K, this->N_cw, this->path_fb, this->path_pb, this->sigma);
	else if (this->type == "FILE") return new tools::Frozenbits_generator_file(this->K, this->N_cw, this->path_fb                            );

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

tools::Frozenbits_generator* Frozenbits_generator
::build(const parameters &params)
{
	return params.build();
}
