#include <cmath>
#include <string>
#include <vector>
#include <stdexcept>

#include "Monitor_EXIT.hpp"

using namespace aff3ct::module;

template <typename B, typename R>
Monitor_EXIT<B,R>
::Monitor_EXIT(const int size, const unsigned n_trials, const int n_frames, const std::string name)
: Monitor(size, n_frames, name.c_str()),
  n_trials(n_trials),
  I_A_sum((R)0),
  n_analyzed_frames(0)
{
	auto &p = this->create_task("check_mutual_info");
	auto &ps_bits   = this->template create_socket_in<B>(p, "bits",   this->size * this->n_frames);
	auto &ps_llrs_a = this->template create_socket_in<R>(p, "llrs_a", this->size * this->n_frames);
	auto &ps_llrs_e = this->template create_socket_in<R>(p, "llrs_e", this->size * this->n_frames);
	this->create_codelet(p, [this, &ps_bits, &ps_llrs_a, &ps_llrs_e]() -> int
	{
		this->check_mutual_info(static_cast<B*>(ps_bits  .get_dataptr()),
		                        static_cast<R*>(ps_llrs_a.get_dataptr()),
		                        static_cast<R*>(ps_llrs_e.get_dataptr()));

		return 0;
	});
}

template <typename B, typename R>
void Monitor_EXIT<B,R>
::check_mutual_info(const B *bits, const R *llrs_a, const R *llrs_e)
{
	for (auto f = 0; f < this->n_frames; f++)
		this->_check_mutual_info_avg(bits   + f * this->size,
		                             llrs_a + f * this->size,
		                             f);

	bits_buff  .insert(bits_buff  .end(), bits,   bits   + this->size);
	llrs_e_buff.insert(llrs_e_buff.end(), llrs_e, llrs_e + this->size);

	n_analyzed_frames += this->n_frames;

	for (auto c : this->callbacks_measure)
		c();
}

template <typename B, typename R>
void Monitor_EXIT<B,R>
::_check_mutual_info_avg(const B *bits, const R *llrs_a, const int frame_id)
{
	for (int j = 0; j < this->size; j++)
	{
		double symb = -2.0 * (double)bits[j] +1.0;
		I_A_sum += (1.0 - std::log2(1.0 + std::exp(-symb * (double)llrs_a[j])));
	}
}

