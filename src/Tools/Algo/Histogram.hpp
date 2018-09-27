#ifndef HISTOGRAM_H__
#define HISTOGRAM_H__

#include <cmath>
#include <fstream>
#include <vector>
#include <sstream>
#include <map>
#include <numeric>

#include "Tools/Exception/exception.hpp"

namespace aff3ct
{
namespace tools
{

template <typename R>
class Histogram
{
protected:
	size_t n_values = 0;
	std::map<int, size_t> hist;

	const unsigned precision;
	const R  stock_norm = (R)std::pow((unsigned)10,precision);
	const R _stock_norm = (R)1/stock_norm;

public:
	/*
	 * @precision the number of decimal took into account in the given values
	 */
	explicit Histogram(unsigned precision = 3) : precision(precision) {}

	~Histogram() = default;

	Histogram<R>& operator=(const Histogram<R>& other)
	{
		if (this->precision != other.precision)
		{
			std::stringstream message;
			message << "Histograms do not have the same precision ('this->precision' = " << this->precision
			        << ", other.precision = " << other.precision << ")." << std::endl;
			throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
		}

		n_values = other.n_values;
		hist     = other.hist;

		return *this;
	}

	inline void add_values(const Histogram<R>& other)
	{
		if (this->precision != other.precision)
		{
			std::stringstream message;
			message << "Histograms do not have the same precision ('this->precision' = " << this->precision
			        << ", other.precision = " << other.precision << ")." << std::endl;
			throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
		}

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
	 * call dump by setting the hist_min and hist_max values as, respectively, get_hist_min() and get_hist_max()
	 */
	int dump(std::ofstream& hist_file, unsigned n_rung = 0, bool norm_sum_to_1 = false, bool cumul_vals = false,
	         bool cumul_borders = false, const std::string& data_separator = "; ") const
	{
		return dump(hist_file, get_hist_min(), get_hist_max(), n_rung, norm_sum_to_1, cumul_vals, cumul_borders, data_separator);
	}

	/*
	 * @hist_file is the output stream where will be dump the histogram with one (x,y) couple per line
	 * @hist_min is the left border of the dump
	 * @hist_max is the right border of the dump
	 * @n_rung is the number of values plotted on the abscissa in the asked range (if 0 then dump directly the values)
	 * @norm_sum_to_1 is to normalize the values to a cumulative integration to 1.
+	 * @cumul_vals option accumulates values rung after rung as for a CDF
	 * @cumul_borders is to accumulate the values over the given range [hist_min, hist_max] on those border values
	 * @data_separator is the tag used to separate x from y when dumping them
	 * @return 1 when there is an error in given limits
	 */
	int dump(std::ofstream& hist_file, R hist_min, R hist_max, unsigned n_rung = 0, bool norm_sum_to_1 = false,
	         bool cumul_vals = false, bool cumul_borders = false, const std::string& data_separator = "; ") const
	{
		if (hist_min >= hist_max)
			return 1;

		if (n_rung == 0)
			return dump_all_values(hist_file, hist_min, hist_max,         norm_sum_to_1, cumul_vals, cumul_borders, data_separator);
		else
			return dump_intervals (hist_file, hist_min, hist_max, n_rung, norm_sum_to_1, cumul_vals, cumul_borders, data_separator);
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
		if (hist.empty())
		{
			std::stringstream message;
			message << "The histogram is empty." << std::endl;
			throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
		}

		return uncalibrate_val(hist.begin()->first);
	}

	inline R get_hist_max() const
	{
		if (hist.empty())
		{
			std::stringstream message;
			message << "The histogram is empty." << std::endl;
			throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
		}

		return uncalibrate_val(hist.rbegin()->first);
	}

	inline size_t get_n_values() const
	{
		return n_values;
	}

private:
	int dump_all_values(std::ofstream& hist_file, R hist_min, R hist_max, bool norm_sum_to_1 = false,
	                    bool cumul_vals = false, bool cumul_borders = false, const std::string& data_separator = "; ") const
	{
		R factor = 1;
		if (norm_sum_to_1)
		{
			factor = (R)1 /(R)n_values;
		}

		size_t cumul = 0;
		bool dumped_hist_min = false;

		auto cal_hist_min = calibrate_val(hist_min);
		auto cal_hist_max = calibrate_val(hist_max);

		for (auto it = hist.begin(); it != hist.end(); it++)
		{
			if ((it->first > cal_hist_min) && (cal_hist_max > it->first))
			{
				if (!dumped_hist_min)
				{ // then end of the cumul border on the left
					hist_file << hist_min << data_separator << std::scientific << (R)cumul * factor << std::endl;
					dumped_hist_min = true;
				}

				if (cumul_vals)
					cumul += it->second;
				else
					cumul = it->second;

				hist_file << uncalibrate_val(it->first) << data_separator << std::scientific << (R)cumul * factor << std::endl;
			}
			else if (cumul_borders || it->first == cal_hist_min || it->first == cal_hist_max)
			{
				cumul += it->second;
			}
		}

		if (cumul != 0) // then end of the cumul border on the right
			hist_file << hist_max << data_separator << std::scientific << (R)cumul * factor << std::endl;

		return 0;
	}

	int dump_intervals(std::ofstream& hist_file, R hist_min, R hist_max, unsigned n_rung, bool norm_sum_to_1 = false,
	                   bool cumul_vals = false, bool cumul_borders = false, const std::string& data_separator = "; ") const
	{
		R factor = 1;
		if (norm_sum_to_1)
		{
			factor = (R)1 /(R)n_values;
		}

		R dump_step = (R)n_rung / (hist_max - hist_min);
		std::vector<size_t> dump_hist(n_rung + 1, 0);

		for (auto it = hist.begin(); it != hist.end(); it++)
		{
			auto x = (int)round((uncalibrate_val(it->first) - hist_min) * dump_step);
			if (x >= 0 && x <= (int)n_rung)
			{
				dump_hist[x] += it->second;
			}
			else if (cumul_borders)
			{
				if (x < 0)
					dump_hist.front() += it->second;
				else // x > n_intervals
					dump_hist.back() += it->second;
			}
		}

		R _dump_step = (R)1/dump_step;
		size_t cumul = 0;

		for (unsigned i = 0; i <= n_rung; ++i)
		{
			if (cumul_vals)
				cumul += dump_hist[i];
			else
				cumul = dump_hist[i];

			hist_file << ((R) i * _dump_step + hist_min) << data_separator << std::scientific
			          << (R) cumul * factor << std::endl;
		}
		return 0;
	}
};

}
}


#endif // HISTOGRAM_H__