#include <utility>

#include "Tools/Exception/exception.hpp"
#include "Tools/Documentation/documentation.h"
#include "Module/Encoder/AZCW/Encoder_AZCW.hpp"
#include "Module/Encoder/Coset/Encoder_coset.hpp"
#include "Module/Encoder/User/Encoder_user.hpp"
#include "Factory/Module/Encoder/Encoder.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Encoder_name   = "Encoder";
const std::string aff3ct::factory::Encoder_prefix = "enc";

Encoder
::Encoder(const std::string &prefix)
: Factory(Encoder_name, Encoder_name, prefix)
{
}

Encoder
::Encoder(const std::string &name, const std::string &prefix)
: Factory(name, Encoder_name, prefix)
{
}

Encoder* Encoder
::clone() const
{
	return new Encoder(*this);
}

void Encoder
::get_description(cli::Argument_map_info &args) const
{
	auto p = this->get_prefix();
	const std::string class_name = "factory::Encoder::";

	tools::add_arg(args, p, class_name+"p+info-bits,K",
		cli::Integer(cli::Positive(), cli::Non_zero()),
		cli::arg_rank::REQ);

	tools::add_arg(args, p, class_name+"p+cw-size,N",
		cli::Integer(cli::Positive(), cli::Non_zero()),
		cli::arg_rank::REQ);

	tools::add_arg(args, p, class_name+"p+type",
		cli::Text(cli::Including_set("AZCW", "COSET", "USER")));

	tools::add_arg(args, p, class_name+"p+path",
		cli::File(cli::openmode::read));

	tools::add_arg(args, p, class_name+"p+start-idx",
		cli::Integer(cli::Positive()));

	tools::add_arg(args, p, class_name+"p+seed,S",
		cli::Integer(cli::Positive()));
}

void Encoder
::store(const cli::Argument_map_value &vals)
{
	auto p = this->get_prefix();

	if(vals.exist({p+"-info-bits", "K"})) this->K          = vals.to_int ({p+"-info-bits", "K"});
	if(vals.exist({p+"-cw-size",   "N"})) this->N_cw       = vals.to_int ({p+"-cw-size",   "N"});
	if(vals.exist({p+"-seed",      "S"})) this->seed       = vals.to_int ({p+"-seed",      "S"});
	if(vals.exist({p+"-type"          })) this->type       = vals.at     ({p+"-type"          });
	if(vals.exist({p+"-path"          })) this->path       = vals.to_file({p+"-path"          });
	if(vals.exist({p+"-no-sys"        })) this->systematic = false;
	if(vals.exist({p+"-start-idx"     })) this->start_idx  = vals.to_int ({p+"-start-idx"     });

	this->R = (float)this->K / (float)this->N_cw;
}

void Encoder
::get_headers(std::map<std::string,tools::header_list>& headers, const bool full) const
{
	auto p = this->get_prefix();

	headers[p].push_back(std::make_pair("Type", this->type));
	if (full) headers[p].push_back(std::make_pair("Info. bits (K)", std::to_string(this->K)));
	if (full) headers[p].push_back(std::make_pair("Codeword size (N)", std::to_string(this->N_cw)));
	if (full) headers[p].push_back(std::make_pair("Code rate (R)", std::to_string(this->R)));
	headers[p].push_back(std::make_pair("Systematic", ((this->systematic) ? "yes" : "no")));
	if (this->type == "USER")
		headers[p].push_back(std::make_pair("Path", this->path));
	if (this->type == "COSET" && full)
		headers[p].push_back(std::make_pair("Seed", std::to_string(this->seed)));
}

template <typename B>
module::Encoder<B>* Encoder
::build() const
{
	if (this->type == "AZCW" ) return new module::Encoder_AZCW <B>(this->K, this->N_cw);
	if (this->type == "COSET") return new module::Encoder_coset<B>(this->K, this->N_cw, this->seed);
	if (this->type == "USER" ) return new module::Encoder_user <B>(this->K, this->N_cw, this->path, this->start_idx);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::module::Encoder<B_8 >* aff3ct::factory::Encoder::build<B_8 >() const;
template aff3ct::module::Encoder<B_16>* aff3ct::factory::Encoder::build<B_16>() const;
template aff3ct::module::Encoder<B_32>* aff3ct::factory::Encoder::build<B_32>() const;
template aff3ct::module::Encoder<B_64>* aff3ct::factory::Encoder::build<B_64>() const;
#else
template aff3ct::module::Encoder<B>* aff3ct::factory::Encoder::build<B>() const;
#endif
// ==================================================================================== explicit template instantiation
