#ifndef DISTRIBUTION_HPP__
#define DISTRIBUTION_HPP__

#include <vector>

namespace aff3ct
{
namespace tools
{

template <typename R = float>
class Distribution
{
protected:
	std::vector<R>              pdf_x; // given probability density function as x
	std::vector<std::vector<R>> pdf_y; // given probability density function as y

	std::vector<std::vector<R>> pdf_norm_y; // normalized probability density function as y

	std::vector<std::vector<R>> cdf_x; // cumulative density function as x
	std::vector<std::vector<R>> cdf_y; // cumulative density function as y

public:
	Distribution(const std::vector<R>& _x_data, const std::vector<R>& _y_data);
	Distribution(std::vector<R>&& _x_data, std::vector<R>&& _y_data);
	Distribution(const std::vector<R>& _x_data, const std::vector<std::vector<R>>& _y_data);
	Distribution(std::vector<R>&& _x_data, std::vector<std::vector<R>>&& _y_data);

	virtual ~Distribution();

	const std::vector<R>&              get_pdf_x     () const;
	const std::vector<std::vector<R>>& get_pdf_y     () const;
	const std::vector<std::vector<R>>& get_cdf_x     () const;
	const std::vector<std::vector<R>>& get_cdf_y     () const;
	const std::vector<std::vector<R>>& get_pdf_norm_y() const;

protected:
	void compute_cdf();
	void sort_pdf();
};

}
}

#endif /* DISTRIBUTION_HPP__ */
