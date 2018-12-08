#include "Tools/general_utils.h"
#include "Tools/Exception/exception.hpp"

#include "Module/Puncturer/NO/Puncturer_NO.hpp"
#include "Module/Puncturer/Turbo/Puncturer_turbo.hpp"

#include "Puncturer_turbo.hpp"

#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
using PT = aff3ct::module::Puncturer_turbo<B_32,Q_32>;
#else
using PT = aff3ct::module::Puncturer_turbo<B,Q>;
#endif

using namespace aff3ct;
using namespace aff3ct::factory;


const std::string aff3ct::factory::Puncturer_turbo_name   = "Puncturer Turbo";
const std::string aff3ct::factory::Puncturer_turbo_prefix = "pct";


Puncturer_turbo::parameters
::parameters(const std::string &prefix)
: Puncturer::parameters(Puncturer_turbo_name, prefix)
{
	this->type = "TURBO";
}

Puncturer_turbo::parameters* Puncturer_turbo::parameters
::clone() const
{
	return new Puncturer_turbo::parameters(*this);
}

struct sub_same_length
{
	template <typename T>
	static void check(const std::vector<T>& v)
	{
		for(unsigned i = 0; i < v.size(); i++)
			if (v[i].size() != v[0].size())
				throw std::runtime_error("all elements do not have the same length");
	}
};

void Puncturer_turbo::parameters
::get_description(tools::Argument_map_info &args) const
{
	Puncturer::parameters::get_description(args);

	auto p = this->get_prefix();
	const std::string class_name = "factory::Puncturer_turbo::parameters::";

	args.erase({p+"-fra-size", "N"});

	tools::add_options(args.at({p+"-type"}), 0, "TURBO");

	// args.add(
	// 	{p+"-pattern"},
	// 	tools::List2D<bool>(tools::Boolean(),
	// 	                    std::make_tuple(tools::Length(3, 3), tools::Function<sub_same_length>("elements of same length")),
	// 	                    std::make_tuple(tools::Length(1))),
	// 	"puncturing pattern for the turbo encoder (ex: \"11,10,01\").");
	add_arg(args, p, class_name+"p+pattern",
		tools::List2D<bool>(tools::Boolean(),
	 	                    std::make_tuple(tools::Length(3, 3), tools::Function<sub_same_length>("elements of same length")),
	 	                    std::make_tuple(tools::Length(1))));

	// args.add(
	// 	{p+"-tail-length"},
	// 	tools::Integer(tools::Positive()),
	// 	"total number of tail bits at the end of the frame.");
	add_arg(args, p, class_name+"p+tail-length",
		tools::Integer(tools::Positive()));

	// args.add(
	// 	{p+"-no-buff"},
	// 	tools::None(),
	// 	"does not suppose a buffered encoding.");
	add_arg(args, p, class_name+"p+no-buff",
		tools::None());
}

void Puncturer_turbo::parameters
::store(const tools::Argument_map_value &vals)
{
	Puncturer::parameters::store(vals);

	auto p = this->get_prefix();

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
#ifdef AFF3CT_MULTI_PREC
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
