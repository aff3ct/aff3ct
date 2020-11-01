#include "Tools/general_utils.h"
#include "Tools/Exception/exception.hpp"
#include "Module/Puncturer/NO/Puncturer_NO.hpp"
#include "Module/Puncturer/Turbo_DB/Puncturer_turbo_DB.hpp"
#include "Factory/Module/Puncturer/Turbo_DB/Puncturer_turbo_DB.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Puncturer_turbo_DB_name   = "Puncturer Turbo DB";
const std::string aff3ct::factory::Puncturer_turbo_DB_prefix = "pct";

Puncturer_turbo_DB
::Puncturer_turbo_DB(const std::string &prefix)
: Puncturer(Puncturer_turbo_DB_name, prefix)
{
	this->type = "TURBO_DB";
}

Puncturer_turbo_DB* Puncturer_turbo_DB
::clone() const
{
	return new Puncturer_turbo_DB(*this);
}

void Puncturer_turbo_DB
::get_description(cli::Argument_map_info &args) const
{
	Puncturer::get_description(args);

	auto p = this->get_prefix();

	cli::add_options(args.at({p+"-type"}), 0, "TURBO_DB");
}

void Puncturer_turbo_DB
::store(const cli::Argument_map_value &vals)
{
	Puncturer::store(vals);

	this->N_cw = 3 * this->K;

	if (this->N == this->N_cw)
		this->type = "NO";
}

void Puncturer_turbo_DB
::get_headers(std::map<std::string,tools::header_list>& headers, const bool full) const
{
	Puncturer::get_headers(headers, full);
}

template <typename B, typename Q>
module::Puncturer<B,Q>* Puncturer_turbo_DB
::build() const
{
	if (this->type == "TURBO_DB") return new module::Puncturer_turbo_DB<B,Q>(this->K, this->N);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::module::Puncturer<B_8 ,Q_8 >* aff3ct::factory::Puncturer_turbo_DB::build<B_8 ,Q_8 >() const;
template aff3ct::module::Puncturer<B_16,Q_16>* aff3ct::factory::Puncturer_turbo_DB::build<B_16,Q_16>() const;
template aff3ct::module::Puncturer<B_32,Q_32>* aff3ct::factory::Puncturer_turbo_DB::build<B_32,Q_32>() const;
template aff3ct::module::Puncturer<B_64,Q_64>* aff3ct::factory::Puncturer_turbo_DB::build<B_64,Q_64>() const;
#else
template aff3ct::module::Puncturer<B,Q>* aff3ct::factory::Puncturer_turbo_DB::build<B,Q>() const;
#endif
// ==================================================================================== explicit template instantiation
