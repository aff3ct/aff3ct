#ifndef USER_PDF_NOISE_GENERATOR_HPP
#define USER_PDF_NOISE_GENERATOR_HPP

#include <sstream>
#include <algorithm>
#include <vector>
#include <utility>

#include <iomanip>
#include <fstream>
#define SP << std::fixed << std::setw( 15 ) << std::setprecision( 6 ) <<

#include "Tools/Exception/exception.hpp"
#include "Tools/Math/Point.hpp"
#include "Tools/Math/numerical_integration.h"
#include "Tools/Math/interpolation.h"

#include "../Noise_generator.hpp"

namespace aff3ct
{
namespace tools
{
template <typename R = float>
class User_pdf_noise_generator : public Noise_generator<R>
{
protected:
	std::vector<Point<R>> pdf;   // input probability density function
	std::vector<R> cdf_x, cdf_y; // cumulative density function

public:
	User_pdf_noise_generator(const std::vector<R>& _x_data, const std::vector<R>& _y_data)
	: Noise_generator<R>(), pdf(_x_data.size())
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

	User_pdf_noise_generator(const std::vector<Point<R>>& _pdf)
	: Noise_generator<R>(), pdf(_pdf)
	{
		compute_cdf();
	}

	User_pdf_noise_generator(const std::vector<std::pair<R,R>>& _pdf)
	: Noise_generator<R>(), pdf(_pdf.size())
	{
		for (unsigned i = 0; i < this->pdf.size(); i++)
		{
			this->pdf[i].x(_pdf[i].first);
			this->pdf[i].y(_pdf[i].second);
		}

		compute_cdf();
	}

	virtual ~User_pdf_noise_generator()
	{
	}

	R get_min_x() const
	{
		return this->cdf_x.front();
	}

	R get_max_x() const
	{
		return this->cdf_x.back();
	}


protected:
	void compute_cdf()
	{
		// first sort it in ascending order
		std::sort(this->pdf.begin(), this->pdf.end(), tools::x_cmp_lt<R>);

		// then normalize it with its integral value
		auto integ = tools::trapz_integral_seq(this->pdf.data(), this->pdf.size());
		std::for_each(this->pdf.begin(), this->pdf.end(), [integ](Point<R>& p){p.y(p.y()/integ);}); // divide all elements by 'integ'

		// interpolation on a bigger vector of the input pdf for better integration
		std::vector<Point<R>> cdf(10000);//(pdf.size() * 10);
		const auto min_x  = this->pdf.front().x();
		const auto max_x  = this->pdf.back().x();
		const auto step_x = (max_x - min_x) / (cdf.size() - 1);

		R x = min_x;
		for (unsigned i = 0; i < cdf.size(); i++, x += step_x)
			cdf[i].x(x);
		cdf.back().x(max_x); // force the value to have exactly the max instead of an approximation after the "+= step_x"

		linear_interpolation(this->pdf, cdf);

		// computing the cumulative distribution function for input pdf
		std::vector<R> cumul(cdf.size());
		cumtrapz_integral_seq(cdf.data(), cumul.data(), cumul.size());

		for (unsigned i = 0; i < cdf.size(); i++)
			cdf[i].y(cumul[i]);

		// keeping the parts of cdf not parallel to the X axis
		std::vector<unsigned> erase_index;
		for (unsigned i = 1; i < cdf.size(); i++) // keep the first index
			if (tools::comp_equal(cdf[i].y() - cdf[i-1].y(), (R)0)) // == 0 means parallel to the X axis
				erase_index.push_back(i);

		for (unsigned i = 0; i < erase_index.size(); i++)
			cdf.erase(cdf.begin() + erase_index[i] - i);

		this->cdf_x.resize(cdf.size());
		this->cdf_y.resize(cdf.size());

		for (unsigned i = 0; i < cdf.size(); i++)
		{
			this->cdf_x[i] = cdf[i].x();
			this->cdf_y[i] = cdf[i].y();
		}
	}
};

template <typename R = float>
using User_pdf_gen = User_pdf_noise_generator<R>;
}
}

#endif /* USER_PDF_NOISE_GENERATOR_HPP */
