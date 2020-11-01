#include <utility>

#include "Tools/Exception/exception.hpp"
#include "Tools/Documentation/documentation.h"
#include "Module/Quantizer/Pow2/Quantizer_pow2.hpp"
#include "Module/Quantizer/Pow2/Quantizer_pow2_fast.hpp"
#include "Module/Quantizer/Custom/Quantizer_custom.hpp"
#include "Module/Quantizer/NO/Quantizer_NO.hpp"
#include "Factory/Module/Quantizer/Quantizer.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Quantizer_name   = "Quantizer";
const std::string aff3ct::factory::Quantizer_prefix = "qnt";

Quantizer
::Quantizer(const std::string &prefix)
: Factory(Quantizer_name, Quantizer_name, prefix)
{
}

Quantizer* Quantizer
::clone() const
{
	return new Quantizer(*this);
}

void Quantizer
::get_description(cli::Argument_map_info &args) const
{
	auto p = this->get_prefix();
	const std::string class_name = "factory::Quantizer::";

	tools::add_arg(args, p, class_name+"p+size,N",
		cli::Integer(cli::Positive(), cli::Non_zero()),
		cli::arg_rank::REQ);

	tools::add_arg(args, p, class_name+"p+type",
		cli::Text(cli::Including_set("POW2", "CUSTOM")));

	tools::add_arg(args, p, class_name+"p+implem",
		cli::Text(cli::Including_set("STD", "FAST")));

	tools::add_arg(args, p, class_name+"p+dec",
		cli::Integer(cli::Positive()));

	tools::add_arg(args, p, class_name+"p+bits",
		cli::Integer(cli::Positive(), cli::Non_zero()));

	tools::add_arg(args, p, class_name+"p+range",
		cli::Real(cli::Positive(), cli::Non_zero()));
}

void Quantizer
::store(const cli::Argument_map_value &vals)
{
	auto p = this->get_prefix();

	if(vals.exist({p+"-range"    })) this->range      = vals.to_float({p+"-range"    });
	if(vals.exist({p+"-size", "N"})) this->size       = vals.to_int  ({p+"-size", "N"});
	if(vals.exist({p+"-dec"      })) this->n_decimals = vals.to_int  ({p+"-dec"      });
	if(vals.exist({p+"-bits"     })) this->n_bits     = vals.to_int  ({p+"-bits"     });
	if(vals.exist({p+"-type"     })) this->type       = vals.at      ({p+"-type"     });
	if(vals.exist({p+"-implem"   })) this->implem     = vals.at      ({p+"-implem"   });
}

void Quantizer
::get_headers(std::map<std::string,tools::header_list>& headers, const bool full) const
{
	auto p = this->get_prefix();

	std::string quantif = "unused";
	if (this->type == "CUSTOM")
		quantif = "{"+std::to_string(this->n_bits)+", "+std::to_string(this->range)+"f}";
	else if (this->type == "POW2")
		quantif = "{"+std::to_string(this->n_bits)+", "+std::to_string(this->n_decimals)+"}";

	headers[p].push_back(std::make_pair("Type", this->type));
	headers[p].push_back(std::make_pair("Implementation", this->implem));
	if (full) headers[p].push_back(std::make_pair("Frame size (N)", std::to_string(this->size)));
	headers[p].push_back(std::make_pair("Fixed-point config.", quantif));
}

template <typename R, typename Q>
module::Quantizer<R,Q>* Quantizer
::build() const
{
	if (this->type == "POW2"   && this->implem == "STD" ) return new module::Quantizer_pow2     <R,Q>(this->size, this->n_decimals, this->n_bits);
	if (this->type == "POW2"   && this->implem == "FAST") return new module::Quantizer_pow2_fast<R,Q>(this->size, this->n_decimals, this->n_bits);
	if (this->type == "CUSTOM" && this->implem == "STD" ) return new module::Quantizer_custom   <R,Q>(this->size, this->range,      this->n_bits);
	if (this->type == "NO"                              ) return new module::Quantizer_NO       <R,Q>(this->size                                );

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::module::Quantizer<R_8 ,Q_8 >* aff3ct::factory::Quantizer::build<R_8 ,Q_8 >() const;
template aff3ct::module::Quantizer<R_16,Q_16>* aff3ct::factory::Quantizer::build<R_16,Q_16>() const;
template aff3ct::module::Quantizer<R_32,Q_32>* aff3ct::factory::Quantizer::build<R_32,Q_32>() const;
template aff3ct::module::Quantizer<R_64,Q_64>* aff3ct::factory::Quantizer::build<R_64,Q_64>() const;
#else
template aff3ct::module::Quantizer<R,Q>* aff3ct::factory::Quantizer::build<R,Q>() const;
#endif
// ==================================================================================== explicit template instantiation
