#include "Tools/Math/utils.h"

#include "Tools/Code/Turbo/Post_processing_SISO/Self_corrected/Self_corrected.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

template <typename B, typename R>
Self_corrected<B,R>
::Self_corrected(const int K, const int n_ite, const int m, const int M)
: Post_processing_SISO<B,R>     (                                             ),
  K                             (K                                            ),
  m                             (m                                            ),
  M                             (M == -1 ? n_ite : M                          ),
  ext_nat(n_ite, mipp::vector<R>(K * this->n_frames + mipp::nElReg<R>(), (R)0)),
  ext_int(n_ite, mipp::vector<R>(K * this->n_frames + mipp::nElReg<R>(), (R)0)),
  osc_nat                       (K * this->n_frames + mipp::nElReg<B>(), (B)0 ),
  osc_int                       (K * this->n_frames + mipp::nElReg<B>(), (B)0 ),
  previously_corrected_nat      (K * this->n_frames + mipp::nElReg<B>(), (B)0 ),
  previously_corrected_int      (K * this->n_frames + mipp::nElReg<B>(), (B)0 )
{
}

template <typename B, typename R>
Self_corrected<B,R>* Self_corrected<B,R>
::clone() const
{
	auto t = new Self_corrected(*this);
	t->deep_copy(*this);
	return t;
}

template <typename B, typename R>
bool Self_corrected<B,R>
::siso_n(const int ite,
         const mipp::vector<R>& sys,
               mipp::vector<R>& ext,
               mipp::vector<B>& s)
{
	if (ite <= M)
	{
		this->collect(ite, ext, this->ext_nat);
		if (ite > 1)
		{
			this->calc_osc(ite, this->ext_nat, this->osc_nat);
			if (ite >= m)
				this->correct(this->osc_nat, this->previously_corrected_nat, ext);
		}
	}

	return false;
}

template <typename B, typename R>
bool Self_corrected<B,R>
::siso_i(const int ite,
         const mipp::vector<R>& sys,
               mipp::vector<R>& ext)
{
	if (ite <= M)
	{
		this->collect(ite, ext, this->ext_int);
		if (ite > 1)
		{
			this->calc_osc(ite, this->ext_int, this->osc_int);
			if (ite >= m)
				this->correct(this->osc_int, this->previously_corrected_int, ext);
		}
	}

	return false;
}

template <typename B, typename R>
inline void Self_corrected<B,R>
::collect(const int ite, const mipp::vector<R> &cur_ext, std::vector<mipp::vector<R>> &ext_hist)
{
	// ite starts at 1 in tdec process
	ext_hist[ite -1] = cur_ext;
}

template <typename B, typename R>
inline void Self_corrected<B,R>
::calc_osc(const int ite, const std::vector<mipp::vector<R>> &ext_hist, mipp::vector<B> &osc)
{
	B sgn_cur, sgn_prev;
	for (auto idx = 0; idx < (int)osc.size(); idx++)
	{
		sgn_cur  = sgn<B,R>(ext_hist[ite-1][idx]);
		sgn_prev = sgn<B,R>(ext_hist[ite-2][idx]);
		osc[idx] = sgn_cur != sgn_prev;
	}
}

template <typename B, typename R>
inline void Self_corrected<B,R>
::correct(const mipp::vector<B> &osc, mipp::vector<B> &prev_corr, mipp::vector<R> &ext)
{
	for (auto idx = 0; idx < (int)ext.size(); idx++)
	{
		if (osc[idx] && !prev_corr[idx])
		{
			ext[idx]       = (R)0;
			prev_corr[idx] = (B)1;
		}
		else
			prev_corr[idx] = (B)0;
	}
}

template <typename B, typename R>
void Self_corrected<B,R>
::set_n_frames(const size_t n_frames)
{
	const auto old_n_frames = this->get_n_frames();
	if (n_frames != old_n_frames)
	{
		Post_processing_SISO<B,R>::set_n_frames(n_frames);

		for (auto &ext : ext_nat)
			ext.resize(this->K * this->n_frames + mipp::nElReg<R>(), (R)0);
		for (auto &ext : ext_int)
			ext.resize(this->K * this->n_frames + mipp::nElReg<R>(), (R)0);

		osc_nat                 .resize(this->K * this->n_frames + mipp::nElReg<B>(), (B)0);
		osc_int                 .resize(this->K * this->n_frames + mipp::nElReg<B>(), (B)0);
		previously_corrected_nat.resize(this->K * this->n_frames + mipp::nElReg<B>(), (B)0);
		previously_corrected_int.resize(this->K * this->n_frames + mipp::nElReg<B>(), (B)0);
	}
}


// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::tools::Self_corrected<B_8,Q_8>;
template class aff3ct::tools::Self_corrected<B_16,Q_16>;
template class aff3ct::tools::Self_corrected<B_32,Q_32>;
template class aff3ct::tools::Self_corrected<B_64,Q_64>;
#else
template class aff3ct::tools::Self_corrected<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
