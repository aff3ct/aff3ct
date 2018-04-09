#ifndef DISTRIBUTIONS_HPP
#define DISTRIBUTIONS_HPP

#include <fstream>
#include <string>
#include <map>

#include "Distribution.hpp"

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
protected:
	std::map<int, Distribution<R>*> distributions; // distributions in function of the noise power
	const int saved_noise_precision = 1e6;

public:
	Distributions();

	Distributions(std::ifstream& f_distributions);

	virtual ~Distributions();

	const Distribution<R>* const get_distribution(const R noise_power) const;

	static std::vector<R> get_noise_range(std::ifstream& f_distributions);

	/*
	 * Add a distribution 'new_distribution' associated with the noise power 'noise_power'.
	 */
	void add_distribution(R noise_power, Distribution<R>* new_distribution);
};

}
}

#endif /* DISTRIBUTIONS_HPP */
