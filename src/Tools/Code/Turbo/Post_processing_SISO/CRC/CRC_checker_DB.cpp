#include <algorithm>

#include "Tools/Code/Turbo/Post_processing_SISO/CRC/CRC_checker_DB.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

template <typename B, typename R>
CRC_checker_DB<B,R>
::CRC_checker_DB(module::CRC<B> &crc, const int start_crc_check_ite, const int simd_inter_frame_level)
: CRC_checker<B,R>(crc, start_crc_check_ite, simd_inter_frame_level),
  apost           (2 * (crc.get_K() + crc.get_size()) * simd_inter_frame_level)
{
}

template <typename B, typename R>
bool CRC_checker_DB<B,R>
::siso_n(const int ite,
         const mipp::vector<R>& sys,
               mipp::vector<R>& ext,
               mipp::vector<B>& s)
{
	if (ite >= this->start_crc_check_ite)
	{
		for (auto i = 0; i < (int)apost.size(); i++)
			apost[i] = sys[i] + ext[i];

		// compute the hard decision (for the CRC)
		const auto loop_size = (int)s.size();
		for (auto i = 0; i < loop_size; i+=2)
		{
			s[i  ] = (std::max(apost[2*i+2], apost[2*i+3]) - std::max(apost[2*i+0], apost[2*i+1])) > 0;
			s[i+1] = (std::max(apost[2*i+1], apost[2*i+3]) - std::max(apost[2*i+0], apost[2*i+2])) > 0;
		}
		return this->crc.check(s, this->simd_inter_frame_level);
	}

	return false;
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::tools::CRC_checker_DB<B_8,Q_8>;
template class aff3ct::tools::CRC_checker_DB<B_16,Q_16>;
template class aff3ct::tools::CRC_checker_DB<B_32,Q_32>;
template class aff3ct::tools::CRC_checker_DB<B_64,Q_64>;
#else
template class aff3ct::tools::CRC_checker_DB<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
