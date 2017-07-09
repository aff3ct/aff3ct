#include "CRC_checker.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B, typename R>
CRC_checker<B,R>
::CRC_checker(CRC<B> &crc, const int start_crc_check_ite, const int simd_inter_frame_level)
: Post_processing_SISO<B,R>(),
  start_crc_check_ite   (start_crc_check_ite   ),
  simd_inter_frame_level(simd_inter_frame_level),
  crc                   (crc                   )
{
}

template <typename B, typename R>
CRC_checker<B,R>
::~CRC_checker()
{
}

template <typename B, typename R>
bool CRC_checker<B,R>
::siso_n(const int ite,
         const mipp::vector<R>& sys,
               mipp::vector<R>& ext,
               mipp::vector<B>& s)
{
	if (ite >= start_crc_check_ite)
	{
		// compute the hard decision (for the CRC): fast version
		const auto loop_size1 = (int)s.size() / mipp::nElReg<R>();
		for (auto i = 0; i < loop_size1; i++)
		{
			const auto r_sys  = mipp::Reg<R>(&sys[i * mipp::nElReg<R>()]);
			const auto r_ext  = mipp::Reg<R>(&ext[i * mipp::nElReg<R>()]);
			const auto r_post = r_sys + r_ext;
			const auto r_dec  = mipp::cast<R,B>(r_post) >> (sizeof(B) * 8 - 1);
			r_dec.store(&s[i * mipp::nElReg<R>()]);
		}
		const auto loop_size2 = (int)s.size();
		for (auto i = loop_size1 * mipp::nElReg<R>(); i < loop_size2; i++)
			s[i] = (sys[i] + ext[i]) < 0;

		return crc.check(s, simd_inter_frame_level);
	}

	return false;
}

namespace aff3ct
{
namespace tools
{
template <>
bool CRC_checker<int64_t, double>
::siso_n(const int ite,
         const mipp::vector<double >& sys,
               mipp::vector<double >& ext,
               mipp::vector<int64_t>& s)
{
	if (ite >= start_crc_check_ite)
	{
		// compute the hard decision (for the CRC)
		const auto loop_size = (int)s.size();
		for (auto i = 0; i < loop_size; i++)
			s[i] = (sys[i] + ext[i]) < 0;

		return crc.check(s, simd_inter_frame_level);
	}

	return false;
}
}
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::tools::CRC_checker<B_8,Q_8>;
template class aff3ct::tools::CRC_checker<B_16,Q_16>;
template class aff3ct::tools::CRC_checker<B_32,Q_32>;
template class aff3ct::tools::CRC_checker<B_64,Q_64>;
#else
template class aff3ct::tools::CRC_checker<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
