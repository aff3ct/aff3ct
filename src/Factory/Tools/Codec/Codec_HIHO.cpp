#include "Factory/Tools/Codec/BCH/Codec_BCH.hpp"
#include "Factory/Tools/Codec/LDPC/Codec_LDPC.hpp"
#include "Factory/Tools/Codec/Polar/Codec_polar.hpp"
#include "Factory/Tools/Codec/RA/Codec_RA.hpp"
#include "Factory/Tools/Codec/Repetition/Codec_repetition.hpp"
#include "Factory/Tools/Codec/RS/Codec_RS.hpp"
#include "Factory/Tools/Codec/RSC/Codec_RSC.hpp"
#include "Factory/Tools/Codec/RSC_DB/Codec_RSC_DB.hpp"
#include "Factory/Tools/Codec/Turbo/Codec_turbo.hpp"
#include "Factory/Tools/Codec/Turbo_DB/Codec_turbo_DB.hpp"
#include "Factory/Tools/Codec/Uncoded/Codec_uncoded.hpp"
#include "Factory/Tools/Codec/Turbo_product/Codec_turbo_product.hpp"
#include "Factory/Tools/Codec/Codec_HIHO.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Codec_HIHO_name   = "Codec HIHO";
const std::string aff3ct::factory::Codec_HIHO_prefix = "cdc";

Codec_HIHO
::Codec_HIHO(const std::string &prefix)
: Codec(Codec_HIHO_name, prefix)
{
}

Codec_HIHO
::Codec_HIHO(const std::string &name, const std::string &prefix)
: Codec(name, prefix)
{
}

template <typename B, typename Q>
tools::Codec_HIHO<B,Q>* Codec_HIHO
::build(const module::CRC<B>* crc) const
{
	if (get_name() == Codec_BCH_name          ) return dynamic_cast<const Codec_BCH          &>(*this).template build<B,Q>(crc);
	if (get_name() == Codec_LDPC_name         ) return dynamic_cast<const Codec_LDPC         &>(*this).template build<B,Q>(crc);
	if (get_name() == Codec_polar_name        ) return dynamic_cast<const Codec_polar        &>(*this).template build<B,Q>(crc);
	if (get_name() == Codec_RA_name           ) return dynamic_cast<const Codec_RA           &>(*this).template build<B,Q>(crc);
	if (get_name() == Codec_repetition_name   ) return dynamic_cast<const Codec_repetition   &>(*this).template build<B,Q>(crc);
	if (get_name() == Codec_RS_name           ) return dynamic_cast<const Codec_RS           &>(*this).template build<B,Q>(crc);
	if (get_name() == Codec_RSC_name          ) return dynamic_cast<const Codec_RSC          &>(*this).template build<B,Q>(crc);
	if (get_name() == Codec_RSC_DB_name       ) return dynamic_cast<const Codec_RSC_DB       &>(*this).template build<B,Q>(crc);
	if (get_name() == Codec_turbo_name        ) return dynamic_cast<const Codec_turbo        &>(*this).template build<B,Q>(crc);
	if (get_name() == Codec_turbo_DB_name     ) return dynamic_cast<const Codec_turbo_DB     &>(*this).template build<B,Q>(crc);
	if (get_name() == Codec_uncoded_name      ) return dynamic_cast<const Codec_uncoded      &>(*this).template build<B,Q>(crc);
	if (get_name() == Codec_turbo_product_name) return dynamic_cast<const Codec_turbo_product&>(*this).template build<B,Q>(crc);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::tools::Codec_HIHO<B_8 ,Q_8 >* aff3ct::factory::Codec_HIHO::build<B_8 ,Q_8 >(const aff3ct::module::CRC<B_8 >*) const;
template aff3ct::tools::Codec_HIHO<B_16,Q_16>* aff3ct::factory::Codec_HIHO::build<B_16,Q_16>(const aff3ct::module::CRC<B_16>*) const;
template aff3ct::tools::Codec_HIHO<B_32,Q_32>* aff3ct::factory::Codec_HIHO::build<B_32,Q_32>(const aff3ct::module::CRC<B_32>*) const;
template aff3ct::tools::Codec_HIHO<B_64,Q_64>* aff3ct::factory::Codec_HIHO::build<B_64,Q_64>(const aff3ct::module::CRC<B_64>*) const;
#else
template aff3ct::tools::Codec_HIHO<B,Q>* aff3ct::factory::Codec_HIHO::build<B,Q>(const aff3ct::module::CRC<B>*) const;
#endif
// ==================================================================================== explicit template instantiation
