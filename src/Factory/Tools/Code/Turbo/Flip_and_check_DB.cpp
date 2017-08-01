#include "Tools/Exception/exception.hpp"

#include "Flip_and_check_DB.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Flip_and_check_DB::name   = "Flip and check DB";
const std::string aff3ct::factory::Flip_and_check_DB::prefix = "fnc";

template<typename B, typename Q>
tools::Flip_and_check_DB<B,Q>* Flip_and_check_DB
::build(const parameters& params, module::CRC<B> &crc)
{
	if (!params.enable)
		throw tools::cannot_allocate(__FILE__, __LINE__, __func__);

	return new tools::Flip_and_check_DB<B,Q>(params.size, params.n_ite, crc, params.start_crc_check_ite, params.q, params.ite_min, params.ite_max, params.ite_step, params.n_frames);
}

void Flip_and_check_DB
::build_args(arg_map &req_args, arg_map &opt_args, const std::string p)
{
	Flip_and_check::build_args(req_args, opt_args, p);
}

void Flip_and_check_DB
::store_args(const arg_val_map &vals, parameters &params, const std::string p)
{
	Flip_and_check::store_args(vals, params, p);
}

void Flip_and_check_DB
::make_header(params_list& head_fnc, const parameters& params, const bool full)
{
	Flip_and_check::make_header(head_fnc, params, full);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::tools::Flip_and_check_DB<B_8 ,Q_8 >* aff3ct::factory::Flip_and_check_DB::build<B_8 ,Q_8 >(const aff3ct::factory::Flip_and_check_DB::parameters&, module::CRC<B_8 >&);
template aff3ct::tools::Flip_and_check_DB<B_16,Q_16>* aff3ct::factory::Flip_and_check_DB::build<B_16,Q_16>(const aff3ct::factory::Flip_and_check_DB::parameters&, module::CRC<B_16>&);
template aff3ct::tools::Flip_and_check_DB<B_32,Q_32>* aff3ct::factory::Flip_and_check_DB::build<B_32,Q_32>(const aff3ct::factory::Flip_and_check_DB::parameters&, module::CRC<B_32>&);
template aff3ct::tools::Flip_and_check_DB<B_64,Q_64>* aff3ct::factory::Flip_and_check_DB::build<B_64,Q_64>(const aff3ct::factory::Flip_and_check_DB::parameters&, module::CRC<B_64>&);
#else
template aff3ct::tools::Flip_and_check_DB<B,Q>* aff3ct::factory::Flip_and_check_DB::build<B,Q>(const aff3ct::factory::Flip_and_check_DB::parameters&, module::CRC<B>&);
#endif
// ==================================================================================== explicit template instantiation
