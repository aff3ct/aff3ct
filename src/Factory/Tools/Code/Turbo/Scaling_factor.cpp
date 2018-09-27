#include <cctype>

#include "Tools/Exception/exception.hpp"

#include "Tools/Code/Turbo/Post_processing_SISO/Scaling_factor/Scaling_factor_seq.hpp"
#include "Tools/Code/Turbo/Post_processing_SISO/Scaling_factor/Scaling_factor_vec.hpp"
#include "Tools/Code/Turbo/Post_processing_SISO/Scaling_factor/Scaling_factor_array.hpp"
#include "Tools/Code/Turbo/Post_processing_SISO/Scaling_factor/Scaling_factor_constant.hpp"
#include "Tools/Code/Turbo/Post_processing_SISO/Scaling_factor/Scaling_factor_adaptive.hpp"

#include "Scaling_factor.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Scaling_factor_name   = "Scaling factor";
const std::string aff3ct::factory::Scaling_factor_prefix = "sf";

Scaling_factor::parameters
::parameters(const std::string &prefix)
: Factory::parameters(Scaling_factor_name, Scaling_factor_name, prefix)
{
}

Scaling_factor::parameters* Scaling_factor::parameters
::clone() const
{
	return new Scaling_factor::parameters(*this);
}

void Scaling_factor::parameters
::get_description(tools::Argument_map_info &args) const
{
	auto p = this->get_prefix();

	args.add(
		{p+"-type"},
		tools::Text(tools::Including_set("CST", "LTE", "LTE_VEC", "ARRAY", "ADAPTIVE")),
		"scaling factor type.");

	args.add(
		{p+"-ite"},
		tools::Integer(tools::Positive(), tools::Non_zero()),
		"number of iterations.");
}

void Scaling_factor::parameters
::store(const tools::Argument_map_value &vals)
{
	auto p = this->get_prefix();

	if(vals.exist({p+"-type"}))
	{
		this->enable = true;
		this->type = vals.at({p+"-type"});

		if (std::isdigit(this->type[0]))
		{
			this->cst  = std::stof(this->type);
			this->type = "CST";
		}
	}

	if(vals.exist({p+"-ite", "i"})) this->n_ite = vals.to_int({p+"-ite", "i"});
}

void Scaling_factor::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	auto p = this->get_prefix();

	if (this->enable)
	{
		headers[p].push_back(std::make_pair("Enabled", "yes"));
		headers[p].push_back(std::make_pair("Scaling factor (SF)", this->type));
		headers[p].push_back(std::make_pair("SF iterations", std::to_string(this->n_ite)));
		if (this->type == "CST")
			headers[p].push_back(std::make_pair("SF constant", std::to_string(this->cst)));
	}
	else
	{
		headers[p].push_back(std::make_pair("Enabled", "no"));
	}
}

template<typename B, typename Q>
tools::Scaling_factor<B,Q>* Scaling_factor::parameters
::build() const
{
	if (this->type == "CST"     ) return new tools::Scaling_factor_constant<B,Q>(this->n_ite, this->cst        );
	if (this->type == "LTE_VEC" ) return new tools::Scaling_factor_vec     <B,Q>(this->n_ite                   );
	if (this->type == "LTE"     ) return new tools::Scaling_factor_seq     <B,Q>(this->n_ite                   );
	if (this->type == "ARRAY"   ) return new tools::Scaling_factor_array   <B,Q>(this->n_ite, this->alpha_array);
	if (this->type == "ADAPTIVE") return new tools::Scaling_factor_adaptive<B,Q>(this->n_ite                   );

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template<typename B, typename Q>
tools::Scaling_factor<B,Q>* Scaling_factor
::build(const parameters& params)
{
	return params.template build<B,Q>();
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::tools::Scaling_factor<B_8 ,Q_8 >* aff3ct::factory::Scaling_factor::parameters::build<B_8 ,Q_8 >() const;
template aff3ct::tools::Scaling_factor<B_16,Q_16>* aff3ct::factory::Scaling_factor::parameters::build<B_16,Q_16>() const;
template aff3ct::tools::Scaling_factor<B_32,Q_32>* aff3ct::factory::Scaling_factor::parameters::build<B_32,Q_32>() const;
template aff3ct::tools::Scaling_factor<B_64,Q_64>* aff3ct::factory::Scaling_factor::parameters::build<B_64,Q_64>() const;
template aff3ct::tools::Scaling_factor<B_8 ,Q_8 >* aff3ct::factory::Scaling_factor::build<B_8 ,Q_8 >(const aff3ct::factory::Scaling_factor::parameters&);
template aff3ct::tools::Scaling_factor<B_16,Q_16>* aff3ct::factory::Scaling_factor::build<B_16,Q_16>(const aff3ct::factory::Scaling_factor::parameters&);
template aff3ct::tools::Scaling_factor<B_32,Q_32>* aff3ct::factory::Scaling_factor::build<B_32,Q_32>(const aff3ct::factory::Scaling_factor::parameters&);
template aff3ct::tools::Scaling_factor<B_64,Q_64>* aff3ct::factory::Scaling_factor::build<B_64,Q_64>(const aff3ct::factory::Scaling_factor::parameters&);
#else
template aff3ct::tools::Scaling_factor<B,Q>* aff3ct::factory::Scaling_factor::parameters::build<B,Q>() const;
template aff3ct::tools::Scaling_factor<B,Q>* aff3ct::factory::Scaling_factor::build<B,Q>(const aff3ct::factory::Scaling_factor::parameters&);
#endif
// ==================================================================================== explicit template instantiation
