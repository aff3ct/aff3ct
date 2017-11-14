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
::get_description(tools::Argument_map_info &req_args, tools::Argument_map_info &opt_args) const
{
	auto p = this->get_prefix();

	req_args.add(
		{p+"-info-bits", "K"},
		new tools::Integer<>({new tools::Positive<int>(), new tools::Non_zero<int>()}),
		"useful number of bit transmitted (information bits).");

	req_args.add(
		{p+"-cw-size", "N"},
		new tools::Integer<>({new tools::Positive<int>(), new tools::Non_zero<int>()}),
		"the codeword size.");

	opt_args.add(
		{p+"-sigma"},
		new tools::Real<>({new tools::Positive<float>(), new tools::Non_zero<float>()}),
		"sigma value for the polar codes generation (adaptive frozen bits if sigma is not set).");

	opt_args.add(
		{p+"-gen-method"},
		new tools::Text<>({new tools::Including_set<std::string>({"GA", "FILE", "TV"})}),
		"select the frozen bits generation method.");

	opt_args.add(
		{p+"-awgn-path"},
		new tools::Text<>(),
		"path to a file or a directory containing the best channels to use for information bits.");

#ifdef ENABLE_POLAR_BOUNDS
	opt_args.add(
		{p+"-pb-path"},
		new tools::Text<>(),
		"path of the polar bounds code generator (generates best channels to use).");
#endif
}

void Frozenbits_generator::parameters
::store(const tools::Argument_map_value &vals)
{
	auto p = this->get_prefix();

	if(vals.exist({p+"-info-bits", "K"})) this->K       = vals.to_int  ({p+"-info-bits", "K"});
	if(vals.exist({p+"-cw-size",   "N"})) this->N_cw    = vals.to_int  ({p+"-cw-size",   "N"});
	if(vals.exist({p+"-sigma"         })) this->sigma   = vals.to_float({p+"-sigma"         });
	if(vals.exist({p+"-awgn-path"     })) this->path_fb = vals.at      ({p+"-awgn-path"     });
	if(vals.exist({p+"-gen-method"    })) this->type    = vals.at      ({p+"-gen-method"    });

#ifdef ENABLE_POLAR_BOUNDS
	if(vals.exist({p+"-pb-path"})) this->path_pb = vals.at({p+"-pb-path"});
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
