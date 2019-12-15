/*!
 * \file
 * \brief Class tools::Frozenbits_generator_file.
 */
#ifndef FROZENBITS_GENERATOR_FILE_HPP_
#define FROZENBITS_GENERATOR_FILE_HPP_

#include <cstdint>
#include <string>
#include <vector>

#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator.hpp"

namespace aff3ct
{
namespace tools
{
class Frozenbits_generator_file : public Frozenbits_generator
{
protected:
	const std::string filename;

protected:
	Frozenbits_generator_file(const int K, const int N);

public:
	Frozenbits_generator_file(const int K, const int N, const std::string& filename);

	virtual ~Frozenbits_generator_file() = default;

	virtual Frozenbits_generator_file* clone() const;

protected:
	void evaluate();
	bool load_channels_file(const std::string& filename, std::vector<uint32_t>& best_channels);
	virtual void check_noise();
};
}
}

#endif /* FROZENBITS_GENERATOR_FILE_HPP_ */
