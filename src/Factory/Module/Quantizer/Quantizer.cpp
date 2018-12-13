#include <type_traits>

#include "Tools/Exception/exception.hpp"
#include "Tools/Documentation/documentation.h"

#include "Module/Quantizer/Pow2/Quantizer_pow2.hpp"
#include "Module/Quantizer/Pow2/Quantizer_pow2_fast.hpp"
#include "Module/Quantizer/Custom/Quantizer_custom.hpp"
#include "Module/Quantizer/NO/Quantizer_NO.hpp"

#include "Quantizer.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Quantizer_name   = "Quantizer";
const std::string aff3ct::factory::Quantizer_prefix = "qnt";

Quantizer::parameters
::parameters(const std::string &prefix)
: Factory::parameters(Quantizer_name, Quantizer_name, prefix)
{
}

Quantizer::parameters* Quantizer::parameters
::clone() const
{
	return new Quantizer::parameters(*this);
}

void Quantizer::parameters
::get_description(tools::Argument_map_info &args) const
{
	auto p = this->get_prefix();
	const std::string class_name = "factory::Quantizer::parameters::";

	// args.add(
	// 	{p+"-size", "N"},
	// 	tools::Integer(tools::Positive(), tools::Non_zero()),
	// 	"number of real to quantize.",
	// 	tools::arg_rank::REQ);
	tools::add_arg(args, p, class_name+"p+size,N",
		tools::Integer(tools::Positive(), tools::Non_zero()),
		tools::arg_rank::REQ);

	// args.add(
	// 	{p+"-fra", "F"},
	// 	tools::Integer(tools::Positive(), tools::Non_zero()),
	// 	"set the number of inter frame level to process.");
	tools::add_arg(args, p, class_name+"p+fra,F",
		tools::Integer(tools::Positive(), tools::Non_zero()));

	// args.add(
	// 	{p+"-type"},
	// 	tools::Text(tools::Including_set("POW2", "CUSTOM")),
	// 	"type of the quantizer to use in the simulation.");
	tools::add_arg(args, p, class_name+"p+type",
		tools::Text(tools::Including_set("POW2", "CUSTOM")));

	// args.add(
	// 	{p+"-implem"},
	// 	tools::Text(tools::Including_set("STD", "FAST")),
	// 	"select the implementation of quantizer.");
	tools::add_arg(args, p, class_name+"p+implem",
		tools::Text(tools::Including_set("STD", "FAST")));

	// args.add(
	// 	{p+"-dec"},
	// 	tools::Integer(tools::Positive()),
	// 	"the position of the fixed point in the quantified representation.");
	tools::add_arg(args, p, class_name+"p+dec",
		tools::Integer(tools::Positive()));

	// args.add(
	// 	{p+"-bits"},
	// 	tools::Integer(tools::Positive(), tools::Non_zero()),
	// 	"the number of bits used for the quantizer.");
	tools::add_arg(args, p, class_name+"p+bits",
		tools::Integer(tools::Positive(), tools::Non_zero()));

	// args.add(
	// 	{p+"-range"},
	// 	tools::Real(tools::Positive(), tools::Non_zero()),
	// 	"the min/max bound for the tricky quantizer.");
	tools::add_arg(args, p, class_name+"p+range",
		tools::Real(tools::Positive(), tools::Non_zero()));
}

void Quantizer::parameters
::store(const tools::Argument_map_value &vals)
{
	auto p = this->get_prefix();

	if(vals.exist({p+"-range"    })) this->range      = vals.to_float({p+"-range"    });
	if(vals.exist({p+"-size", "N"})) this->size       = vals.to_int  ({p+"-size", "N"});
	if(vals.exist({p+"-fra",  "F"})) this->n_frames   = vals.to_int  ({p+"-fra",  "F"});
	if(vals.exist({p+"-dec"      })) this->n_decimals = vals.to_int  ({p+"-dec"      });
	if(vals.exist({p+"-bits"     })) this->n_bits     = vals.to_int  ({p+"-bits"     });
	if(vals.exist({p+"-type"     })) this->type       = vals.at      ({p+"-type"     });
	if(vals.exist({p+"-implem"   })) this->implem     = vals.at      ({p+"-implem"   });
}

void Quantizer::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
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
	if (full) headers[p].push_back(std::make_pair("Inter frame level", std::to_string(this->n_frames)));
	headers[p].push_back(std::make_pair("Fixed-point config.", quantif));
}

template <typename R, typename Q>
module::Quantizer<R,Q>* Quantizer::parameters
::build() const
{
	if (this->type == "POW2"   && this->implem == "STD" ) return new module::Quantizer_pow2     <R,Q>(this->size, this->n_decimals, this->n_bits, this->n_frames);
	if (this->type == "POW2"   && this->implem == "FAST") return new module::Quantizer_pow2_fast<R,Q>(this->size, this->n_decimals, this->n_bits, this->n_frames);
	if (this->type == "CUSTOM" && this->implem == "STD" ) return new module::Quantizer_custom   <R,Q>(this->size, this->range,      this->n_bits, this->n_frames);
	if (this->type == "NO"                              ) return new module::Quantizer_NO       <R,Q>(this->size,                                 this->n_frames);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename R, typename Q>
module::Quantizer<R,Q>* Quantizer
::build(const parameters& params)
{
	return params.template build<R,Q>();
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::module::Quantizer<R_8 ,Q_8 >* aff3ct::factory::Quantizer::parameters::build<R_8 ,Q_8 >() const;
template aff3ct::module::Quantizer<R_16,Q_16>* aff3ct::factory::Quantizer::parameters::build<R_16,Q_16>() const;
template aff3ct::module::Quantizer<R_32,Q_32>* aff3ct::factory::Quantizer::parameters::build<R_32,Q_32>() const;
template aff3ct::module::Quantizer<R_64,Q_64>* aff3ct::factory::Quantizer::parameters::build<R_64,Q_64>() const;
template aff3ct::module::Quantizer<R_8 ,Q_8 >* aff3ct::factory::Quantizer::build<R_8 ,Q_8 >(const aff3ct::factory::Quantizer::parameters&);
template aff3ct::module::Quantizer<R_16,Q_16>* aff3ct::factory::Quantizer::build<R_16,Q_16>(const aff3ct::factory::Quantizer::parameters&);
template aff3ct::module::Quantizer<R_32,Q_32>* aff3ct::factory::Quantizer::build<R_32,Q_32>(const aff3ct::factory::Quantizer::parameters&);
template aff3ct::module::Quantizer<R_64,Q_64>* aff3ct::factory::Quantizer::build<R_64,Q_64>(const aff3ct::factory::Quantizer::parameters&);
#else
template aff3ct::module::Quantizer<R,Q>* aff3ct::factory::Quantizer::parameters::build<R,Q>() const;
template aff3ct::module::Quantizer<R,Q>* aff3ct::factory::Quantizer::build<R,Q>(const aff3ct::factory::Quantizer::parameters&);
#endif
// ==================================================================================== explicit template instantiation
