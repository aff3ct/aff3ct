#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <type_traits>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <limits>
#include <vector>
#include <mipp.h>

#include "Tools/Perf/common/mutual_info.h"

#ifdef MIPP_AVX1

template <typename B, typename R>
R aff3ct::tools::mutual_info_histo(const B* ref, const R* llr, const unsigned size)
{
	return mutual_info_histo_seq(ref, llr, size);
}

#else

template <typename B, typename R>
R aff3ct::tools::mutual_info_histo(const B* ref, const R* llr, const unsigned size)
{
	static_assert(mipp::N<B>() == mipp::N<R>(), "B and R shall have the same size");
	if (std::is_same<R,double>::value)
		return mutual_info_histo_seq(ref, llr, size);

	/* determine the min and max value for llrs / 0 and llrs / 1
	 * compute the llr_sum  as the sum of the llr[i]
	 * compute the llr_sum2 as the sum of the llr[i]^2
	 * compute the llr_mean as llr_sum / llr_noninf_count
	 * compute the llr_variance as sum of (llr[i] - mean)^2 / llr_noninf_count
	 *                              = sum( llr_sum2 - 2 * mean * llr_sum + llr_noninf_count * mean^2 ) / llr_noninf_count
	 *                              = sum( llr_sum2 - 2 * mean * llr_sum + llr_sum * mean ) / llr_noninf_count
	 *                              = sum( llr_sum2 - 1 * mean * llr_sum) / llr_noninf_count
	 */

	const R inf = std::numeric_limits<R>::infinity();

	const mipp::Reg<B> Bzeros = (B)0, Bones = (B)1;
	const mipp::Reg<R> Rzeros = (R)0, Rones = (R)1;
	const mipp::Reg<R> r_infp = (R)+inf;
	const mipp::Reg<R> r_infn = (R)-inf;



	mipp::Reg<B> r_bit_1_count        = (B)0; // sum of ref at 1
	mipp::Reg<B> r_llr_0_noninf_count = (B)0; // sum of non infinite llr when ref is 0
	mipp::Reg<B> r_llr_1_noninf_count = (B)0; // sum of non infinite llr when ref is 1
	mipp::Reg<B> r_llr_0_infpos_count = (B)0; // sum of positive infinite llr when ref is 0
	mipp::Reg<B> r_llr_0_infneg_count = (B)0; // sum of negative infinite llr when ref is 0
	mipp::Reg<B> r_llr_1_infpos_count = (B)0; // sum of positive infinite llr when ref is 1
	mipp::Reg<B> r_llr_1_infneg_count = (B)0; // sum of negative infinite llr when ref is 1

	mipp::Reg<R> r_llr_0_sum  = (R)0;
	mipp::Reg<R> r_llr_1_sum  = (R)0;
	mipp::Reg<R> r_llr_0_sum2 = (R)0;
	mipp::Reg<R> r_llr_1_sum2 = (R)0;

	mipp::Reg<R> r_llr_0_max = (R)-inf;
	mipp::Reg<R> r_llr_1_max = (R)-inf;
	mipp::Reg<R> r_llr_0_min = (R)+inf;
	mipp::Reg<R> r_llr_1_min = (R)+inf;

	auto vec_loop_size = (size / mipp::N<B>()) * mipp::N<B>();

	for (unsigned i = 0; i < vec_loop_size; i += mipp::N<B>())
	{
		const mipp::Reg<B> r_ref = ref + i;
		const auto m_ref = r_ref != Bzeros; // mask is true when ref is not null

		r_bit_1_count += mipp::blend(Bones, Bzeros, m_ref);

		const mipp::Reg<R> r_llr = llr + i;
		auto m_llr_infp = r_llr == r_infp;                 // mask is true when llr is infinite positive
		auto m_llr_infn = r_llr == r_infn;                 // mask is true when llr is infinite negative
		const auto m_llr_ninf = (~m_llr_infp) & (~m_llr_infn);   // mask is true when llr is not infinite (pos or neg)

		const auto m_ref0_ninf = mipp::andnb(m_ref, m_llr_ninf); // mask is true when ref is 0 and llr is not infinite
		const auto m_ref1_ninf = mipp::andb (m_ref, m_llr_ninf); // mask is true when ref is 1 and llr is not infinite

		const auto m_ref0_infp = mipp::andnb(m_ref, m_llr_infp); // mask is true when ref is 0 and llr is infinite positive
		const auto m_ref1_infp = mipp::andb (m_ref, m_llr_infp); // mask is true when ref is 1 and llr is infinite positive

		const auto m_ref0_infn = mipp::andnb(m_ref, m_llr_infn); // mask is true when ref is 0 and llr is infinite negative
		const auto m_ref1_infn = mipp::andb (m_ref, m_llr_infn); // mask is true when ref is 1 and llr is infinite negative


		r_llr_0_noninf_count += mipp::blend(Bones, Bzeros, m_ref0_ninf);
		r_llr_1_noninf_count += mipp::blend(Bones, Bzeros, m_ref1_ninf);

		r_llr_0_infpos_count += mipp::blend(Bones, Bzeros, m_ref0_infp);
		r_llr_1_infpos_count += mipp::blend(Bones, Bzeros, m_ref1_infp);

		r_llr_0_infneg_count += mipp::blend(Bones, Bzeros, m_ref0_infn);
		r_llr_1_infneg_count += mipp::blend(Bones, Bzeros, m_ref1_infn);


		r_llr_0_sum  += mipp::blend(r_llr,         Rzeros, m_ref0_ninf);
		r_llr_0_sum2 += mipp::blend(r_llr * r_llr, Rzeros, m_ref0_ninf);

		r_llr_1_sum  += mipp::blend(r_llr,         Rzeros, m_ref1_ninf);
		r_llr_1_sum2 += mipp::blend(r_llr * r_llr, Rzeros, m_ref1_ninf);


		r_llr_0_min = mipp::blend(mipp::min(r_llr, r_llr_0_min), r_llr_0_min, m_ref0_ninf);
		r_llr_0_max = mipp::blend(mipp::max(r_llr, r_llr_0_max), r_llr_0_max, m_ref0_ninf);

		r_llr_1_min = mipp::blend(mipp::min(r_llr, r_llr_1_min), r_llr_1_min, m_ref1_ninf);
		r_llr_1_max = mipp::blend(mipp::max(r_llr, r_llr_1_max), r_llr_1_max, m_ref1_ninf);
	}

	// reductions
	B bit_1_count        = mipp::hadd(r_bit_1_count       );
	B llr_0_noninf_count = mipp::hadd(r_llr_0_noninf_count);
	B llr_1_noninf_count = mipp::hadd(r_llr_1_noninf_count);
	B llr_0_infpos_count = mipp::hadd(r_llr_0_infpos_count);
	B llr_0_infneg_count = mipp::hadd(r_llr_0_infneg_count);
	B llr_1_infpos_count = mipp::hadd(r_llr_1_infpos_count);
	B llr_1_infneg_count = mipp::hadd(r_llr_1_infneg_count);

	R llr_0_sum  = mipp::hadd(r_llr_0_sum );
	R llr_1_sum  = mipp::hadd(r_llr_1_sum );
	R llr_0_sum2 = mipp::hadd(r_llr_0_sum2);
	R llr_1_sum2 = mipp::hadd(r_llr_1_sum2);

	R llr_0_max = mipp::hmax(r_llr_0_max);
	R llr_1_max = mipp::hmax(r_llr_1_max);
	R llr_0_min = mipp::hmin(r_llr_0_min);
	R llr_1_min = mipp::hmin(r_llr_1_min);

	// finishes the loop sequentially if needed
	for (unsigned i = vec_loop_size; i < size; i++)
	{
		bit_1_count += ref[i] ? (unsigned)1 : (unsigned)0;

		if (llr[i] == +inf)
		{
			if (ref[i])
				llr_1_infpos_count ++;
			else
				llr_0_infpos_count ++;
		}
		else if (llr[i] == -inf)
		{
			if (ref[i])
				llr_1_infneg_count ++;
			else
				llr_0_infneg_count ++;
		}
		else
		{
			if (ref[i])
			{
				llr_1_noninf_count++;

				llr_1_sum  += llr[i];
				llr_1_sum2 += llr[i] * llr[i];

				llr_1_min = std::min(llr[i], llr_1_min);
				llr_1_max = std::max(llr[i], llr_1_max);
			}
			else
			{
				llr_0_noninf_count++;

				llr_0_sum  += llr[i];
				llr_0_sum2 += llr[i] * llr[i];

				llr_0_min = std::min(llr[i], llr_0_min);
				llr_0_max = std::max(llr[i], llr_0_max);
			}
		}
	}


	B bit_0_count = (B)size - bit_1_count;

	// if all ones or all zeros then quit
	if (bit_1_count == 0 || bit_0_count == 0)
		return (R)0;



	bool     lots_of_bins;
	unsigned bin_count;
	int      bin_offset = 0;
	R        bin_width  = (R)0;

	if (llr_0_noninf_count && llr_1_noninf_count && llr_0_min <= llr_1_max && llr_1_min <= llr_0_max)
	{
		R llr_0_mean     = llr_0_sum / (R)llr_0_noninf_count;
		R llr_1_mean     = llr_1_sum / (R)llr_1_noninf_count;
		R llr_0_variance = (llr_0_sum2 - (R)1 * llr_0_mean * llr_0_sum) / (R)llr_0_noninf_count;
		R llr_1_variance = (llr_1_sum2 - (R)1 * llr_1_mean * llr_1_sum) / (R)llr_1_noninf_count;

		bin_width = (R)0.5 * ((R)3.49 * (R)std::sqrt(llr_0_variance) * (R)(std::pow(llr_0_noninf_count, (R)-1.0 / (R)3.0)) +
		                      (R)3.49 * (R)std::sqrt(llr_1_variance) * (R)(std::pow(llr_1_noninf_count, (R)-1.0 / (R)3.0)));

		if (bin_width > (R)0)
		{
			bin_offset = (int)std::floor(std::min(llr_0_min, llr_1_min) / bin_width) -1;

			auto tmp = std::max(llr_0_max, llr_1_max) / bin_width - (R)bin_offset + (R)1;
			bin_count = (unsigned)std::ceil(tmp);

			if ((R)bin_count == tmp)
				bin_count++;
		}
		else
		{
			bin_offset = -1;
			bin_count  = 3;
		}

		lots_of_bins = true;
	}
	else
	{
		lots_of_bins = false;
		bin_count    = 4;
	}


	std::vector<std::vector<B>> hist(2, std::vector<B>(bin_count, 0));


	hist[0].front() = llr_0_infneg_count;
	hist[0].back () = llr_0_infpos_count;
	hist[1].front() = llr_1_infneg_count;
	hist[1].back () = llr_1_infpos_count;

	if (!lots_of_bins)
	{
		hist[0][1] = llr_0_noninf_count;
		hist[1][2] = llr_1_noninf_count;
	}
	else if (bin_width == (R)0.0 || bin_offset == -1)
	{
		hist[0][1] = llr_0_noninf_count;
		hist[1][1] = llr_1_noninf_count;
	}
	else
	{
		for (unsigned i = 0; i < size; i++)
		{
			const unsigned hist_idx = ref[i] ? 1 : 0;
			hist[hist_idx][(int)(std::floor(llr[i] / bin_width) - bin_offset)] += std::isinf(llr[i]) ? 0 : 1;
		}
	}


	const mipp::Reg<R> r_ln2 = (R)M_LN2;
	mipp::Reg<R> r_MI = (R)0;

	vec_loop_size = (bin_count / mipp::N<B>()) * mipp::N<B>();

	for (unsigned i = 0; i < vec_loop_size; i += mipp::N<B>())
	{
		const mipp::Reg<B> r_hist0 = hist[0].data() + i;
		const mipp::Reg<B> r_hist1 = hist[1].data() + i;
		const auto m_hist0 = r_hist0 != Bzeros; // mask is true when hist0 is not null
		const auto m_hist1 = r_hist1 != Bzeros; // mask is true when hist1 is not null

		const auto r_hist0_R = mipp::cvt<B, R>(r_hist0) / (R)bit_0_count;
		const auto r_hist1_R = mipp::cvt<B, R>(r_hist1) / (R)bit_1_count;

		const auto r_ln_sum = r_ln2 - mipp::log(r_hist0_R + r_hist1_R);

		const auto temp0 = r_hist0_R * (r_ln_sum + mipp::log(r_hist0_R));
		const auto temp1 = r_hist1_R * (r_ln_sum + mipp::log(r_hist1_R));

		r_MI += mipp::blend(temp0, Rzeros, m_hist0);
		r_MI += mipp::blend(temp1, Rzeros, m_hist1);
	}

	// reductions
	R MI = mipp::hadd(r_MI);

	// finish the loop sequentially if needed
	for (unsigned i = vec_loop_size; i < bin_count; i++)
	{
		const auto pdf0 = (hist[0][i] == 0)? (R)0 : (R)hist[0][i] / (R)bit_0_count;
		const auto pdf1 = (hist[1][i] == 0)? (R)0 : (R)hist[1][i] / (R)bit_1_count;
		const auto sum = pdf0 + pdf1;

		const auto ln_sum = (R)M_LN2 - std::log(sum);

		MI += (pdf0 == (R)0.0) ? (R)0.0 : pdf0 * (ln_sum + std::log(pdf0));
		MI += (pdf1 == (R)0.0) ? (R)0.0 : pdf1 * (ln_sum + std::log(pdf1));
	}

	return MI * (R)0.5 / (R)M_LN2;
}

