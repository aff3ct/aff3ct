#include <sstream>
#include <ios>

#include "Tools/Exception/exception.hpp"
#include "Tools/Algo/Histogram.hpp"

namespace aff3ct
{
namespace tools
{

template <typename R>
Histogram<R>
::Histogram(unsigned precision)
: precision(precision)
{
}

template <typename R>
Histogram<R>& Histogram<R>
::operator=(const Histogram<R>& other)
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

template <typename R>
void Histogram<R>
::add_values(const Histogram<R>& other)
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

template <typename R>
void Histogram<R>
::add_values(const std::vector<R>& draw)
{
	add_values(draw.data(), draw.size());
}

template <typename R>
void Histogram<R>
::add_value(const R& d)
{
	add_values(&d, 1);
}

template <typename R>
void Histogram<R>
::add_values(const R* draw, size_t size)
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

template <typename R>
void Histogram<R>
::add_value(const R& d, size_t weight)
{
	auto x = calibrate_val(d);

	auto it = hist.find(x);
	if (it == hist.end())
		hist[x] = weight;
	else
		it->second += weight;

	n_values += weight;
}

template <typename R>
void Histogram<R>
::norm_sum_to_1(bool val)
{
	_norm_sum_to_1 = val;
}

template <typename R>
void Histogram<R>
::norm_minus_1(bool val)
{
	_norm_minus_1 = val;
}

template <typename R>
void Histogram<R>
::cumul_vals(bool val)
{
	_cumul_vals = val;
}

template <typename R>
void Histogram<R>
::cumul_borders (bool val)
{
	_cumul_borders = val;
}

template <typename R>
void Histogram<R>
::n_rung(unsigned val)
{
	_n_rung = val;
}

template <typename R>
void Histogram<R>
::data_separator(const std::string& val)
{
	_data_separator = val;
}

template <typename R>
bool Histogram<R>
::norm_sum_to_1() const
{
	return _norm_sum_to_1;
}

template <typename R>
bool Histogram<R>
::norm_minus_1() const
{
	return _norm_minus_1;
}

template <typename R>
bool Histogram<R>
::cumul_vals() const
{
	return _cumul_vals;
}

template <typename R>
bool Histogram<R>
::cumul_borders() const
{
	return _cumul_borders;
}

template <typename R>
unsigned Histogram<R>
::n_rung() const
{
	return _n_rung;
}

template <typename R>
const std::string& Histogram<R>
::data_separator() const
{
	return _data_separator;
}

template <typename R>
int Histogram<R>
::dump(std::ofstream& hist_file) const
{
	return dump(hist_file, get_hist_min(), get_hist_max());
}

template <typename R>
int Histogram<R>
::dump(std::ofstream& hist_file, R hist_min, R hist_max) const
{
	if (hist_min >= hist_max)
		return 1;

	if (n_rung() == 0)
		return dump_all_values(hist_file, hist_min, hist_max);
	else
		return dump_intervals (hist_file, hist_min, hist_max);
}

template <typename R>
void Histogram<R>
::reset()
{
	n_values = 0;
	hist.clear();
}

template <typename R>
int Histogram<R>
::calibrate_val(R val) const
{
	return (int)round(val * stock_norm);
}

template <typename R>
R Histogram<R>
::uncalibrate_val(int val) const
{
	return (R)val * _stock_norm;
}

template <typename R>
R Histogram<R>
::get_hist_min() const
{
	if (hist.empty())
	{
		std::stringstream message;
		message << "The histogram is empty." << std::endl;
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	return uncalibrate_val(hist.begin()->first);
}

template <typename R>
R Histogram<R>
::get_hist_max() const
{
	if (hist.empty())
	{
		std::stringstream message;
		message << "The histogram is empty." << std::endl;
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	return uncalibrate_val(hist.rbegin()->first);
}

template <typename R>
size_t Histogram<R>
::get_n_values() const
{
	return n_values;
}

template <typename R>
int Histogram<R>
::dump_all_values(std::ofstream& hist_file, R hist_min, R hist_max) const
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

template <typename R>
int Histogram<R>
::dump_intervals(std::ofstream& hist_file, R hist_min, R hist_max) const
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
}
}