template <typename B, typename R>
R Monitor_EXIT<B,R>
::_check_mutual_info_histo() const
{
	unsigned size = this->bits_buff.size();
	unsigned bit_1_count = 0;
	for (unsigned i = 0; i < size; i++)
		bit_1_count += (unsigned)bits_buff[i];

	unsigned bit_0_count = (unsigned)size - bit_1_count;
	if (bit_0_count == 0 || bit_1_count == 0)
	{
		return (R)0;
	}
	else
	{
		const R inf = std::numeric_limits<R>::infinity();

		bool     lots_of_bins;
		unsigned bin_count;
		int      bin_offset = 0;
		R        bin_width  = (R)0;

		// determine the min and max value for llrs / 0 and llrs / 1
		R llr_0_max = -inf, llr_1_max = -inf;
		R llr_0_min = +inf, llr_1_min = +inf;

		unsigned llr_0_noninfinite_count = 0;
		unsigned llr_1_noninfinite_count = 0;

		for (unsigned i = 0; i < size; i++)
		{
			if (!std::isinf(llrs_e_buff[i]))
			{
				if ((int)bits_buff[i] == 0)
				{
					llr_0_noninfinite_count++;
					llr_0_min = std::min(llrs_e_buff[i], llr_0_min);
					llr_0_max = std::max(llrs_e_buff[i], llr_0_max);
				}
				else
				{
					llr_1_noninfinite_count++;
					llr_1_min = std::min(llrs_e_buff[i], llr_1_min);
					llr_1_max = std::max(llrs_e_buff[i], llr_1_max);
				}
			}
		}
		if (llr_0_noninfinite_count && llr_1_noninfinite_count && llr_0_min <= llr_1_max && llr_1_min <= llr_0_max)
		{
			R llr_0_mean = (R)0;
			R llr_1_mean = (R)0;
			for (unsigned i = 0; i < size; i++)
			{
				if (!std::isinf(llrs_e_buff[i]))
				{
					if ((int)bits_buff[i] == 0) llr_0_mean += llrs_e_buff[i];
					else                        llr_1_mean += llrs_e_buff[i];
				}
			}
			llr_0_mean /= llr_0_noninfinite_count;
			llr_1_mean /= llr_1_noninfinite_count;

			R llr_0_variance = (R)0;
			R llr_1_variance = (R)0;
			for (unsigned i = 0; i < size; i++)
			{
				if (!std::isinf(llrs_e_buff[i]))
				{
					if ((int)bits_buff[i] == 0) llr_0_variance += std::pow((llrs_e_buff[i] - llr_0_mean), 2);
					else                        llr_1_variance += std::pow((llrs_e_buff[i] - llr_1_mean), 2);
				}
			}
			llr_0_variance /= llr_0_noninfinite_count;
			llr_1_variance /= llr_1_noninfinite_count;

			bin_width = (R)0.5 * ((R)3.49 * std::sqrt(llr_0_variance) * (std::pow(llr_0_noninfinite_count, (R)-1.0 / (R)3.0)) +
			                      (R)3.49 * std::sqrt(llr_1_variance) * (std::pow(llr_1_noninfinite_count, (R)-1.0 / (R)3.0)));
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

		std::vector<std::vector<unsigned>> histogram(2, std::vector<unsigned>(bin_count));
		std::vector<std::vector<R       >> pdf      (2, std::vector<R       >(bin_count));
		for (unsigned i = 0; i < size; i++)
		{
			if      (llrs_e_buff[i] == -inf) histogram[(int)bits_buff[i]][0           ]++;
			else if (llrs_e_buff[i] ==  inf) histogram[(int)bits_buff[i]][bin_count -1]++;
			else
			{
				if (lots_of_bins)
				{
					if (bin_width > 0.0)
						histogram[(int)bits_buff[i]][(int)(std::floor(llrs_e_buff[i] / bin_width) - bin_offset)]++;
					else
						histogram[(int)bits_buff[i]][1]++;
				}
				else
					histogram[(int)bits_buff[i]][(int)bits_buff[i] +1]++;
			}
		}

		for (unsigned i = 0; i < bin_count; i++)
		{
			pdf[0][i] = (R)histogram[0][i] / (R)bit_0_count;
			pdf[1][i] = (R)histogram[1][i] / (R)bit_1_count;
		}

		R I_E = (R)0;
		for (auto b = 0; b < 2; b++)
			for (unsigned bin_ix = 0; bin_ix < bin_count; bin_ix++)
				if (pdf[b][bin_ix] > (R)0)
					I_E += (R)0.5 * pdf[b][bin_ix] * std::log2((R)2.0 * pdf[b][bin_ix] / (pdf[0][bin_ix] + pdf[1][bin_ix]));

		return I_E;
	}
}

template <typename B, typename R>
unsigned Monitor_EXIT<B,R>
::get_n_trials() const
{
	return n_trials;
}

template <typename B, typename R>
bool Monitor_EXIT<B,R>
::n_trials_achieved() const
{
	return (get_n_analyzed_fra() >= get_n_trials()) || Monitor::interrupt;
}

template <typename B, typename R>
R Monitor_EXIT<B,R>
::get_I_A() const
{
	return this->I_A_sum / (R)(this->size * this->n_analyzed_frames);
}

template <typename B, typename R>
R Monitor_EXIT<B,R>
::get_I_E() const
{
	return this->_check_mutual_info_histo();
}

template <typename B, typename R>
unsigned long long Monitor_EXIT<B,R>
::get_n_analyzed_fra() const
{
	return n_analyzed_frames;
}

template <typename B, typename R>
void Monitor_EXIT<B,R>
::add_handler_measure(std::function<void(void)> callback)
{
	this->callbacks_measure.push_back(callback);
}

template <typename B, typename R>
void Monitor_EXIT<B,R>
::reset()
{
	Monitor::reset();

	this->n_analyzed_frames = 0;
	this->I_A_sum = (R)0;
	this->bits_buff.clear();
	this->llrs_e_buff.clear();
}

template <typename B, typename R>
void Monitor_EXIT<B,R>
::clear_callbacks()
{
	this->callbacks_measure.clear();
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Monitor_EXIT<B_8 ,R_8 >;
template class aff3ct::module::Monitor_EXIT<B_16,R_16>;
template class aff3ct::module::Monitor_EXIT<B_32,R_32>;
template class aff3ct::module::Monitor_EXIT<B_64,R_64>;
#else
template class aff3ct::module::Monitor_EXIT<B,R>;
#endif
// ==================================================================================== explicit template instantiation
