#include <algorithm>
#include <cmath>

#include "Tools/Code/Turbo/Post_processing_SISO/Flip_and_check/Flip_and_check.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

template <typename B, typename R>
Flip_and_check<B,R>
::Flip_and_check(const int K, const int n_ite, const module::CRC<B> &crc, const int start_crc_check_ite,
                 const int q, const int m, const int M, const int s)
: CRC_checker<B,R>(crc, start_crc_check_ite),
  K               (K                                    ),
  q               (q                                    ),
  metric          (K * this->n_frames                   ),
  s_tmp           (K * this->n_frames                   ),
  tab_flips       ((1 << q) -1, mipp::vector<B>(q, (B)0))
{
	// generation of the array that will be used to flip the bits
	// it contains: 1000, 0100, 1100, 0010, ...
	for (auto i = 1; i <= (int)tab_flips.size(); i++)
	{
		auto b10 = i; // number in base 10
		auto l   = 0;
		while (b10 > 0)
		{
			tab_flips[i -1][l] = b10 % 2;
			b10 /= 2;
			l++;
		}
	}

	// fnc_ite contains the iterations for which the FNC will be performed
	auto ite = m;
	do
	{
		fnc_ite.push_back(ite);
		ite += s;
	}
	while (ite <= ((M == -1) ? n_ite : M));
}

template <typename B, typename R>
Flip_and_check<B,R>* Flip_and_check<B,R>
::clone() const
{
	auto t = new Flip_and_check(*this);
	t->deep_copy(*this);
	return t;
}

template <typename B, typename R>
bool Flip_and_check<B,R>
::siso_n(const int ite,
         const mipp::vector<R>& sys,
               mipp::vector<R>& ext,
               mipp::vector<B>& s)
{
	auto stop = CRC_checker<B,R>::siso_n(ite, sys, ext, s);

	if (!stop && std::binary_search(fnc_ite.begin(), fnc_ite.end(), ite))
		return apply_flip_and_check(sys, ext, s);
	else
		return stop;
}

template <typename R>
mipp::vector<unsigned int> partial_sort_indexes(const mipp::vector<R> &v)
{
	// initialize original index locations
	mipp::vector<unsigned int> idx(v.size());
	for (size_t i = 0; i != idx.size(); ++i)
		idx[i] = (unsigned int)i;
	// sort indexes based on comparing values in v
	std::partial_sort(idx.begin(),idx.begin() + 20, idx.end(),
	                  [&v](size_t i1, size_t i2) { return v[i1] < v[i2]; });
	return idx;
}

template <typename B, typename R>
bool Flip_and_check<B,R>
::apply_flip_and_check(const mipp::vector<R>& sys, const mipp::vector<R>& ext, mipp::vector<B>& s)
{
	// reconstruct the a posteriori information and calculate the metric associated
	for (size_t i = 0; i < this->K * this->n_frames; i++)
		metric[i] = std::abs(sys[i] + ext[i]);

	// get the least reliable positions
	mipp::vector<unsigned int> positions = partial_sort_indexes(metric);

	// test all patterns by xoring with tab_flib
	bool check_crc = false;
	auto pattern = 0;
	do
	{
		std::copy(s.begin(), s.end(), s_tmp.begin());
		for (auto depth = 0; depth < q; depth++)
			s_tmp[positions[depth]] ^= tab_flips[pattern][depth];
		check_crc = this->crc->check(s_tmp);
		pattern++;
	}
	while ((pattern < (int)tab_flips.size()) && !check_crc);

	// if the crc is verified, replace the decoder's output
	if (check_crc)
		std::copy(s_tmp.begin(), s_tmp.end(), s.begin());

	return check_crc;
}

template <typename B, typename R>
void Flip_and_check<B,R>
::set_n_frames(const size_t n_frames)
{
	const auto old_n_frames = this->get_n_frames();
	if (n_frames != old_n_frames)
	{
		CRC_checker<B,R>::set_n_frames(n_frames);

		const auto old_metric_size = this->metric.size();
		const auto new_metric_size = (old_metric_size / old_n_frames) * n_frames;
		this->metric.resize(new_metric_size);

		const auto old_s_tmp_size = this->s_tmp.size();
		const auto new_s_tmp_size = (old_s_tmp_size / old_n_frames) * n_frames;
		this->s_tmp.resize(new_s_tmp_size);
	}
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::tools::Flip_and_check<B_8,Q_8>;
template class aff3ct::tools::Flip_and_check<B_16,Q_16>;
template class aff3ct::tools::Flip_and_check<B_32,Q_32>;
template class aff3ct::tools::Flip_and_check<B_64,Q_64>;
#else
template class aff3ct::tools::Flip_and_check<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
