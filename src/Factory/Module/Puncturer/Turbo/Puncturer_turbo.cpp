#include "Tools/general_utils.h"
#include "Tools/Exception/exception.hpp"

#include "Module/Puncturer/NO/Puncturer_NO.hpp"
#include "Module/Puncturer/Turbo/Puncturer_turbo.hpp"

#include "Puncturer_turbo.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

using PT = aff3ct::module::Puncturer_turbo<>;

const std::string aff3ct::factory::Puncturer_turbo::name   = "Puncturer Turbo";
const std::string aff3ct::factory::Puncturer_turbo::prefix = "pct";


Puncturer_turbo::parameters
::parameters(const std::string prefix)
: Puncturer::parameters(Puncturer_turbo::name, prefix)
{
	this->type = "TURBO";
}

Puncturer_turbo::parameters
::~parameters()
{
}

Puncturer_turbo::parameters* Puncturer_turbo::parameters
::clone() const
{
	return new Puncturer_turbo::parameters(*this);
}

void Puncturer_turbo::parameters
::get_description(tools::Argument_map_info &req_args, tools::Argument_map_info &opt_args) const
{
	Puncturer::parameters::get_description(req_args, opt_args);

	auto p = this->get_prefix();

	req_args.erase({p+"-fra-size", "N"});

	tools::add_options<std::string>(opt_args.at({p+"-type"}), {"TURBO"}, 0);

	opt_args.add(
		{p+"-pattern"},
		new tools::List2D<bool>(new tools::Boolean<>(), {new tools::Length<std::vector<std::vector<bool>>>(3, 3)}, {new tools::Length<std::vector<bool>>(2, 3)}),
		"puncturing pattern for the turbo encoder (ex: \"11,10,01\").");

	opt_args.add(
		{p+"-tail-length"},
		new tools::Integer<>({new tools::Positive<int>()}),
		"total number of tail bits at the end of the frame.");

	opt_args.add(
		{p+"-no-buff"},
		new tools::None(),
		"does not suppose a buffered encoding.");
}

void Puncturer_turbo::parameters
::store(const tools::Argument_map_value &vals)
{
	Puncturer::parameters::store(vals);

	auto p = this->get_prefix();

	// if(vals.exist({p+"-pattern"    })) this->pattern2    = vals.at     ({p+"-pattern"    });
	if(vals.exist({p+"-pattern"    })) this->pattern     = vals.to_list<std::vector<bool>>({p+"-pattern"    });
	if(vals.exist({p+"-tail-length"})) this->tail_length = vals.to_int({p+"-tail-length"});
	if(vals.exist({p+"-no-buff"    })) this->buffered    = false;

	this->N_cw = 3 * this->K + this->tail_length;
	this->N    = PT::compute_N(this->K, this->tail_length, this->pattern);

	if (this->N == this->N_cw)
		this->type = "NO";
}

void Puncturer_turbo::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	Puncturer::parameters::get_headers(headers, full);

	auto p = this->get_prefix();

	if (this->type != "NO")
	{
		headers[p].push_back(std::make_pair(std::string("Pattern"), std::string("{" + PT::display_pattern(this->pattern) + "}")));
		// headers[p].push_back(std::make_pair(std::string("Pattern cmd"), std::string("{" + this->pattern2 + "}")));
		if (full) headers[p].push_back(std::make_pair(std::string("Tail length"), std::to_string(this->tail_length)));
		if (full) headers[p].push_back(std::make_pair(std::string("Buffered"), this->buffered ? "on" : "off"));
	}
}

template <typename B, typename Q>
module::Puncturer<B,Q>* Puncturer_turbo::parameters
::build() const
{
	if (this->type == "TURBO") return new module::Puncturer_turbo<B,Q>(this->K, this->N, this->tail_length, this->pattern, this->buffered, this->n_frames);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q>
module::Puncturer<B,Q>* Puncturer_turbo
::build(const parameters &params)
{
	return params.template build<B,Q>();
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Puncturer<B_8 ,Q_8 >* aff3ct::factory::Puncturer_turbo::parameters::build<B_8 ,Q_8 >() const;
template aff3ct::module::Puncturer<B_16,Q_16>* aff3ct::factory::Puncturer_turbo::parameters::build<B_16,Q_16>() const;
template aff3ct::module::Puncturer<B_32,Q_32>* aff3ct::factory::Puncturer_turbo::parameters::build<B_32,Q_32>() const;
template aff3ct::module::Puncturer<B_64,Q_64>* aff3ct::factory::Puncturer_turbo::parameters::build<B_64,Q_64>() const;
template aff3ct::module::Puncturer<B_8 ,Q_8 >* aff3ct::factory::Puncturer_turbo::build<B_8 ,Q_8 >(const aff3ct::factory::Puncturer_turbo::parameters&);
template aff3ct::module::Puncturer<B_16,Q_16>* aff3ct::factory::Puncturer_turbo::build<B_16,Q_16>(const aff3ct::factory::Puncturer_turbo::parameters&);
template aff3ct::module::Puncturer<B_32,Q_32>* aff3ct::factory::Puncturer_turbo::build<B_32,Q_32>(const aff3ct::factory::Puncturer_turbo::parameters&);
template aff3ct::module::Puncturer<B_64,Q_64>* aff3ct::factory::Puncturer_turbo::build<B_64,Q_64>(const aff3ct::factory::Puncturer_turbo::parameters&);
#else
template aff3ct::module::Puncturer<B,Q>* aff3ct::factory::Puncturer_turbo::parameters::build<B,Q>() const;
template aff3ct::module::Puncturer<B,Q>* aff3ct::factory::Puncturer_turbo::build<B,Q>(const aff3ct::factory::Puncturer_turbo::parameters&);
#endif
// ==================================================================================== explicit template instantiation
