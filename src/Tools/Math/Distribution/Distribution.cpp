#include <utility>
#include <random>
#include <sstream>
#include <algorithm>
#include <vector>
#include <numeric>

#include "Tools/Exception/exception.hpp"
#include "Tools/Math/numerical_integration.h"
#include "Tools/Math/interpolation.h"

#include "Distribution.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

template <typename R>
Distribution<R>
::Distribution(const std::vector<R>& _x_data, const std::vector<R>& _y_data, Distribution_mode mode)
: pdf_x(_x_data), pdf_y(1)
{
	pdf_y.front() = _y_data;

	compute_cdf(mode);
}

template <typename R>
Distribution<R>
::Distribution(std::vector<R>&& _x_data, std::vector<R>&& _y_data, Distribution_mode mode)
: pdf_x(std::move(_x_data)), pdf_y(1)
{
	pdf_y.front() = std::move(_y_data);

	compute_cdf(mode);
}

template <typename R>
Distribution<R>
::Distribution(const std::vector<R>& _x_data, const std::vector<std::vector<R>>& _y_data, Distribution_mode mode)
: pdf_x(_x_data), pdf_y(_y_data)
{
	compute_cdf(mode);
}

template <typename R>
Distribution<R>
::Distribution(std::vector<R>&& _x_data, std::vector<std::vector<R>>&& _y_data, Distribution_mode mode)
: pdf_x(std::move(_x_data)), pdf_y(std::move(_y_data))
{
	compute_cdf(mode);
}

template <typename R>
Distribution<R>
::~Distribution()
{
}

