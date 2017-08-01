#include "Tools/general_utils.h"
#include "Tools/Exception/exception.hpp"

#include "Module/Puncturer/NO/Puncturer_NO.hpp"
#include "Module/Puncturer/Turbo_DB/Puncturer_turbo_DB.hpp"

#include "Puncturer_turbo_DB.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Puncturer_turbo_DB::name   = "Puncturer Turbo DB";
const std::string aff3ct::factory::Puncturer_turbo_DB::prefix = "pct";

template <typename B, typename Q>
module::Puncturer<B,Q>* Puncturer_turbo_DB
::build(const parameters &params)
{
	if (params.type == "TURBO_DB") return new module::Puncturer_turbo_DB<B,Q>(params.K, params.N, params.n_frames);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

void Puncturer_turbo_DB
::build_args(arg_map &req_args, arg_map &opt_args, const std::string p)
{
	Puncturer::build_args(req_args, opt_args, p);

	opt_args[{p+"-type"}][2] += ", TURBO_DB";
}

void Puncturer_turbo_DB
::store_args(const arg_val_map &vals, parameters &params, const std::string p)
{
	params.type = "TURBO_DB";

	Puncturer::store_args(vals, params, p);

	params.N_cw = 3 * params.K;

	if (params.N == params.N_cw)
		params.type = "NO";
}

void Puncturer_turbo_DB
::make_header(params_list& head_pct, const parameters& params, const bool full)
{
	Puncturer::make_header(head_pct, params, full);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Puncturer<B_8 ,Q_8 >* aff3ct::factory::Puncturer_turbo_DB::build<B_8 ,Q_8 >(const aff3ct::factory::Puncturer_turbo_DB::parameters&);
template aff3ct::module::Puncturer<B_16,Q_16>* aff3ct::factory::Puncturer_turbo_DB::build<B_16,Q_16>(const aff3ct::factory::Puncturer_turbo_DB::parameters&);
template aff3ct::module::Puncturer<B_32,Q_32>* aff3ct::factory::Puncturer_turbo_DB::build<B_32,Q_32>(const aff3ct::factory::Puncturer_turbo_DB::parameters&);
template aff3ct::module::Puncturer<B_64,Q_64>* aff3ct::factory::Puncturer_turbo_DB::build<B_64,Q_64>(const aff3ct::factory::Puncturer_turbo_DB::parameters&);
#else
template aff3ct::module::Puncturer<B,Q>* aff3ct::factory::Puncturer_turbo_DB::build<B,Q>(const aff3ct::factory::Puncturer_turbo_DB::parameters&);
#endif
// ==================================================================================== explicit template instantiation
