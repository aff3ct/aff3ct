#ifndef HISTOGRAM_H__
#define HISTOGRAM_H__

#include <cmath>
#include <fstream>
#include <vector>
#include <map>
#include <numeric>

#include "Tools/Exception/exception.hpp"

namespace aff3ct
{
namespace tools
{

/*
 * @precision the number of decimal took into account in the given values
 */
template <typename R, unsigned precision = 3>
class Histogram
{
protected:
	const R  stock_norm = (R)std::pow((unsigned)10,precision);
	const R _stock_norm = (R)1/stock_norm;
	size_t n_values = 0;
	std::map<int, size_t> hist;

public:
	inline void add_values(const Histogram<R, precision>& other)
	{
		for (auto& o : other.hist)
		{
			auto it = hist.find(o.first);
			if (it == hist.end())
				hist[o.first] = o.second;
			else
				it->second += o.second;
		}

		n_values += other.n_values;
	}

	inline void add_values(const std::vector<R>& draw)
	{
		add_values(draw.data(), draw.size());
	}

	inline void add_value(const R& d)
	{
		add_values(&d, 1);
	}

	inline void add_values(const R* draw, size_t size)
	{
		for (size_t i = 0; i < size; i++)
		{
			auto x = calibrate_val(draw[i]);

			auto it = hist.find(x);
			if (it == hist.end())
				hist[x] = 1;
			else
				it->second++;
		}

		n_values += size;
	}


	/*
	 * @hist_file is the output stream where will be dump the histogram with one (x,y) couple per line
	 * @hist_min is the left border of the dump
	 * @hist_max is the right border of the dump
	 * @n_intervals the number of intervals needed to dump the asked range (if -1 then dump directly the values)
	 * @norm_sum_to_1 is to normalize the values to a cumulative integration to 1.
	 * @cumul_borders is to accumulate the values over the given range [hist_min, hist_max] on those border values
	 * @data_separator is the tag used to separate x from y when dumping them
	 * @return 1 when given limits are over ranges
	 */
	int dump(std::ofstream& hist_file, R hist_min = get_hist_min(), R hist_max = get_hist_max(), int n_intervals = 100, bool norm_sum_to_1 = false,
	          bool cumul_borders = false, const std::string& data_separator = "; ") const
	{
		if (hist_min >= hist_max)
			return 1;

		// find the min max positions in the saved values
		auto it_min = hist.lower_bound(calibrate_val(hist_min));
		auto it_max = hist.upper_bound(calibrate_val(hist_max));

		if (it_min == hist.end() || it_max == hist.begin()) // above limits bounds
			return 1;

		hist_min = uncalibrate_val(it_min->first);

		it_max --;
		hist_max = uncalibrate_val(it_max->first);
		it_max ++;

		R dump_step = (R)n_intervals / (hist_max - hist_min);
		R _dump_step = (R)1/dump_step;

		std::vector<size_t> dump_hist(n_intervals + 1, 0);

		for (auto it = it_min; it != it_max; it++)
		{
			auto x = (unsigned)round((uncalibrate_val(it->first) - hist_min) * dump_step);
			dump_hist[x] += it->second;
		}

		if (cumul_borders)
		{
			auto sum = dump_hist.front();
			for (auto it = hist.begin(); it != it_min; it++)
				sum += it->second;
			dump_hist.front() = sum;

			sum = dump_hist.back();
			for (auto it = it_max; it != hist.end(); it++)
				sum += it->second;
			dump_hist.back() = sum;
		}

		R factor = 1;
		if (norm_sum_to_1)
			factor = (R)1 /(R)n_values;

		for (unsigned i = 0; i < dump_hist.size(); ++i)
			hist_file << ((R)i * _dump_step + hist_min) << data_separator << std::scientific << dump_hist[i] * factor
			          << std::endl;

		return 0;
	}

	inline void reset()
	{
		n_values = 0;
		hist.clear();
	}

	inline int calibrate_val(R val) const
	{
		return (int)round(val * stock_norm);
	}

	inline R uncalibrate_val(int val) const
	{
		return (R)val * _stock_norm;
	}

	inline R get_hist_min() const
	{
		return uncalibrate_val(hist.begin()->first);
	}

	inline R get_hist_max() const
	{
		return uncalibrate_val(hist.rbegin()->first);
	}

	inline size_t get_n_values() const
	{
		return n_values;
	}
};

}
}


#endif // HISTOGRAM_H__