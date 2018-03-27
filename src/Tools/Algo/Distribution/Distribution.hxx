#ifndef DISTRIBUTION_HXX__
#define DISTRIBUTION_HXX__

#include <utility>
#include <random>
#include <sstream>
#include <algorithm>

#include "Tools/Exception/exception.hpp"
#include "Tools/Math/numerical_integration.h"
#include "Tools/Math/interpolation.h"


#include "Distribution.hpp"

namespace aff3ct
{
namespace tools
{

template <typename R>
Distribution<R>
::Distribution(const std::vector<R>& _x_data, const std::vector<R>& _y_data)
: pdf(_x_data.size())
{
	if (_x_data.size() != _y_data.size())
	{
		std::stringstream message;
		message << "'_x_data.size()' has to be equal to 0 '_y_data.size()' ('_x_data.size()'' = " << _x_data.size()
		        << " and '_y_data.size()' = " << _y_data.size() << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	for(unsigned i = 0; i < this->pdf.size(); i++)
	{
		this->pdf[i].x(_x_data[i]);
		this->pdf[i].y(_y_data[i]);
	}

	compute_cdf();
}

template <typename R>
Distribution<R>
::Distribution(const std::vector<Point<R>>& _pdf)
: pdf(_pdf)
{
	compute_cdf();
}

template <typename R>
Distribution<R>
::Distribution(const std::vector<std::pair<R,R>>& _pdf)
: pdf(_pdf.size())
{
	for (unsigned i = 0; i < this->pdf.size(); i++)
	{
		this->pdf[i].x(_pdf[i].first);
		this->pdf[i].y(_pdf[i].second);
	}

	compute_cdf();
}

template <typename R>
Distribution<R>
::~Distribution()
{
}

template <typename R>
R Distribution<R>
::get_min_x() const
{
	return this->cdf_x.front();
}

template <typename R>
R Distribution<R>
::get_max_x() const
{
	return this->cdf_x.back();
}

template <typename R>
void Distribution<R>
::compute_cdf()
{
	// first sort it in ascending order
	std::sort(this->pdf.begin(), this->pdf.end(), tools::x_cmp_lt<R>);

	// then normalize it with its integral value
	auto integ = tools::trapz_integral_seq(this->pdf.data(), this->pdf.size());
	std::for_each(this->pdf.begin(), this->pdf.end(), [integ](Point<R>& p){p.y(p.y()/integ);}); // divide all elements by 'integ'

	// interpolation on a bigger vector of the input pdf for better integration
	this->cdf.resize(10000);//(pdf.size() * 10);
	const auto min_x  = this->pdf.front().x();
	const auto max_x  = this->pdf.back().x();
	const auto step_x = (max_x - min_x) / (this->cdf.size() - 1);

	R x = min_x;
	for (unsigned i = 0; i < this->cdf.size(); i++, x += step_x)
		this->cdf[i].x(x);
	this->cdf.back().x(max_x); // force the value to have exactly the max instead of an approximation after the "+= step_x"

	linear_interpolation(this->pdf, this->cdf);

	// computing the cumulative distribution function for input pdf
	std::vector<R> cumul(this->cdf.size());
	cumtrapz_integral_seq(this->cdf.data(), cumul.data(), cumul.size());

	for (unsigned i = 0; i < this->cdf.size(); i++)
		this->cdf[i].y(cumul[i]);

	// keeping the parts of this->cdf not parallel to the X axis
	std::vector<unsigned> erase_index;
	for (unsigned i = 1; i < this->cdf.size(); i++) // keep the first index
		if (tools::comp_equal(this->cdf[i].y() - this->cdf[i-1].y(), (R)0)) // == 0 means parallel to the X axis
			erase_index.push_back(i);

	for (unsigned i = 0; i < erase_index.size(); i++)
		this->cdf.erase(this->cdf.begin() + erase_index[i] - i);


	// write cdf as x and y vectors
	this->cdf_x.resize(this->cdf.size());
	this->cdf_y.resize(this->cdf.size());
	for (unsigned i = 0; i < this->cdf.size(); i++)
	{
		this->cdf_x[i] = this->cdf[i].x();
		this->cdf_y[i] = this->cdf[i].y();
	}

	// write pdf as x and y vectors
	this->pdf_x.resize(this->pdf.size());
	this->pdf_y.resize(this->pdf.size());
	for (unsigned i = 0; i < this->pdf.size(); i++)
	{
		this->pdf_x[i] = this->pdf[i].x();
		this->pdf_y[i] = this->pdf[i].y();
	}
}


template <typename R>
const std::vector<Point<R>>& Distribution<R>
::get_cdf  () const
{
	return this->cdf;
}

template <typename R>
const std::vector<R>& Distribution<R>
::get_cdf_x() const
{
	return this->cdf_x;
}

template <typename R>
const std::vector<R>& Distribution<R>
::get_cdf_y() const
{
	return this->cdf_y;
}

template <typename R>
const std::vector<Point<R>>& Distribution<R>
::get_pdf  () const
{
	return this->pdf;
}

template <typename R>
const std::vector<R>& Distribution<R>
::get_pdf_x() const
{
	return this->pdf_x;
}

template <typename R>
const std::vector<R>& Distribution<R>
::get_pdf_y() const
{
	return this->pdf_y;
}



}
}

#endif /* DISTRIBUTION_HXX__ */