template <typename R>
void Distribution<R>
::sort_pdf()
{
	for (unsigned i = 1; i < this->pdf_x.size(); i++)
		for (unsigned j = i; j > 0 && this->pdf_x[j] < this->pdf_x[j-1]; j--)
		{
			std::swap(this->pdf_x[j], this->pdf_x[j-1]); // order the x position

			for (unsigned k = 0; k < this->pdf_y.size(); k++)
				std::swap(this->pdf_y[k][j], this->pdf_y[k][j-1]); // the y follow their x moving the same way
		}
}
template <typename R>
void Distribution<R>
::compute_cdf(Distribution_mode mode)
{
	if (this->pdf_x.empty())
	{
		std::stringstream message;
		message << "'pdf_x' can't be empty.";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	for (unsigned k = 0; k < this->pdf_y.size(); k++)
	{
		if (this->pdf_x.size() != this->pdf_y[k].size())
		{
			std::stringstream message;
			message << "'pdf_x.size()' has to be equal to 'pdf_y[" << k << "].size()' ('pdf_x.size()' = " << pdf_x.size()
			        << " and 'pdf_y[" << k << "].size()' = " << pdf_y[k].size() << ").";
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}
	}

	sort_pdf(); // first make sure x values are sorted in ascending order

	this->cdf_x.resize(this->pdf_y.size());
	this->cdf_y.resize(this->pdf_y.size());

	switch (mode)
	{
		case Distribution_mode::SUMMATION:
			compute_cdf_summation();
			break;
		case Distribution_mode::INTERPOLATION:
			compute_cdf_interpolation();
			break;
	}
}

template <typename R>
void Distribution<R>
::compute_cdf_summation()
{
	for (unsigned k = 0; k < this->pdf_y.size(); k++)
	{
		std::vector<R> cumul_y(this->pdf_y[k].size());
		std::partial_sum(this->pdf_y[k].begin(), this->pdf_y[k].end(), cumul_y.begin());

		auto sum = cumul_y.back();

		if (sum == (R) 0)
		{
			std::stringstream message;
			message << "The sum of pdf_y[" << k << "] is null.";
			throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
		}

		// keep the parts of the cdf not parallel to the X axis -> pdf_y not null
		for (unsigned i = 0; i < this->pdf_y[k].size(); i++) // keep the first index
			if (this->pdf_y[k][i] != (R)0) // proba not null
			{
				this->cdf_x[k].push_back(this->pdf_x[i]);
				this->cdf_y[k].push_back(cumul_y[i]/sum);
			}
	}
}

template <typename R>
void Distribution<R>
::compute_cdf_interpolation()
{
	// normalize pdf with its integral value
	this->pdf_norm_y.resize(this->pdf_y.size());
	for (unsigned k = 0; k < this->pdf_y.size(); k++)
	{
		// resize pdf_norm
		this->pdf_norm_y[k].resize(this->pdf_x.size());

		// compute the integral of pdf_y[k] along pdf_x
		auto integ = tools::trapz_integral_seq(this->pdf_x.data(), this->pdf_y[k].data(), this->pdf_x.size());

		if (integ == (R)0)
		{
			std::stringstream message;
			message << "The integral of pdf_y[" << k << "] along pdf_x is null.";
			throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
		}

		// divide all elements by 'integ'
		std::transform(this->pdf_y[k].begin(), this->pdf_y[k].end(), this->pdf_norm_y[k].begin(), [integ](R& y){return y/integ;});
	}


	// interpolation on a bigger vector of the input pdf for better integration
	std::vector<R> interp_x(this->pdf_x.size() * 50);
	const auto min_x  = this->pdf_x.front();
	const auto max_x  = this->pdf_x.back ();
	const auto step_x = (max_x - min_x) / (interp_x.size() - 1);

	// create the interpolation x vector
	R x = min_x;
	for (unsigned i = 0; i < interp_x.size(); i++, x += step_x)
		interp_x[i] = x;
	interp_x.back() = max_x; // force the value to have exactly the max instead of an approximation after all the "+= step_x"


	// compute linear interpolation and the cumulative distribution
	for (unsigned k = 0; k < this->pdf_y.size(); k++)
	{
		std::vector<R> interp_y(interp_x.size());

		// compute the linear interpolation of pdf_norm_y along interp_x
		linear_interpolation(this->pdf_x.data(), this->pdf_norm_y[k].data(), this->pdf_x.size(),
		                     interp_x.data(), interp_y.data(), interp_x.size());

		// computing the cumulative distribution function for input pdf
		std::vector<R> cumul_y(interp_x.size());
		cumtrapz_integral_seq(interp_x.data(), interp_y.data(), cumul_y.data(), interp_x.size());

		// keep the first element of the cdf
//		this->cdf_x[k].push_back(interp_x[0]);
//		this->cdf_y[k].push_back(cumul_y [0]);

		// keep the parts of the cdf not parallel to the X axis
		for (unsigned i = 1; i < cumul_y.size(); i++) // keep the first index
//			if (cumul_y[i] != cumul_y[i-1]) // == means parallel to the X axis
			if (!tools::comp_equal(cumul_y[i] - cumul_y[i-1], (R)0)) // == means parallel to the X axis
			{
				this->cdf_x[k].push_back(interp_x[i]);
				this->cdf_y[k].push_back(cumul_y [i]);
			}
	}
}

template <typename R>
const std::vector<R>& Distribution<R>
::get_pdf_x() const
{
	return this->pdf_x;
}

template <typename R>
const std::vector<std::vector<R>>& Distribution<R>
::get_pdf_y() const
{
	return this->pdf_y;
}

template <typename R>
const std::vector<std::vector<R>>& Distribution<R>
::get_cdf_x() const
{
	return this->cdf_x;
}

template <typename R>
const std::vector<std::vector<R>>& Distribution<R>
::get_cdf_y() const
{
	return this->cdf_y;
}

template <typename R>
const std::vector<std::vector<R>>& Distribution<R>
::get_pdf_norm_y() const
{
	return this->pdf_norm_y;
}


// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::tools::Distribution<R_32>;
template class aff3ct::tools::Distribution<R_64>;
#else
template class aff3ct::tools::Distribution<R>;
#endif
// ==================================================================================== explicit template instantiation
