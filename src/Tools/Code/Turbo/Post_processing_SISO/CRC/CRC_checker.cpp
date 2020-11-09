#include "Tools/Code/Turbo/Post_processing_SISO/CRC/CRC_checker.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

template <typename B, typename R>
CRC_checker<B,R>
::CRC_checker(const module::CRC<B> &crc, const int start_crc_check_ite)
: Post_processing_SISO<B,R>(),
  start_crc_check_ite(start_crc_check_ite),
  crc                (crc.clone())
{
	this->set_n_frames(this->crc->get_n_frames());
}

template <typename B, typename R>
CRC_checker<B,R>* CRC_checker<B,R>
::clone() const
{
	auto t = new CRC_checker(*this);
	t->deep_copy(*this);
	return t;
}


template <typename B, typename R>
void CRC_checker<B,R>
::deep_copy(const CRC_checker<B,R> &t)
{
	Post_processing_SISO<B,R>::deep_copy(t);
	if (t.crc != nullptr) this->crc.reset(t.crc->clone());
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

		return crc->check(s);
	}

	return false;
}

template <typename B, typename R>
void CRC_checker<B,R>
::set_n_frames(const size_t n_frames)
{
	const auto old_n_frames = this->get_n_frames();
	if (n_frames != old_n_frames)
	{
		Post_processing_SISO<B,R>::set_n_frames(n_frames);
		crc->set_n_frames(n_frames);
	}
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::tools::CRC_checker<B_8,Q_8>;
template class aff3ct::tools::CRC_checker<B_16,Q_16>;
template class aff3ct::tools::CRC_checker<B_32,Q_32>;
template class aff3ct::tools::CRC_checker<B_64,Q_64>;
#else
template class aff3ct::tools::CRC_checker<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
