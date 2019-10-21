#include <stdexcept>
#include <utility>
#include <tuple>

#include "Tools/general_utils.h"
#include "Tools/Exception/exception.hpp"
#include "Tools/Documentation/documentation.h"
#include "Module/Puncturer/NO/Puncturer_NO.hpp"
#include "Module/Puncturer/Turbo/Puncturer_turbo.hpp"
#include "Factory/Module/Puncturer/Turbo/Puncturer_turbo.hpp"
#include "Tools/types.h"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Puncturer_turbo_name   = "Puncturer Turbo";
const std::string aff3ct::factory::Puncturer_turbo_prefix = "pct";

#ifdef AFF3CT_MULTI_PREC
using PT = aff3ct::module::Puncturer_turbo<B_32,Q_32>;
#else
using PT = aff3ct::module::Puncturer_turbo<B,Q>;
#endif

Puncturer_turbo
::Puncturer_turbo(const std::string &prefix)
: Puncturer(Puncturer_turbo_name, prefix)
{
	this->type = "TURBO";
}

Puncturer_turbo* Puncturer_turbo
::clone() const
{
	return new Puncturer_turbo(*this);
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

void Puncturer_turbo
::get_description(cli::Argument_map_info &args) const
{
	Puncturer::get_description(args);

	auto p = this->get_prefix();
	const std::string class_name = "factory::Puncturer_turbo::";

	args.erase({p+"-fra-size", "N"});

	cli::add_options(args.at({p+"-type"}), 0, "TURBO");

	tools::add_arg(args, p, class_name+"p+pattern",
		cli::List2D<bool>(cli::Boolean(),
	 	                  std::make_tuple(cli::Length(3, 3), cli::Function<sub_same_length>("elements of same length")),
	 	                  std::make_tuple(cli::Length(1))));

	tools::add_arg(args, p, class_name+"p+tail-length",
		cli::Integer(cli::Positive()));

	tools::add_arg(args, p, class_name+"p+no-buff",
		cli::None());
}

void Puncturer_turbo
::store(const cli::Argument_map_value &vals)
{
	Puncturer::store(vals);

	auto p = this->get_prefix();

	if(vals.exist({p+"-pattern"    })) this->pattern     = vals.to_list<std::vector<bool>>({p+"-pattern"    });
	if(vals.exist({p+"-tail-length"})) this->tail_length = vals.to_int({p+"-tail-length"});
	if(vals.exist({p+"-no-buff"    })) this->buffered    = false;

	this->N_cw = 3 * this->K + this->tail_length;
	this->N    = PT::compute_N(this->K, this->tail_length, this->pattern);

	if (this->N == this->N_cw)
		this->type = "NO";
}

void Puncturer_turbo
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	Puncturer::get_headers(headers, full);

	auto p = this->get_prefix();

	if (this->type != "NO")
	{
		headers[p].push_back(std::make_pair(std::string("Pattern"), std::string("{" + PT::display_pattern(this->pattern) + "}")));
		if (full) headers[p].push_back(std::make_pair(std::string("Tail length"), std::to_string(this->tail_length)));
		if (full) headers[p].push_back(std::make_pair(std::string("Buffered"), this->buffered ? "on" : "off"));
	}
}

template <typename B, typename Q>
module::Puncturer<B,Q>* Puncturer_turbo
::build() const
{
	if (this->type == "TURBO") return new module::Puncturer_turbo<B,Q>(this->K, this->N, this->tail_length, this->pattern, this->buffered, this->n_frames);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::module::Puncturer<B_8 ,Q_8 >* aff3ct::factory::Puncturer_turbo::build<B_8 ,Q_8 >() const;
template aff3ct::module::Puncturer<B_16,Q_16>* aff3ct::factory::Puncturer_turbo::build<B_16,Q_16>() const;
template aff3ct::module::Puncturer<B_32,Q_32>* aff3ct::factory::Puncturer_turbo::build<B_32,Q_32>() const;
template aff3ct::module::Puncturer<B_64,Q_64>* aff3ct::factory::Puncturer_turbo::build<B_64,Q_64>() const;
#else
template aff3ct::module::Puncturer<B,Q>* aff3ct::factory::Puncturer_turbo::build<B,Q>() const;
#endif
// ==================================================================================== explicit template instantiation
