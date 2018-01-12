#include "Tools/Exception/exception.hpp"

#include "Flip_and_check_DB.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Flip_and_check_DB::name   = "Flip and check DB";
const std::string aff3ct::factory::Flip_and_check_DB::prefix = "fnc";

Flip_and_check_DB::parameters
::parameters(const std::string &prefix)
: Flip_and_check::parameters(Flip_and_check_DB::name, prefix)
{
}

Flip_and_check_DB::parameters
::~parameters()
{
}

Flip_and_check_DB::parameters* Flip_and_check_DB::parameters
::clone() const
{
	return new Flip_and_check_DB::parameters(*this);
}

void Flip_and_check_DB::parameters
::get_description(arg_map &req_args, arg_map &opt_args) const
{
	Flip_and_check::parameters::get_description(req_args, opt_args);
}

void Flip_and_check_DB::parameters
::store(const arg_val_map &vals)
{
	Flip_and_check::parameters::store(vals);
}

void Flip_and_check_DB::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	Flip_and_check::parameters::get_headers(headers, full);
}

template<typename B, typename Q>
tools::Flip_and_check_DB<B,Q>* Flip_and_check_DB::parameters
::build(module::CRC<B> &crc) const
{
	if (!this->enable)
		throw tools::cannot_allocate(__FILE__, __LINE__, __func__);

	return new tools::Flip_and_check_DB<B,Q>(this->size, this->n_ite, crc, this->start_crc_check_ite, this->q, this->ite_min, this->ite_max, this->ite_step, this->n_frames);
}

template<typename B, typename Q>
tools::Flip_and_check_DB<B,Q>* Flip_and_check_DB
::build(const parameters& params, module::CRC<B> &crc)
{
	return params.template build<B,Q>(crc);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::tools::Flip_and_check_DB<B_8 ,Q_8 >* aff3ct::factory::Flip_and_check_DB::parameters::build<B_8 ,Q_8 >(module::CRC<B_8 >&) const;
template aff3ct::tools::Flip_and_check_DB<B_16,Q_16>* aff3ct::factory::Flip_and_check_DB::parameters::build<B_16,Q_16>(module::CRC<B_16>&) const;
template aff3ct::tools::Flip_and_check_DB<B_32,Q_32>* aff3ct::factory::Flip_and_check_DB::parameters::build<B_32,Q_32>(module::CRC<B_32>&) const;
template aff3ct::tools::Flip_and_check_DB<B_64,Q_64>* aff3ct::factory::Flip_and_check_DB::parameters::build<B_64,Q_64>(module::CRC<B_64>&) const;
template aff3ct::tools::Flip_and_check_DB<B_8 ,Q_8 >* aff3ct::factory::Flip_and_check_DB::build<B_8 ,Q_8 >(const aff3ct::factory::Flip_and_check_DB::parameters&, module::CRC<B_8 >&);
template aff3ct::tools::Flip_and_check_DB<B_16,Q_16>* aff3ct::factory::Flip_and_check_DB::build<B_16,Q_16>(const aff3ct::factory::Flip_and_check_DB::parameters&, module::CRC<B_16>&);
template aff3ct::tools::Flip_and_check_DB<B_32,Q_32>* aff3ct::factory::Flip_and_check_DB::build<B_32,Q_32>(const aff3ct::factory::Flip_and_check_DB::parameters&, module::CRC<B_32>&);
template aff3ct::tools::Flip_and_check_DB<B_64,Q_64>* aff3ct::factory::Flip_and_check_DB::build<B_64,Q_64>(const aff3ct::factory::Flip_and_check_DB::parameters&, module::CRC<B_64>&);
#else
template aff3ct::tools::Flip_and_check_DB<B,Q>* aff3ct::factory::Flip_and_check_DB::parameters::build<B,Q>(module::CRC<B>&) const;
template aff3ct::tools::Flip_and_check_DB<B,Q>* aff3ct::factory::Flip_and_check_DB::build<B,Q>(const aff3ct::factory::Flip_and_check_DB::parameters&, module::CRC<B>&);
#endif
// ==================================================================================== explicit template instantiation
