#include <utility>
#include <cctype>

#include "Tools/Exception/exception.hpp"
#include "Tools/Documentation/documentation.h"
#include "Tools/Code/Turbo/Post_processing_SISO/Scaling_factor/Scaling_factor_seq.hpp"
#include "Tools/Code/Turbo/Post_processing_SISO/Scaling_factor/Scaling_factor_vec.hpp"
#include "Tools/Code/Turbo/Post_processing_SISO/Scaling_factor/Scaling_factor_array.hpp"
#include "Tools/Code/Turbo/Post_processing_SISO/Scaling_factor/Scaling_factor_constant.hpp"
#include "Tools/Code/Turbo/Post_processing_SISO/Scaling_factor/Scaling_factor_adaptive.hpp"
#include "Factory/Tools/Code/Turbo/Scaling_factor.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Scaling_factor_name   = "Scaling factor";
const std::string aff3ct::factory::Scaling_factor_prefix = "sf";

Scaling_factor
::Scaling_factor(const std::string &prefix)
: Factory(Scaling_factor_name, Scaling_factor_name, prefix)
{
}

Scaling_factor* Scaling_factor
::clone() const
{
	return new Scaling_factor(*this);
}

void Scaling_factor
::get_description(cli::Argument_map_info &args) const
{
	auto p = this->get_prefix();
	const std::string class_name = "factory::Scaling_factor::";

	tools::add_arg(args, p, class_name+"p+type",
		cli::Text(cli::Including_set("CST", "LTE", "LTE_VEC", "ARRAY", "ADAPTIVE")));

	tools::add_arg(args, p, class_name+"p+ite",
		cli::Integer(cli::Positive(), cli::Non_zero()));
}

void Scaling_factor
::store(const cli::Argument_map_value &vals)
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

void Scaling_factor
::get_headers(std::map<std::string,tools::header_list>& headers, const bool full) const
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
tools::Scaling_factor<B,Q>* Scaling_factor
::build() const
{
	if (this->type == "CST"     ) return new tools::Scaling_factor_constant<B,Q>(this->n_ite, this->cst        );
	if (this->type == "LTE_VEC" ) return new tools::Scaling_factor_vec     <B,Q>(this->n_ite                   );
	if (this->type == "LTE"     ) return new tools::Scaling_factor_seq     <B,Q>(this->n_ite                   );
	if (this->type == "ARRAY"   ) return new tools::Scaling_factor_array   <B,Q>(this->n_ite, this->alpha_array);
	if (this->type == "ADAPTIVE") return new tools::Scaling_factor_adaptive<B,Q>(this->n_ite                   );

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::tools::Scaling_factor<B_8 ,Q_8 >* aff3ct::factory::Scaling_factor::build<B_8 ,Q_8 >() const;
template aff3ct::tools::Scaling_factor<B_16,Q_16>* aff3ct::factory::Scaling_factor::build<B_16,Q_16>() const;
template aff3ct::tools::Scaling_factor<B_32,Q_32>* aff3ct::factory::Scaling_factor::build<B_32,Q_32>() const;
template aff3ct::tools::Scaling_factor<B_64,Q_64>* aff3ct::factory::Scaling_factor::build<B_64,Q_64>() const;
#else
template aff3ct::tools::Scaling_factor<B,Q>* aff3ct::factory::Scaling_factor::build<B,Q>() const;
#endif
// ==================================================================================== explicit template instantiation
