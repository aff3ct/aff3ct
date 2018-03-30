#ifndef DISTRIBUTION_HPP__
#define DISTRIBUTION_HPP__

#include <vector>

#include "Tools/Math/Point.hpp"

namespace aff3ct
{
namespace tools
{

template <typename R = float>
class Distribution
{
protected:
	std::vector<Point<R>> pdf;   // input probability density function as Points
	std::vector<R> pdf_x, pdf_y; // input probability density function as x and y

	std::vector<Point<R>> pdf_norm;        // normalized probability density function as Points
	std::vector<R> pdf_norm_x, pdf_norm_y; // normalized probability density function as x and y

	std::vector<Point<R>> cdf;   // cumulative density function as Points
	std::vector<R> cdf_x, cdf_y; // cumulative density function as x and y

public:
	Distribution(const std::vector<R>& _x_data, const std::vector<R>& _y_data);
	Distribution(const std::vector<Point<R>>& _pdf);
	Distribution(const std::vector<std::pair<R,R>>& _pdf);

	virtual ~Distribution();

	R get_min_x() const;
	R get_max_x() const;

	const std::vector<Point<R>>& get_cdf       () const;
	const std::vector<R>&        get_cdf_x     () const;
	const std::vector<R>&        get_cdf_y     () const;
	const std::vector<Point<R>>& get_pdf       () const;
	const std::vector<R>&        get_pdf_x     () const;
	const std::vector<R>&        get_pdf_y     () const;
	const std::vector<Point<R>>& get_pdf_norm  () const;
	const std::vector<R>&        get_pdf_norm_x() const;
	const std::vector<R>&        get_pdf_norm_y() const;

protected:
	void compute_cdf();
};

}
}

#include "Distribution.hxx"

#endif /* DISTRIBUTION_HPP__ */
