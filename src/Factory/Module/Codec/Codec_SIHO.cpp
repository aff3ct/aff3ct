#include "BCH/Codec_BCH.hpp"
#include "LDPC/Codec_LDPC.hpp"
#include "Polar/Codec_polar.hpp"
#include "RA/Codec_RA.hpp"
#include "Repetition/Codec_repetition.hpp"
#include "RS/Codec_RS.hpp"
#include "RSC/Codec_RSC.hpp"
#include "RSC_DB/Codec_RSC_DB.hpp"
#include "Turbo/Codec_turbo.hpp"
#include "Turbo_DB/Codec_turbo_DB.hpp"
#include "Uncoded/Codec_uncoded.hpp"
#include "Turbo_product/Codec_turbo_product.hpp"

#include "Factory/Module/Codec/Codec_SIHO.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Codec_SIHO_name   = "Codec SIHO";
const std::string aff3ct::factory::Codec_SIHO_prefix = "cdc";

Codec_SIHO::parameters
::parameters(const std::string &prefix)
: Codec::parameters(Codec_SIHO_name, prefix)
{
}

Codec_SIHO::parameters
::parameters(const std::string &name, const std::string &prefix)
: Codec::parameters(name, prefix)
{
}

template <typename B, typename Q>
module::Codec_SIHO<B,Q>* Codec_SIHO::parameters
::build(module::CRC<B>* crc) const
{
	if (get_name() == Codec_BCH_name          ) return dynamic_cast<const Codec_BCH          ::parameters&>(*this).template build<B,Q>(crc);
	if (get_name() == Codec_LDPC_name         ) return dynamic_cast<const Codec_LDPC         ::parameters&>(*this).template build<B,Q>(crc);
	if (get_name() == Codec_polar_name        ) return dynamic_cast<const Codec_polar        ::parameters&>(*this).template build<B,Q>(crc);
	if (get_name() == Codec_RA_name           ) return dynamic_cast<const Codec_RA           ::parameters&>(*this).template build<B,Q>(crc);
	if (get_name() == Codec_repetition_name   ) return dynamic_cast<const Codec_repetition   ::parameters&>(*this).template build<B,Q>(crc);
	if (get_name() == Codec_RS_name           ) return dynamic_cast<const Codec_RS           ::parameters&>(*this).template build<B,Q>(crc);
	if (get_name() == Codec_RSC_name          ) return dynamic_cast<const Codec_RSC          ::parameters&>(*this).template build<B,Q>(crc);
	if (get_name() == Codec_RSC_DB_name       ) return dynamic_cast<const Codec_RSC_DB       ::parameters&>(*this).template build<B,Q>(crc);
	if (get_name() == Codec_turbo_name        ) return dynamic_cast<const Codec_turbo        ::parameters&>(*this).template build<B,Q>(crc);
	if (get_name() == Codec_turbo_DB_name     ) return dynamic_cast<const Codec_turbo_DB     ::parameters&>(*this).template build<B,Q>(crc);
	if (get_name() == Codec_uncoded_name      ) return dynamic_cast<const Codec_uncoded      ::parameters&>(*this).template build<B,Q>(crc);
	if (get_name() == Codec_turbo_product_name) return dynamic_cast<const Codec_turbo_product::parameters&>(*this).template build<B,Q>(crc);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q>
module::Codec_SIHO<B,Q>* Codec_SIHO
::build(const parameters &params, module::CRC<B> *crc)
{
	return params.template build<B,Q>(crc);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::module::Codec_SIHO<B_8 ,Q_8 >* aff3ct::factory::Codec_SIHO::parameters::build<B_8 ,Q_8 >(aff3ct::module::CRC<B_8 >*) const;
template aff3ct::module::Codec_SIHO<B_16,Q_16>* aff3ct::factory::Codec_SIHO::parameters::build<B_16,Q_16>(aff3ct::module::CRC<B_16>*) const;
template aff3ct::module::Codec_SIHO<B_32,Q_32>* aff3ct::factory::Codec_SIHO::parameters::build<B_32,Q_32>(aff3ct::module::CRC<B_32>*) const;
template aff3ct::module::Codec_SIHO<B_64,Q_64>* aff3ct::factory::Codec_SIHO::parameters::build<B_64,Q_64>(aff3ct::module::CRC<B_64>*) const;
template aff3ct::module::Codec_SIHO<B_8 ,Q_8 >* aff3ct::factory::Codec_SIHO::build<B_8 ,Q_8 >(const aff3ct::factory::Codec_SIHO::parameters&, aff3ct::module::CRC<B_8 >*);
template aff3ct::module::Codec_SIHO<B_16,Q_16>* aff3ct::factory::Codec_SIHO::build<B_16,Q_16>(const aff3ct::factory::Codec_SIHO::parameters&, aff3ct::module::CRC<B_16>*);
template aff3ct::module::Codec_SIHO<B_32,Q_32>* aff3ct::factory::Codec_SIHO::build<B_32,Q_32>(const aff3ct::factory::Codec_SIHO::parameters&, aff3ct::module::CRC<B_32>*);
template aff3ct::module::Codec_SIHO<B_64,Q_64>* aff3ct::factory::Codec_SIHO::build<B_64,Q_64>(const aff3ct::factory::Codec_SIHO::parameters&, aff3ct::module::CRC<B_64>*);
#else
template aff3ct::module::Codec_SIHO<B,Q>* aff3ct::factory::Codec_SIHO::parameters::build<B,Q>(aff3ct::module::CRC<B>*) const;
template aff3ct::module::Codec_SIHO<B,Q>* aff3ct::factory::Codec_SIHO::build<B,Q>(const aff3ct::factory::Codec_SIHO::parameters&, aff3ct::module::CRC<B>*);
#endif
// ==================================================================================== explicit template instantiation
