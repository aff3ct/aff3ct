#include "Tools/general_utils.h"
#include "Tools/Exception/exception.hpp"

#include "Module/Puncturer/NO/Puncturer_NO.hpp"
#include "Module/Puncturer/Turbo_DB/Puncturer_turbo_DB.hpp"

#include "Puncturer_turbo_DB.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Puncturer_turbo_DB::name   = "Puncturer Turbo DB";
const std::string aff3ct::factory::Puncturer_turbo_DB::prefix = "pct";

Puncturer_turbo_DB::parameters
::parameters(const std::string prefix)
: Puncturer::parameters(Puncturer_turbo_DB::name, prefix)
{
	this->type = "TURBO_DB";
}

Puncturer_turbo_DB::parameters
::~parameters()
{
}

Puncturer_turbo_DB::parameters* Puncturer_turbo_DB::parameters
::clone() const
{
	return new Puncturer_turbo_DB::parameters(*this);
}

void Puncturer_turbo_DB::parameters
::get_description(tools::Argument_map_info &req_args, tools::Argument_map_info &opt_args) const
{
	Puncturer::parameters::get_description(req_args, opt_args);

	auto p = this->get_prefix();

	tools::add_options(opt_args.at({p+"-type"}), 0, "TURBO_DB");
}

void Puncturer_turbo_DB::parameters
::store(const tools::Argument_map_value &vals)
{
	Puncturer::parameters::store(vals);

	this->N_cw = 3 * this->K;

	if (this->N == this->N_cw)
		this->type = "NO";
}

void Puncturer_turbo_DB::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	Puncturer::parameters::get_headers(headers, full);
}

template <typename B, typename Q>
module::Puncturer<B,Q>* Puncturer_turbo_DB::parameters
::build() const
{
	if (this->type == "TURBO_DB") return new module::Puncturer_turbo_DB<B,Q>(this->K, this->N, this->n_frames);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q>
module::Puncturer<B,Q>* Puncturer_turbo_DB
::build(const parameters &params)
{
	return params.template build<B,Q>();
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Puncturer<B_8 ,Q_8 >* aff3ct::factory::Puncturer_turbo_DB::parameters::build<B_8 ,Q_8 >() const;
template aff3ct::module::Puncturer<B_16,Q_16>* aff3ct::factory::Puncturer_turbo_DB::parameters::build<B_16,Q_16>() const;
template aff3ct::module::Puncturer<B_32,Q_32>* aff3ct::factory::Puncturer_turbo_DB::parameters::build<B_32,Q_32>() const;
template aff3ct::module::Puncturer<B_64,Q_64>* aff3ct::factory::Puncturer_turbo_DB::parameters::build<B_64,Q_64>() const;
template aff3ct::module::Puncturer<B_8 ,Q_8 >* aff3ct::factory::Puncturer_turbo_DB::build<B_8 ,Q_8 >(const aff3ct::factory::Puncturer_turbo_DB::parameters&);
template aff3ct::module::Puncturer<B_16,Q_16>* aff3ct::factory::Puncturer_turbo_DB::build<B_16,Q_16>(const aff3ct::factory::Puncturer_turbo_DB::parameters&);
template aff3ct::module::Puncturer<B_32,Q_32>* aff3ct::factory::Puncturer_turbo_DB::build<B_32,Q_32>(const aff3ct::factory::Puncturer_turbo_DB::parameters&);
template aff3ct::module::Puncturer<B_64,Q_64>* aff3ct::factory::Puncturer_turbo_DB::build<B_64,Q_64>(const aff3ct::factory::Puncturer_turbo_DB::parameters&);
#else
template aff3ct::module::Puncturer<B,Q>* aff3ct::factory::Puncturer_turbo_DB::parameters::build<B,Q>() const;
template aff3ct::module::Puncturer<B,Q>* aff3ct::factory::Puncturer_turbo_DB::build<B,Q>(const aff3ct::factory::Puncturer_turbo_DB::parameters&);
#endif
// ==================================================================================== explicit template instantiation
