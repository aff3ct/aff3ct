#include <type_traits>

#include "Tools/Exception/exception.hpp"

#include "Module/Quantizer/Standard/Quantizer_standard.hpp"
#include "Module/Quantizer/Fast/Quantizer_fast.hpp"
#include "Module/Quantizer/Tricky/Quantizer_tricky.hpp"
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

Quantizer::parameters
::~parameters()
{
}

Quantizer::parameters* Quantizer::parameters
::clone() const
{
	return new Quantizer::parameters(*this);
}

void Quantizer::parameters
::get_description(arg_map &req_args, arg_map &opt_args) const
{
	auto p = this->get_prefix();

	req_args[{p+"-size", "N"}] =
		{"strictly_positive_int",
		 "number of real to quantize."};

	opt_args[{p+"-fra", "F"}] =
		{"strictly_positive_int",
		 "set the number of inter frame level to process."};

	opt_args[{p+"-type"}] =
		{"string",
		 "type of the quantizer to use in the simulation.",
		 "STD, STD_FAST, TRICKY"};

	opt_args[{p+"-dec"}] =
		{"positive_int",
		 "the position of the fixed point in the quantified representation."};

	opt_args[{p+"-bits"}] =
		{"strictly_positive_int",
		 "the number of bits used for the quantizer."};

	opt_args[{p+"-range"}] =
		{"strictly_positive_float",
		 "the min/max bound for the tricky quantizer."};
}

void Quantizer::parameters
::store(const arg_val_map &vals)
{
	auto p = this->get_prefix();

	if(exist(vals, {p+"-size", "N"})) this->size       = std::stoi(vals.at({p+"-size", "N"}));
	if(exist(vals, {p+"-fra",  "F"})) this->n_frames   = std::stoi(vals.at({p+"-fra",  "F"}));
	if(exist(vals, {p+"-type"     })) this->type       =           vals.at({p+"-type"     });
	if(exist(vals, {p+"-dec"      })) this->n_decimals = std::stoi(vals.at({p+"-dec"      }));
	if(exist(vals, {p+"-bits"     })) this->n_bits     = std::stoi(vals.at({p+"-bits"     }));
	if(exist(vals, {p+"-range"    })) this->range      = std::stof(vals.at({p+"-range"    }));
}

void Quantizer::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	auto p = this->get_prefix();

	std::string quantif = "unused";
	if (this->type == "TRICKY")
		quantif = "{"+std::to_string(this->n_bits)+", "+std::to_string(this->range)+"f}";
	else if (this->type == "STD" || this->type == "STD_FAST")
		quantif = "{"+std::to_string(this->n_bits)+", "+std::to_string(this->n_decimals)+"}";

	headers[p].push_back(std::make_pair("Type", this->type));
	if (full) headers[p].push_back(std::make_pair("Frame size (N)", std::to_string(this->size)));
	if (full) headers[p].push_back(std::make_pair("Inter frame level", std::to_string(this->n_frames)));
	headers[p].push_back(std::make_pair("Fixed-point config.", quantif));
}

template <typename R, typename Q>
module::Quantizer<R,Q>* Quantizer::parameters
::build() const
{
	     if (this->type == "STD"     ) return new module::Quantizer_standard<R,Q>(this->size, this->n_decimals, this->n_bits, this->n_frames);
	else if (this->type == "STD_FAST") return new module::Quantizer_fast    <R,Q>(this->size, this->n_decimals, this->n_bits, this->n_frames);
	else if (this->type == "TRICKY"  ) return new module::Quantizer_tricky  <R,Q>(this->size, this->range,      this->n_bits, this->n_frames);
	else if (this->type == "NO"      ) return new module::Quantizer_NO      <R,Q>(this->size,                                 this->n_frames);

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
#ifdef MULTI_PREC
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
