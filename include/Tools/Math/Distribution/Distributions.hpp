/*!
 * \file
 * \brief Class tools::Distributions.
 */
#ifndef DISTRIBUTIONS_HPP
#define DISTRIBUTIONS_HPP

#include <ios>
#include <fstream>
#include <string>
#include <map>
#include <mutex>
#include <memory>
#include <vector>

#include "Tools/Math/Distribution/Distribution.hpp"

namespace aff3ct
{
namespace tools
{
/*
 * Warning all contained distributions are deleted at destruction time.
 */
template <typename R = float>
class Distributions
{
public:
	static const int saved_noise_precision;

protected:
	std::map<int, std::unique_ptr<Distribution<R>>> distributions; // distributions in function of the noise power
	std::ifstream f_distributions;

	Distribution_mode mode;

	std::vector<R> noise_range;
	std::vector<R> noise_range_sorted;
	std::vector<std::streampos> noise_file_index;

	// the data description
	std::vector<std::string> desc;
	size_t ROP_pos;
	size_t x_pos;
	size_t y0_pos;
	size_t y1_pos;

	std::mutex mtx;

public:
	explicit Distributions(const std::string& filename, Distribution_mode mode = Distribution_mode::SUMMATION);

	virtual ~Distributions() = default;

	bool has_distribution(R noise) const;
	const Distribution<R>& get_distribution(R noise) const;
	void read_distribution(R noise);

	const std::vector<R>& get_noise_range() const;

protected:
	/*
	 * Add a distribution 'new_distribution' associated with the noise power 'noise_power'.
	 */
	void add_distribution(R noise, std::unique_ptr<Distribution<R>>&& new_distribution);
	void read_noise_range();
	void read_distribution_from_file(unsigned index);

	static int calibrated_noise(R noise);
	void file_go_to_pos(unsigned index = -1);
};

}
}

#endif /* DISTRIBUTIONS_HPP */
