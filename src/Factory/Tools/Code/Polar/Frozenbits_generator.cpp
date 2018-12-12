#include "Tools/Exception/exception.hpp"
#include "Tools/Documentation/documentation.h"

#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator_file.hpp"
#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator_TV.hpp"
#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator_GA.hpp"

#include "Frozenbits_generator.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Frozenbits_generator_name   = "Frozen bits generator";
const std::string aff3ct::factory::Frozenbits_generator_prefix = "fbg";

Frozenbits_generator::parameters
::parameters(const std::string &prefix)
: Factory::parameters(Frozenbits_generator_name, Frozenbits_generator_name, prefix)
{
}

Frozenbits_generator::parameters* Frozenbits_generator::parameters
::clone() const
{
	return new Frozenbits_generator::parameters(*this);
}

void Frozenbits_generator::parameters
::get_description(tools::Argument_map_info &args) const
{
	auto p = this->get_prefix();
	const std::string class_name = "factory::Frozenbits_generator::parameters::";

	// args.add(
	// 	{p+"-info-bits", "K"},
	// 	tools::Integer(tools::Positive(), tools::Non_zero()),
	// 	"useful number of bit transmitted (information bits).",
	// 	tools::arg_rank::REQ);
	tools::add_arg(args, p, class_name+"p+info-bits,K",
		tools::Integer(tools::Positive(), tools::Non_zero()),
		tools::arg_rank::REQ);

	// args.add(
	// 	{p+"-cw-size", "N"},
	// 	tools::Integer(tools::Positive(), tools::Non_zero()),
	// 	"the codeword size.",
	// 	tools::arg_rank::REQ);
	tools::add_arg(args, p, class_name+"p+cw-size,N",
		tools::Integer(tools::Positive(), tools::Non_zero()),
		tools::arg_rank::REQ);

	// args.add(
	// 	{p+"-sigma"},
	// 	tools::Real(tools::Positive(), tools::Non_zero()),
	// 	"sigma value for the polar codes generation (adaptive frozen bits if "
	//  "sigma is not set).");
	tools::add_arg(args, p, class_name+"p+sigma",
		tools::Real(tools::Positive(), tools::Non_zero()));

	// args.add(
	// 	{p+"-gen-method"},
	// 	tools::Text(tools::Including_set("GA", "FILE", "TV")),
	// 	"select the frozen bits generation method.");
	tools::add_arg(args, p, class_name+"p+gen-method",
		tools::Text(tools::Including_set("GA", "FILE", "TV")));

	// args.add(
	// 	{p+"-awgn-path"},
	// 	tools::Path(tools::openmode::read),
	// 	"path to a file or a directory containing the best channels to use for
	//  "information bits.");
	tools::add_arg(args, p, class_name+"p+awgn-path",
		tools::Path(tools::openmode::read));

#ifdef AFF3CT_POLAR_BOUNDS
	// args.add(
	// 	{p+"-pb-path"},
	// 	tools::File(tools::openmode::read),
	// 	"path of the polar bounds code generator (generates best channels to use");
	tools::add_arg(args, p, class_name+"p+pb-path",
		tools::Path(tools::openmode::read));
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

#ifdef AFF3CT_POLAR_BOUNDS
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
#ifdef AFF3CT_POLAR_BOUNDS
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
	if (this->type == "TV"  ) return new tools::Frozenbits_generator_TV  (this->K, this->N_cw, this->path_fb, this->path_pb, this->sigma);
	if (this->type == "FILE") return new tools::Frozenbits_generator_file(this->K, this->N_cw, this->path_fb                            );

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

tools::Frozenbits_generator* Frozenbits_generator
::build(const parameters &params)
{
	return params.build();
}
