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
	std::vector<Point<R>> pdf;  // input probability density function
	std::vector<Point<R>> cdf; //  cumulative density function

public:
	User_pdf_noise_generator(const std::vector<R>& _xData, const std::vector<R>& _yData)
	: Noise_generator<R>(), pdf(_xData.size())
	{
		if (_xData.size() != _yData.size())
		{
			std::stringstream message;
			message << "'_xData.size()' has to be equal to 0 '_yData.size()' ('_xData.size()'' = " << _xData.size()
			        << " and '_yData.size()' = " << _yData.size() << ").";
			throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}

		for(unsigned i = 0; i < pdf.size(); i++)
		{
			pdf[i].x(_xData[i]);
			pdf[i].y(_yData[i]);
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
		for (unsigned i = 0; i < pdf.size(); i++)
		{
			pdf[i].x(_pdf[i].first);
			pdf[i].y(_pdf[i].second);
		}

		compute_cdf();
	}

	virtual ~User_pdf_noise_generator()
	{
	}

protected:
	void compute_cdf()
	{
		// first sort it in ascending order
		std::sort(pdf.begin(), pdf.end(), tools::x_cmp_lt<R>);

		// then normalize it with its integral value
		auto integ = tools::trapz_integral_seq(pdf.data(), pdf.size());
		std::for_each(pdf.begin(), pdf.end(), [integ](Point<R>& p){p.y(p.y()/integ);}); // divide all elements by 'integ'

		// interpolation on a bigger vector of the input pdf for better integration
		cdf.resize(10000);//(pdf.size() * 10);
		const auto min_x  = pdf.front().x();
		const auto max_x  = pdf.back().x();
		const auto step_x = (max_x - min_x) / (cdf.size() - 1);

		R x = min_x;
		for (unsigned i = 0; i < cdf.size(); i++, x += step_x)
			cdf[i].x(x);
		cdf.back().x(max_x); // force the value to have exactly the max instead of an approximation after the "+= step_x"

		linear_interpolation(pdf, cdf);

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


		// std::ofstream file0("cdf.csv");
		// for(unsigned i = 0; i < cdf.size(); i++)
		// 	file0 << i << "; " << cdf[i].x() << "; " << cdf[i].y() << std::endl;

	}
};

template <typename R = float>
using User_pdf_gen = User_pdf_noise_generator<R>;
}
}

#endif /* USER_PDF_NOISE_GENERATOR_HPP */
