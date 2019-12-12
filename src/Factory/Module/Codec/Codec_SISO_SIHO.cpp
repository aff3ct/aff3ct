#include "Factory/Module/Codec/LDPC/Codec_LDPC.hpp"
#include "Factory/Module/Codec/Polar/Codec_polar.hpp"
#include "Factory/Module/Codec/RSC/Codec_RSC.hpp"
#include "Factory/Module/Codec/RSC_DB/Codec_RSC_DB.hpp"
#include "Factory/Module/Codec/Uncoded/Codec_uncoded.hpp"
#include "Factory/Module/Codec/Codec_SISO_SIHO.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Codec_SISO_SIHO_name   = "Codec SISO/SIHO";
const std::string aff3ct::factory::Codec_SISO_SIHO_prefix = "cdc";

Codec_SISO_SIHO
::Codec_SISO_SIHO(const std::string &prefix)
: Codec     (Codec_SISO_SIHO_name, prefix),
  Codec_SISO(Codec_SISO_SIHO_name, prefix),
  Codec_SIHO(Codec_SISO_SIHO_name, prefix)
{
}

Codec_SISO_SIHO
::Codec_SISO_SIHO(const std::string &name, const std::string &prefix)
: Codec(name, prefix)
{
}

template <typename B, typename Q>
module::Codec_SISO_SIHO<B,Q>* Codec_SISO_SIHO
::build(module::CRC<B>* crc) const
{
	if (get_name() == Codec_LDPC_name   ) return dynamic_cast<const Codec_LDPC   &>(*this).template build<B,Q>(crc);
	if (get_name() == Codec_polar_name  ) return dynamic_cast<const Codec_polar  &>(*this).template build<B,Q>(crc);
	if (get_name() == Codec_RSC_name    ) return dynamic_cast<const Codec_RSC    &>(*this).template build<B,Q>(crc);
	if (get_name() == Codec_RSC_DB_name ) return dynamic_cast<const Codec_RSC_DB &>(*this).template build<B,Q>(crc);
	if (get_name() == Codec_uncoded_name) return dynamic_cast<const Codec_uncoded&>(*this).template build<B,Q>(crc);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::module::Codec_SISO_SIHO<B_8 ,Q_8 >* aff3ct::factory::Codec_SISO_SIHO::build<B_8 ,Q_8 >(aff3ct::module::CRC<B_8 >*) const;
template aff3ct::module::Codec_SISO_SIHO<B_16,Q_16>* aff3ct::factory::Codec_SISO_SIHO::build<B_16,Q_16>(aff3ct::module::CRC<B_16>*) const;
template aff3ct::module::Codec_SISO_SIHO<B_32,Q_32>* aff3ct::factory::Codec_SISO_SIHO::build<B_32,Q_32>(aff3ct::module::CRC<B_32>*) const;
template aff3ct::module::Codec_SISO_SIHO<B_64,Q_64>* aff3ct::factory::Codec_SISO_SIHO::build<B_64,Q_64>(aff3ct::module::CRC<B_64>*) const;
#else
template aff3ct::module::Codec_SISO_SIHO<B,Q>* aff3ct::factory::Codec_SISO_SIHO::build<B,Q>(aff3ct::module::CRC<B>*) const;
#endif
// ==================================================================================== explicit template instantiation
