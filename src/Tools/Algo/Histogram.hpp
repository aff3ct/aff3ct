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
	 *	Add a value 'd' 'weight' times in the histogram
	 */
	inline void add_value(const R& d, size_t weight)
	{
		auto x = calibrate_val(d);

		auto it = hist.find(x);
		if (it == hist.end())
			hist[x] = weight;
		else
			it->second += weight;

		n_values += weight;
	}


	inline void norm_sum_to_1 (bool               val) { _norm_sum_to_1  = val;} // to normalize the values to a cumulative integration to 1.
	inline void norm_minus_1  (bool               val) { _norm_minus_1   = val;} // only when norm_sum_to_1 == true, dump (1-x)
	inline void cumul_vals    (bool               val) { _cumul_vals     = val;} // accumulate values rung after rung as for a CDF
	inline void cumul_borders (bool               val) { _cumul_borders  = val;} // accumulate the values over the given range [hist_min, hist_max] on those border values
	inline void n_rung        (unsigned           val) { _n_rung         = val;} // the number of values plotted on the abscissa in the asked range (if 0 then dump directly the values)
	inline void data_separator(const std::string& val) { _data_separator = val;} // tag used to separate x from y when dumping them

	inline bool               norm_sum_to_1 () const { return _norm_sum_to_1; }
	inline bool               norm_minus_1  () const { return _norm_minus_1;  }
	inline bool               cumul_vals    () const { return _cumul_vals;    }
	inline bool               cumul_borders () const { return _cumul_borders; }
	inline unsigned           n_rung        () const { return _n_rung;        }
	inline const std::string& data_separator() const { return _data_separator;}

	/*
	 * call dump by setting the hist_min and hist_max values as, respectively, get_hist_min() and get_hist_max()
	 */
	int dump(std::ofstream& hist_file) const
	{
		return dump(hist_file, get_hist_min(), get_hist_max());
	}

	/*
	 * @hist_file is the output stream where will be dump the histogram with one (x,y) couple per line
	 * @hist_min is the left border of the dump
	 * @hist_max is the right border of the dump
	 * @n_rung is the number of values plotted on the abscissa in the asked range (if 0 then dump directly the values)
	 * @return 1 when there is an error in given limits
	 */
	int dump(std::ofstream& hist_file, R hist_min, R hist_max) const
	{
		if (hist_min >= hist_max)
			return 1;

		if (n_rung() == 0)
			return dump_all_values(hist_file, hist_min, hist_max);
		else
			return dump_intervals (hist_file, hist_min, hist_max);
	}

	/*
	 * Reset histogram values but not the options.
	 */
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
	int dump_all_values(std::ofstream& hist_file, R hist_min, R hist_max) const
	{
		R factor = 1;
		if (norm_sum_to_1())
		{
			factor = (R)1 /(R)get_n_values();
		}

		size_t cumul = 0;
		bool dumped_hist_min = false;

		auto cal_hist_min = calibrate_val(hist_min);
		auto cal_hist_max = calibrate_val(hist_max);
		R value;

		for (auto it = hist.begin(); it != hist.end(); it++)
		{
			if ((it->first > cal_hist_min) && (cal_hist_max > it->first))
			{
				value = (R)cumul * factor;
				if (norm_sum_to_1() && norm_minus_1())
					value = (R)1 - value;

				if (!dumped_hist_min)
				{ // then end of the cumul border on the left
					hist_file << hist_min << data_separator() << std::scientific << value << std::endl;
					dumped_hist_min = true;
				}

				if (cumul_vals())
					cumul += it->second;
				else
					cumul = it->second;

				value = (R)cumul * factor;
				if (norm_sum_to_1() && norm_minus_1())
					value = (R)1 - value;

				hist_file << uncalibrate_val(it->first) << data_separator() << std::scientific << value << std::endl;
			}
			else if (cumul_borders() || it->first == cal_hist_min || it->first == cal_hist_max)
			{
				cumul += it->second;
			}
		}

		if (cumul != 0) // then end of the cumul border on the right
		{
			value = (R)cumul * factor;
			if (norm_sum_to_1() && norm_minus_1())
				value = (R)1 - value;
			hist_file << hist_max << data_separator() << std::scientific << value << std::endl;
		}

		return 0;
	}

	int dump_intervals(std::ofstream& hist_file, R hist_min, R hist_max) const
	{
		R factor = 1;
		if (norm_sum_to_1())
		{
			factor = (R)1 /(R)get_n_values();
		}

		R dump_step = (R)n_rung() / (hist_max - hist_min);
		std::vector<size_t> dump_hist(n_rung() + 1, 0);

		for (auto it = hist.begin(); it != hist.end(); it++)
		{
			auto x = (int)round((uncalibrate_val(it->first) - hist_min) * dump_step);
			if (x >= 0 && x <= (int)n_rung())
			{
				dump_hist[x] += it->second;
			}
			else if (cumul_borders())
			{
				if (x < 0)
					dump_hist.front() += it->second;
				else // x > n_intervals
					dump_hist.back() += it->second;
			}
		}

		R _dump_step = (R)1/dump_step;
		size_t cumul = 0;

		for (unsigned i = 0; i <= n_rung(); ++i)
		{
			if (cumul_vals())
				cumul += dump_hist[i];
			else
				cumul = dump_hist[i];

			auto value = (R)cumul * factor;
			if (norm_sum_to_1() && norm_minus_1())
				value = (R)1 - value;

			hist_file << ((R) i * _dump_step + hist_min) << data_separator() << std::scientific << value << std::endl;
		}
		return 0;
	}

private:
	bool        _norm_sum_to_1  = false;
	bool        _norm_minus_1   = false;
	bool        _cumul_vals     = false;
	bool        _cumul_borders  = false;
	unsigned    _n_rung         = 0;
	std::string _data_separator = "; ";

};

}
}


#endif // HISTOGRAM_H__