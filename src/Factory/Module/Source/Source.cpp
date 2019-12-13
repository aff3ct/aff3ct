#include <utility>

#include "Tools/Exception/exception.hpp"
#include "Tools/Documentation/documentation.h"
#include "Module/Source/AZCW/Source_AZCW.hpp"
#include "Module/Source/Random/Source_random.hpp"
#include "Module/Source/Random/Source_random_fast.hpp"
#include "Module/Source/User/Source_user.hpp"
#include "Module/Source/User/Source_user_binary.hpp"
#include "Factory/Module/Source/Source.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Source_name   = "Source";
const std::string aff3ct::factory::Source_prefix = "src";

Source
::Source(const std::string &prefix)
: Factory(Source_name, Source_name, prefix)
{
}

Source* Source
::clone() const
{
	return new Source(*this);
}

void Source
::get_description(cli::Argument_map_info &args) const
{
	auto p = this->get_prefix();
	const std::string class_name = "factory::Source::";

	tools::add_arg(args, p, class_name+"p+info-bits,K",
		cli::Integer(cli::Positive(), cli::Non_zero()),
		cli::arg_rank::REQ);

	tools::add_arg(args, p, class_name+"p+fra,F",
		cli::Integer(cli::Positive(), cli::Non_zero()));

	tools::add_arg(args, p, class_name+"p+type",
		cli::Text(cli::Including_set("RAND", "AZCW", "USER", "USER_BIN")));

	tools::add_arg(args, p, class_name+"p+implem",
		cli::Text(cli::Including_set("STD", "FAST")));

	tools::add_arg(args, p, class_name+"p+path",
		cli::File(cli::openmode::read));

	tools::add_arg(args, p, class_name+"p+start-idx",
		cli::Integer(cli::Positive()));

	tools::add_arg(args, p, class_name+"p+seed,S",
		cli::Integer(cli::Positive()));
}

void Source
::store(const cli::Argument_map_value &vals)
{
	auto p = this->get_prefix();

	if(vals.exist({p+"-info-bits", "K"})) this->K        = vals.to_int ({p+"-info-bits", "K"});
	if(vals.exist({p+"-fra",       "F"})) this->n_frames = vals.to_int ({p+"-fra",       "F"});
	if(vals.exist({p+"-type"          })) this->type     = vals.at     ({p+"-type"          });
	if(vals.exist({p+"-implem"        })) this->implem   = vals.at     ({p+"-implem"        });
	if(vals.exist({p+"-path"          })) this->path     = vals.to_file({p+"-path"          });
	if(vals.exist({p+"-seed",      "S"})) this->seed     = vals.to_int ({p+"-seed",      "S"});
	if(vals.exist({p+"-start-idx"     })) this->start_idx= vals.to_int ({p+"-start-idx"     });
}

void Source
::get_headers(std::map<std::string,tools::header_list>& headers, const bool full) const
{
	auto p = this->get_prefix();

	headers[p].push_back(std::make_pair("Type", this->type));
	headers[p].push_back(std::make_pair("Implementation", this->implem));
	headers[p].push_back(std::make_pair("Info. bits (K_info)", std::to_string(this->K)));
	if (full) headers[p].push_back(std::make_pair("Inter frame level", std::to_string(this->n_frames)));
	if (this->type == "USER" || this->type == "USER_BIN")
		headers[p].push_back(std::make_pair("Path", this->path));
	if (this->type == "RAND" && full)
		headers[p].push_back(std::make_pair("Seed", std::to_string(this->seed)));
}

template <typename B>
module::Source<B>* Source
::build() const
{
	if (this->type == "RAND")
	{
		if (this->implem == "STD")
			return new module::Source_random     <B>(this->K, this->seed, this->n_frames);
		else if (this->implem == "FAST")
			return new module::Source_random_fast<B>(this->K, this->seed, this->n_frames);
	}

	if (this->type == "AZCW")  return new module::Source_AZCW<B>(this->K,             this->n_frames);
	if (this->type == "USER")  return new module::Source_user<B>(this->K, this->path, this->n_frames, this->start_idx);

	if (this->type == "USER_BIN")
		return new module::Source_user_binary<B>(this->K, this->path, this->n_frames);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::module::Source<B_8 >* aff3ct::factory::Source::build<B_8 >() const;
template aff3ct::module::Source<B_16>* aff3ct::factory::Source::build<B_16>() const;
template aff3ct::module::Source<B_32>* aff3ct::factory::Source::build<B_32>() const;
template aff3ct::module::Source<B_64>* aff3ct::factory::Source::build<B_64>() const;
#else
template aff3ct::module::Source<B>* aff3ct::factory::Source::build<B>() const;
#endif
// ==================================================================================== explicit template instantiation
