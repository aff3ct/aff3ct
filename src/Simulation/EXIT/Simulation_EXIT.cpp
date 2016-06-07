#include <algorithm>

#include "Simulation_EXIT.hpp"

template <typename B, typename Q>
double Simulation_EXIT<B,Q>
::measure_mutual_info_avg(const mipp::vector<Q>& llrs, const mipp::vector<B>& bits)
{
	double I_A = 0;
	double symb;
	int size = llrs.size();
	for (int j = 0; j < size; j++)
	{
		symb = -2 * (double)bits[j] +1;
		I_A += (1 - log2(1 + std::exp(-symb * (double)llrs[j])));
	}

	return(I_A);
}

template <typename B, typename Q>
double Simulation_EXIT<B,Q>
::measure_mutual_info_histo(const mipp::vector<Q>& llrs, const mipp::vector<B>& bits, const int N)
{
	const double Inf = 10000000;

	int vec_length = (int)bits.size();

	bool lots_of_bins;
	int bin_count, bin_offset = 0, bit_0_count, bit_1_count;
	int llr_0_noninfinite_count = 0;
	int llr_1_noninfinite_count = 0;

	double tmp;
	double I_E = 0.0;
	double bin_width = 0.0f;
	double llr_0_variance = 0.0;
	double llr_1_variance = 0.0;
	double llr_0_max, llr_0_min, llr_1_max, llr_1_min;
	double llr_0_mean = 0.0;
	double llr_1_mean = 0.0;

	bit_1_count = 0;
	for (int i = 0; i < (int)bits.size(); i++)
		bit_1_count += (int)bits[i];

	bit_0_count = vec_length - bit_1_count;
	if (bit_0_count == 0 || bit_1_count == 0)
		I_E = 0;
	else
	{
		// determine the min and max value for llrs / 0 and llrs / 1
		llr_0_max = llr_1_max = -Inf;
		llr_0_min = llr_1_min = +Inf;

		for (int i = 0; i < vec_length; i++)
		{
			if ((int)bits[i] == 0)
			{
				llr_0_noninfinite_count++;
                llr_0_min = std::min((double)llrs[i], llr_0_min);
                llr_0_max = std::max((double)llrs[i], llr_0_max);
			}
			else
			{
				llr_1_noninfinite_count++;
                llr_1_min = std::min((double)llrs[i], llr_1_min);
                llr_1_max = std::max((double)llrs[i], llr_1_max);
			}
		}
		if (llr_0_noninfinite_count > 0 && llr_1_noninfinite_count > 0 && 
		    llr_0_min <= llr_1_max && llr_1_min <= llr_0_max)
		{
			for (int i = 0; i < vec_length; i++)
			{
				if ((double)llrs[i] != -Inf && (double)llrs[i] != Inf)
				{
					if ((int)bits[i] == 0)
						llr_0_mean = llr_0_mean + (double)llrs[i];
					else
						llr_1_mean = llr_1_mean + (double)llrs[i];
				}
			}
			llr_0_mean = llr_0_mean / llr_0_noninfinite_count;
			llr_1_mean = llr_1_mean / llr_1_noninfinite_count;

			for (int i = 0; i < vec_length; i++)
			{
				if ((double)llrs[i] != -Inf && (double)llrs[i] != Inf)
				{
					if ((int)bits[i] == 0)
						llr_0_variance = llr_0_variance + std::pow(((double)llrs[i] - llr_0_mean), 2);
					else
						llr_1_variance = llr_1_variance + std::pow(((double)llrs[i] - llr_1_mean), 2);
				}
			}
			llr_0_variance = llr_0_variance / llr_0_noninfinite_count;
			llr_1_variance = llr_1_variance / llr_1_noninfinite_count;

			bin_width = 0.5 * (3.49 * std::sqrt(llr_0_variance) * (std::pow(llr_0_noninfinite_count, (-1.0 / 3.0))) + 
			            3.49 * std::sqrt(llr_1_variance) * (std::pow(llr_1_noninfinite_count, (-1.0 / 3.0))));
			if (bin_width > 0.0)
			{
				bin_offset = std::floor(std::min(llr_0_min, llr_1_min) / bin_width) - 1;
				tmp = std::max(llr_0_max, llr_1_max) / bin_width - bin_offset + 1;
				bin_count = std::ceil(tmp);
				if (bin_count == tmp)
					bin_count = bin_count + 1;
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

		std::vector<std::vector<double> > histogram(2, std::vector<double>(bin_count));
		std::vector<std::vector<double> > pdf(2, std::vector<double>(bin_count));
		for (int i = 0; i < vec_length; i++)
		{
			if ((double)llrs[i] == -Inf)
				histogram[(int)bits[i]][0] += 1;
			else if ((double)llrs[i] == Inf)
				histogram[(int)bits[i]][bin_count - 1] += 1;
			else
			{
				if (lots_of_bins)
				{
					if (bin_width > 0.0)
						histogram[(int)bits[i]][std::floor((double)llrs[i] / bin_width) - bin_offset] += 1;
					else
						histogram[(int)bits[i]][1] += 1;
				}
				else
					histogram[(int)bits[i]][ (int)bits[i] + 1] += 1;
			}
		}

		for (int i = 0; i < bin_count; i++)
		{
			pdf[0][i] = histogram[0][i] / bit_0_count;
			pdf[1][i] = histogram[1][i] / bit_1_count;
		}

		I_E = 0.0;
		for (int b = 0; b < 2; b++)
			for (int bin_ix = 0; bin_ix < bin_count; bin_ix++)
				if (pdf[b][bin_ix] > 0.0)
					I_E += 0.5 * pdf[b][bin_ix] * std::log2(2.0 * pdf[b][bin_ix] / (pdf[0][bin_ix] + pdf[1][bin_ix]));
	}

	return I_E;
}

// ==================================================================================== explicit template instantiation 
#include "../../Tools/types.h"
#ifdef MULTI_PREC
template class Simulation_EXIT<B_8,Q_8>;
template class Simulation_EXIT<B_16,Q_16>;
template class Simulation_EXIT<B_32,Q_32>;
template class Simulation_EXIT<B_64,Q_64>;
#else
template class Simulation_EXIT<B,Q>;
#endif
// ==================================================================================== explicit template instantiation