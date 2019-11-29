/*!
 * \file
 * \brief Class tools::Histogram.
 */
#ifndef HISTOGRAM_HPP__
#define HISTOGRAM_HPP__

#include <fstream>
#include <cstddef>
#include <string>
#include <cmath>
#include <vector>
#include <map>

#include "Tools/Interface/Interface_reset.hpp"

namespace aff3ct
{
namespace tools
{

template <typename R>
class Histogram : public Interface_reset
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
	inline explicit Histogram(unsigned precision = 3);

	~Histogram() = default;

	inline Histogram<R>& operator=(const Histogram<R>& other);

	inline void add_values(const Histogram<R>& other);

	inline void add_values(const std::vector<R>& draw);

	inline void add_value(const R& d);

	inline void add_values(const R* draw, size_t size);

	/*
	 *	Add a value 'd' 'weight' times in the histogram
	 */
	inline void add_value(const R& d, size_t weight);


	inline void norm_sum_to_1 (bool               val); // to normalize the values to a cumulative integration to 1.
	inline void norm_minus_1  (bool               val); // only when norm_sum_to_1 == true, dump (1-x)
	inline void cumul_vals    (bool               val); // accumulate values rung after rung as for a CDF
	inline void cumul_borders (bool               val); // accumulate the values over the given range [hist_min, hist_max] on those border values
	inline void n_rung        (unsigned           val); // the number of values plotted on the abscissa in the asked range (if 0 then dump directly the values)
	inline void data_separator(const std::string& val); // tag used to separate x from y when dumping them

	inline bool               norm_sum_to_1 () const;
	inline bool               norm_minus_1  () const;
	inline bool               cumul_vals    () const;
	inline bool               cumul_borders () const;
	inline unsigned           n_rung        () const;
	inline const std::string& data_separator() const;

	/*
	 * call dump by setting the hist_min and hist_max values as, respectively, get_hist_min() and get_hist_max()
	 */
	inline int dump(std::ofstream& hist_file) const;

	/*
	 * @hist_file is the output stream where will be dump the histogram with one (x,y) couple per line
	 * @hist_min is the left border of the dump
	 * @hist_max is the right border of the dump
	 * @n_rung is the number of values plotted on the abscissa in the asked range (if 0 then dump directly the values)
	 * @return 1 when there is an error in given limits
	 */
	inline int dump(std::ofstream& hist_file, R hist_min, R hist_max) const;

	/*
	 * Reset histogram values but not the options.
	 */
	inline void reset();

	inline int calibrate_val(R val) const;

	inline R uncalibrate_val(int val) const;

	inline R get_hist_min() const;

	inline R get_hist_max() const;

	inline size_t get_n_values() const;

private:
	inline int dump_all_values(std::ofstream& hist_file, R hist_min, R hist_max) const;

	inline int dump_intervals(std::ofstream& hist_file, R hist_min, R hist_max) const;

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

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Tools/Algo/Histogram.hxx"
#endif

#endif // HISTOGRAM_H__