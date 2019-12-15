#include <utility>

#include "Tools/Exception/exception.hpp"
#include "Tools/Documentation/documentation.h"
#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator_file.hpp"
#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator_GA.hpp"
#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator_GA_Arikan.hpp"
#include "Factory/Tools/Code/Polar_MK/Frozenbits_generator_MK.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Frozenbits_generator_MK_name   = "Frozen bits generator MK";
const std::string aff3ct::factory::Frozenbits_generator_MK_prefix = "fbg";

Frozenbits_generator_MK
::Frozenbits_generator_MK(const std::string &prefix)
: Factory(Frozenbits_generator_MK_name, Frozenbits_generator_MK_name, prefix)
{
}

Frozenbits_generator_MK* Frozenbits_generator_MK
::clone() const
{
	return new Frozenbits_generator_MK(*this);
}

void Frozenbits_generator_MK
::get_description(cli::Argument_map_info &args) const
{
	auto p = this->get_prefix();
	const std::string class_name = "factory::Frozenbits_generator_MK::";

	tools::add_arg(args, p, class_name+"p+info-bits,K",
		cli::Integer(cli::Positive(), cli::Non_zero()),
		cli::arg_rank::REQ);

	tools::add_arg(args, p, class_name+"p+cw-size,N",
		cli::Integer(cli::Positive(), cli::Non_zero()),
		cli::arg_rank::REQ);

	tools::add_arg(args, p, class_name+"p+noise",
		cli::Real(cli::Positive(), cli::Non_zero()));

	tools::add_arg(args, p, class_name+"p+gen-method",
		cli::Text(cli::Including_set("FILE", "GA", "GAA")));

	tools::add_arg(args, p, class_name+"p+awgn-path",
		cli::File(cli::openmode::read));

	tools::add_arg(args, p, class_name+"p+dump-path",
		cli::Folder(cli::openmode::write));
}

void Frozenbits_generator_MK
::store(const cli::Argument_map_value &vals)
{
	auto p = this->get_prefix();

	if(vals.exist({p+"-info-bits", "K"})) this->K                  = vals.to_int   ({p+"-info-bits", "K"});
	if(vals.exist({p+"-cw-size",   "N"})) this->N_cw               = vals.to_int   ({p+"-cw-size",   "N"});
	if(vals.exist({p+"-noise"         })) this->noise              = vals.to_float ({p+"-noise"         });
	if(vals.exist({p+"-awgn-path"     })) this->path_fb            = vals.to_file  ({p+"-awgn-path"     });
	if(vals.exist({p+"-gen-method"    })) this->type               = vals.at       ({p+"-gen-method"    });
	if(vals.exist({p+"-dump-path"     })) this->dump_channels_path = vals.to_folder({p+"-dump-path"     });
}

void Frozenbits_generator_MK
::get_headers(std::map<std::string,tools::header_list>& headers, const bool full) const
{
	auto p = this->get_prefix();

	headers[p].push_back(std::make_pair("Type", this->type));
	if (full) headers[p].push_back(std::make_pair("Info. bits (K)", std::to_string(this->K)));
	if (full) headers[p].push_back(std::make_pair("Codeword size (N)", std::to_string(this->N_cw)));
	headers[p].push_back(std::make_pair("Noise", this->noise == -1.0f ? "adaptive" : std::to_string(this->noise)));
	if (this->type == "FILE")
		headers[p].push_back(std::make_pair("Path", this->path_fb));
	if (!this->dump_channels_path.empty() && (this->type == "GA" || this->type == "GAA" || this->type == "BEC"))
		headers[p].push_back(std::make_pair("Dump channels path", this->dump_channels_path));
}

tools::Frozenbits_generator* Frozenbits_generator_MK
::build(const tools::Polar_code &pc) const
{
	if (this->type == "GAA" && pc.is_mono_kernel() == 2) return new tools::Frozenbits_generator_GA_Arikan(this->K, this->N_cw,     this->dump_channels_path);
	if (this->type == "GA"                             ) return new tools::Frozenbits_generator_GA       (this->K, this->N_cw, pc, this->dump_channels_path);
	if (this->type == "FILE"                           ) return new tools::Frozenbits_generator_file     (this->K, this->N_cw,     this->path_fb           );

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}
