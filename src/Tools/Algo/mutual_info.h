#ifndef MUTUAL_INFO_H__
#define MUTUAL_INFO_H__

#include <cmath>
#include <limits>
#include <vector>

namespace aff3ct
{
namespace tools
{

template <typename B, typename R>
R _check_mutual_info_histo(const B* ref, const R* llr, const unsigned size)
{
	const R inf = std::numeric_limits<R>::infinity();

	// determine the min and max value for llrs / 0 and llrs / 1
	// compute the llr_sum  as the sum of the llr[i]
	// compute the llr_sum2 as the sum of the llr[i]^2
	// compute the llr_mean as llr_sum / llr_noninfinite_count
	// compute the llr_variance as sum of (llr[i] - mean)^2 / llr_noninfinite_count
	//                              = sum( llr_sum2 - 2 * mean * llr_sum + llr_noninfinite_count * mean^2 ) / llr_noninfinite_count
	//                              = sum( llr_sum2 - 2 * mean * llr_sum + llr_sum * mean ) / llr_noninfinite_count
	//                              = sum( llr_sum2 - 1 * mean * llr_sum) / llr_noninfinite_count

	unsigned bit_1_count = 0;
	unsigned llr_0_noninfinite_count = 0;
	unsigned llr_1_noninfinite_count = 0;

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
				llr_1_noninfinite_count++;

				llr_1_sum  += llr[i];
				llr_1_sum2 += llr[i] * llr[i];

				llr_1_min = std::min(llr[i], llr_1_min);
				llr_1_max = std::max(llr[i], llr_1_max);
			}
			else
			{
				llr_0_noninfinite_count++;

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

	if (llr_0_noninfinite_count && llr_1_noninfinite_count && llr_0_min <= llr_1_max && llr_1_min <= llr_0_max)
	{
		R llr_0_mean     = llr_0_sum / (R)llr_0_noninfinite_count;
		R llr_1_mean     = llr_1_sum / (R)llr_1_noninfinite_count;
		R llr_0_variance = (llr_0_sum2 - (R)1 * llr_0_mean * llr_0_sum) / (R)llr_0_noninfinite_count;
		R llr_1_variance = (llr_1_sum2 - (R)1 * llr_1_mean * llr_1_sum) / (R)llr_1_noninfinite_count;

		bin_width = (R)0.5 * ((R)3.49 * (R)std::sqrt(llr_0_variance) * (R)(std::pow(llr_0_noninfinite_count, (R)-1.0 / (R)3.0)) +
		                      (R)3.49 * (R)std::sqrt(llr_1_variance) * (R)(std::pow(llr_1_noninfinite_count, (R)-1.0 / (R)3.0)));

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

	std::vector<std::vector<unsigned>> histogram(2, std::vector<unsigned>(bin_count, 0));
	std::vector<std::vector<R       >> pdf      (2, std::vector<R       >(bin_count, 0));

	for (unsigned i = 0; i < size; i++)
	{
		const unsigned hist_idx = ref[i] ? 1 : 0;

		if(llr[i] == -inf)
			histogram[hist_idx].front()++;

		else if (llr[i] == inf)
			histogram[hist_idx].back()++;

		else if (!lots_of_bins)
			histogram[hist_idx][hist_idx + 1]++;

		else if (bin_width > (R)0.0)
			histogram[hist_idx][(int)(std::floor(llr[i] / bin_width) - bin_offset)]++;

		else
			histogram[hist_idx][1]++;
	}

	for (unsigned i = 0; i < bin_count; i++)
	{
		pdf[0][i] = (R)histogram[0][i] / (R)bit_0_count;
		pdf[1][i] = (R)histogram[1][i] / (R)bit_1_count;
	}

	R MI = (R)0;
	for (auto b = 0; b < 2; b++)
		for (unsigned bin_ix = 0; bin_ix < bin_count; bin_ix++)
			if (pdf[b][bin_ix] > (R)0)
				MI += (R)0.5 * pdf[b][bin_ix] * std::log2((R)2.0 * pdf[b][bin_ix] / (pdf[0][bin_ix] + pdf[1][bin_ix]));

	return MI;
}

}
}
#endif // MUTUAL_INFO_H__
