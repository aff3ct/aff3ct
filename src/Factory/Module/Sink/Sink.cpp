#include <utility>

#include "Tools/Exception/exception.hpp"
#include "Tools/Documentation/documentation.h"
#include "Module/Sink/NO/Sink_NO.hpp"
#include "Module/Sink/User/Sink_user_binary.hpp"
#include "Factory/Module/Sink/Sink.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Sink_name   = "Sink";
const std::string aff3ct::factory::Sink_prefix = "snk";

Sink
::Sink(const std::string &prefix)
: Factory(Sink_name, Sink_name, prefix)
{
}

Sink* Sink
::clone() const
{
	return new Sink(*this);
}

void Sink
::get_description(cli::Argument_map_info &args) const
{
	auto p = this->get_prefix();
	const std::string class_name = "factory::Sink::";

	tools::add_arg(args, p, class_name+"p+info-bits,K",
		cli::Integer(cli::Positive(), cli::Non_zero()),
		cli::arg_rank::REQ);

	tools::add_arg(args, p, class_name+"p+type",
		cli::Text(cli::Including_set("NO", "USER_BIN")));

	tools::add_arg(args, p, class_name+"p+implem",
		cli::Text(cli::Including_set("STD")));

	tools::add_arg(args, p, class_name+"p+path",
		cli::File(cli::openmode::write));
}

void Sink
::store(const cli::Argument_map_value &vals)
{
	auto p = this->get_prefix();

	if(vals.exist({p+"-info-bits", "K"})) this->K      = vals.to_int ({p+"-info-bits", "K"});
	if(vals.exist({p+"-type"          })) this->type   = vals.at     ({p+"-type"          });
	if(vals.exist({p+"-implem"        })) this->implem = vals.at     ({p+"-implem"        });
	if(vals.exist({p+"-path"          })) this->path   = vals.to_file({p+"-path"          });
}

void Sink
::get_headers(std::map<std::string,tools::header_list>& headers, const bool full) const
{
	auto p = this->get_prefix();

	headers[p].push_back(std::make_pair("Type", this->type));
	headers[p].push_back(std::make_pair("Implementation", this->implem));
	if (full) headers[p].push_back(std::make_pair("Info. bits (K_info)", std::to_string(this->K)));
	if (this->type == "USER" || this->type == "USER_BIN")
		headers[p].push_back(std::make_pair("Path", this->path));
}

template <typename B>
module::Sink<B>* Sink
::build() const
{
	if (this->implem == "STD")
	{
		if (this->type == "NO"      ) return new module::Sink_NO         <B>(this->K            );
		if (this->type == "USER_BIN") return new module::Sink_user_binary<B>(this->K, this->path);
	}

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::module::Sink<B_8 >* aff3ct::factory::Sink::build<B_8 >() const;
template aff3ct::module::Sink<B_16>* aff3ct::factory::Sink::build<B_16>() const;
template aff3ct::module::Sink<B_32>* aff3ct::factory::Sink::build<B_32>() const;
template aff3ct::module::Sink<B_64>* aff3ct::factory::Sink::build<B_64>() const;
#else
template aff3ct::module::Sink<B>* aff3ct::factory::Sink::build<B>() const;
#endif
// ==================================================================================== explicit template instantiation
