#include <utility>

#include "Tools/Exception/exception.hpp"
#include "Tools/Documentation/documentation.h"
#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator_file.hpp"
#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator_5G.hpp"
#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator_TV.hpp"
#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator_GA.hpp"
#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator_BEC.hpp"
#include "Factory/Tools/Code/Polar/Frozenbits_generator.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Frozenbits_generator_name   = "Frozen bits generator";
const std::string aff3ct::factory::Frozenbits_generator_prefix = "fbg";

Frozenbits_generator::parameters
::parameters(const std::string &prefix)
: Factory::parameters(Frozenbits_generator_name, Frozenbits_generator_name, prefix)
{
	if (!cli::Is_path::check(this->path_fb))
	{
		auto new_path_fb = cli::modify_path<cli::Is_path>(this->path_fb);
		if (!new_path_fb.empty())
			this->path_fb = new_path_fb;
	}
}

Frozenbits_generator::parameters* Frozenbits_generator::parameters
::clone() const
{
	return new Frozenbits_generator::parameters(*this);
}

void Frozenbits_generator::parameters
::get_description(cli::Argument_map_info &args) const
{
	auto p = this->get_prefix();
	const std::string class_name = "factory::Frozenbits_generator::parameters::";

	tools::add_arg(args, p, class_name+"p+info-bits,K",
		cli::Integer(cli::Positive(), cli::Non_zero()),
		cli::arg_rank::REQ);

	tools::add_arg(args, p, class_name+"p+cw-size,N",
		cli::Integer(cli::Positive(), cli::Non_zero()),
		cli::arg_rank::REQ);

	tools::add_arg(args, p, class_name+"p+noise",
		cli::Real(cli::Positive(), cli::Non_zero()));

	tools::add_arg(args, p, class_name+"p+gen-method",
		cli::Text(cli::Including_set("GA", "FILE", "5G", "TV", "BEC")));

	tools::add_arg(args, p, class_name+"p+awgn-path",
		cli::Path(cli::openmode::read));

	tools::add_arg(args, p, class_name+"p+dump-path",
		cli::Folder(cli::openmode::write));

#ifdef AFF3CT_POLAR_BOUNDS
	tools::add_arg(args, p, class_name+"p+pb-path",
		cli::File(cli::openmode::read));
#endif
}

void Frozenbits_generator::parameters
::store(const cli::Argument_map_value &vals)
{
	auto p = this->get_prefix();

	if(vals.exist({p+"-info-bits", "K"})) this->K                  = vals.to_int   ({p+"-info-bits", "K"});
	if(vals.exist({p+"-cw-size",   "N"})) this->N_cw               = vals.to_int   ({p+"-cw-size",   "N"});
	if(vals.exist({p+"-noise"         })) this->noise              = vals.to_float ({p+"-noise"         });
	if(vals.exist({p+"-awgn-path"     })) this->path_fb            = vals.to_path  ({p+"-awgn-path"     });
	if(vals.exist({p+"-gen-method"    })) this->type               = vals.at       ({p+"-gen-method"    });
	if(vals.exist({p+"-dump-path"     })) this->dump_channels_path = vals.to_folder({p+"-dump-path"     });

#ifdef AFF3CT_POLAR_BOUNDS
	if(vals.exist({p+"-pb-path"})) this->path_pb = vals.to_file({p+"-pb-path"});
#endif
}

void Frozenbits_generator::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	auto p = this->get_prefix();

	headers[p].push_back(std::make_pair("Type", this->type));
	if (full) headers[p].push_back(std::make_pair("Info. bits (K)", std::to_string(this->K)));
	if (full) headers[p].push_back(std::make_pair("Codeword size (N)", std::to_string(this->N_cw)));
	headers[p].push_back(std::make_pair("Noise", this->noise == -1.0f ? "adaptive" : std::to_string(this->noise)));
#ifdef AFF3CT_POLAR_BOUNDS
	if (this->type == "TV")
		headers[p].push_back(std::make_pair("PB path", this->path_pb));
#endif
	if (this->type == "TV" || this->type == "FILE")
		headers[p].push_back(std::make_pair("Path", this->path_fb));
	if (!this->dump_channels_path.empty() && (this->type == "GA" || this->type == "BEC"))
		headers[p].push_back(std::make_pair("Dump channels path", this->dump_channels_path));
}

tools::Frozenbits_generator* Frozenbits_generator::parameters
::build() const
{
	if (this->type == "GA"  ) return new tools::Frozenbits_generator_GA  (this->K, this->N_cw, this->dump_channels_path    );
	if (this->type == "TV"  ) return new tools::Frozenbits_generator_TV  (this->K, this->N_cw, this->path_fb, this->path_pb);
	if (this->type == "FILE") return new tools::Frozenbits_generator_file(this->K, this->N_cw, this->path_fb               );
	if (this->type == "5G")   return new tools::Frozenbits_generator_5G  (this->K, this->N_cw                              );
	if (this->type == "BEC")  return new tools::Frozenbits_generator_BEC (this->K, this->N_cw, this->dump_channels_path    );

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

tools::Frozenbits_generator* Frozenbits_generator
::build(const parameters &params)
{
	return params.build();
}