#endif // #ifdef MIPP_AVX

template <typename B, typename R>
R aff3ct::tools::mutual_info_histo_seq(const B* ref, const R* llr, const unsigned size)
{
	const R inf = std::numeric_limits<R>::infinity();

	// determine the min and max value for llrs / 0 and llrs / 1
	// compute the llr_sum  as the sum of the llr[i]
	// compute the llr_sum2 as the sum of the llr[i]^2
	// compute the llr_mean as llr_sum / llr_noninf_count
	// compute the llr_variance as sum of (llr[i] - mean)^2 / llr_noninf_count
	//                              = sum( llr_sum2 - 2 * mean * llr_sum + llr_noninf_count * mean^2 ) / llr_noninf_count
	//                              = sum( llr_sum2 - 2 * mean * llr_sum + llr_sum * mean ) / llr_noninf_count
	//                              = sum( llr_sum2 - 1 * mean * llr_sum) / llr_noninf_count

	unsigned bit_1_count = 0;
	unsigned llr_0_noninf_count = 0;
	unsigned llr_1_noninf_count = 0;

	R llr_0_sum  = (R)0;
	R llr_1_sum  = (R)0;
	R llr_0_sum2 = (R)0;
	R llr_1_sum2 = (R)0;

	R llr_0_max = -inf;
	R llr_1_max = -inf;
	R llr_0_min = +inf;
	R llr_1_min = +inf;

	for (unsigned i = 0; i < size; i++)
	{
		bit_1_count += ref[i] ? (unsigned)1 : (unsigned)0;

		if (!std::isinf(llr[i]))
		{
			if (ref[i])
			{
				llr_1_noninf_count++;

				llr_1_sum  += llr[i];
				llr_1_sum2 += llr[i] * llr[i];

				llr_1_min = std::min(llr[i], llr_1_min);
				llr_1_max = std::max(llr[i], llr_1_max);
			}
			else
			{
				llr_0_noninf_count++;

				llr_0_sum  += llr[i];
				llr_0_sum2 += llr[i] * llr[i];

				llr_0_min = std::min(llr[i], llr_0_min);
				llr_0_max = std::max(llr[i], llr_0_max);
			}
		}
	}

	unsigned bit_0_count = size - bit_1_count;

	if (bit_0_count == 0 || bit_1_count == 0)
		return (R)0;


	bool     lots_of_bins;
	unsigned bin_count;
	int      bin_offset = 0;
	R        bin_width  = (R)0;

	if (llr_0_noninf_count && llr_1_noninf_count && llr_0_min <= llr_1_max && llr_1_min <= llr_0_max)
	{
		R llr_0_mean     = llr_0_sum / (R)llr_0_noninf_count;
		R llr_1_mean     = llr_1_sum / (R)llr_1_noninf_count;
		R llr_0_variance = (llr_0_sum2 - (R)1 * llr_0_mean * llr_0_sum) / (R)llr_0_noninf_count;
		R llr_1_variance = (llr_1_sum2 - (R)1 * llr_1_mean * llr_1_sum) / (R)llr_1_noninf_count;

		bin_width = (R)0.5 * ((R)3.49 * (R)std::sqrt(llr_0_variance) * (R)(std::pow(llr_0_noninf_count, (R)-1.0 / (R)3.0)) +
		                      (R)3.49 * (R)std::sqrt(llr_1_variance) * (R)(std::pow(llr_1_noninf_count, (R)-1.0 / (R)3.0)));

		if (bin_width > (R)0)
		{
			bin_offset = (int)std::floor(std::min(llr_0_min, llr_1_min) / bin_width) -1;

			auto tmp = std::max(llr_0_max, llr_1_max) / bin_width - (R)bin_offset + (R)1;
			bin_count = (unsigned)std::ceil(tmp);

			if ((R)bin_count == tmp)
				bin_count++;
		}
		else
		{
			bin_offset = -1;
			bin_count  = 3;
		}

		lots_of_bins = true;
	}
	else
	{
		lots_of_bins = false;
		bin_count    = 4;
	}

	std::vector<std::vector<unsigned>> hist(2, std::vector<unsigned>(bin_count, 0));
	std::vector<std::vector<R       >> pdf (2, std::vector<R       >(bin_count, 0));

	for (unsigned i = 0; i < size; i++)
	{
		const unsigned hist_idx = ref[i] ? 1 : 0;

		if(llr[i] == -inf)
			hist[hist_idx].front()++;

		else if (llr[i] == inf)
			hist[hist_idx].back()++;

		else if (!lots_of_bins)
			hist[hist_idx][hist_idx + 1]++;

		else if (bin_width > (R)0.0)
			hist[hist_idx][(int)(std::floor(llr[i] / bin_width) - bin_offset)]++;

		else
			hist[hist_idx][1]++;
	}

	for (unsigned i = 0; i < bin_count; i++)
	{
		pdf[0][i] = (R)hist[0][i] / (R)bit_0_count;
		pdf[1][i] = (R)hist[1][i] / (R)bit_1_count;
	}

	R MI = (R)0;
	for (auto b = 0; b < 2; b++)
		for (unsigned bin_ix = 0; bin_ix < bin_count; bin_ix++)
			if (pdf[b][bin_ix] > (R)0)
				MI += (R)0.5 * pdf[b][bin_ix] * std::log2((R)2.0 * pdf[b][bin_ix] / (pdf[0][bin_ix] + pdf[1][bin_ix]));

	return MI;
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template Q_32 aff3ct::tools::mutual_info_histo<B_32, Q_32>(const B_32*, const Q_32*, const unsigned);
template Q_64 aff3ct::tools::mutual_info_histo<B_64, Q_64>(const B_64*, const Q_64*, const unsigned);
#else
template Q aff3ct::tools::mutual_info_histo<B, Q>(const B*, const Q*, const unsigned);
#endif

#ifdef AFF3CT_MULTI_PREC
template Q_32 aff3ct::tools::mutual_info_histo_seq<B_32, Q_32>(const B_32*, const Q_32*, const unsigned);
template Q_64 aff3ct::tools::mutual_info_histo_seq<B_64, Q_64>(const B_64*, const Q_64*, const unsigned);
#else
template Q aff3ct::tools::mutual_info_histo_seq<B, Q>(const B*, const Q*, const unsigned);
#endif
// ==================================================================================== explicit template instantiation
