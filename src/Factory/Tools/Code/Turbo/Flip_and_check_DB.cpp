#include "Tools/Exception/exception.hpp"
#include "Factory/Tools/Code/Turbo/Flip_and_check_DB.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Flip_and_check_DB_name   = "Flip and check DB";
const std::string aff3ct::factory::Flip_and_check_DB_prefix = "fnc";

Flip_and_check_DB
::Flip_and_check_DB(const std::string &prefix)
: Flip_and_check(Flip_and_check_DB_name, prefix)
{
}

Flip_and_check_DB* Flip_and_check_DB
::clone() const
{
	return new Flip_and_check_DB(*this);
}

void Flip_and_check_DB
::get_description(cli::Argument_map_info &args) const
{
	Flip_and_check::get_description(args);
}

void Flip_and_check_DB
::store(const cli::Argument_map_value &vals)
{
	Flip_and_check::store(vals);
}

void Flip_and_check_DB
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	Flip_and_check::get_headers(headers, full);
}

template<typename B, typename Q>
tools::Flip_and_check_DB<B,Q>* Flip_and_check_DB
::build(module::CRC<B> &crc) const
{
	if (!this->enable)
		throw tools::cannot_allocate(__FILE__, __LINE__, __func__);

	return new tools::Flip_and_check_DB<B,Q>(this->size, this->n_ite, crc, this->crc_start_ite, this->q, this->ite_min, this->ite_max, this->ite_step, this->n_frames);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::tools::Flip_and_check_DB<B_8 ,Q_8 >* aff3ct::factory::Flip_and_check_DB::build<B_8 ,Q_8 >(module::CRC<B_8 >&) const;
template aff3ct::tools::Flip_and_check_DB<B_16,Q_16>* aff3ct::factory::Flip_and_check_DB::build<B_16,Q_16>(module::CRC<B_16>&) const;
template aff3ct::tools::Flip_and_check_DB<B_32,Q_32>* aff3ct::factory::Flip_and_check_DB::build<B_32,Q_32>(module::CRC<B_32>&) const;
template aff3ct::tools::Flip_and_check_DB<B_64,Q_64>* aff3ct::factory::Flip_and_check_DB::build<B_64,Q_64>(module::CRC<B_64>&) const;
#else
template aff3ct::tools::Flip_and_check_DB<B,Q>* aff3ct::factory::Flip_and_check_DB::build<B,Q>(module::CRC<B>&) const;
#endif
// ==================================================================================== explicit template instantiation
