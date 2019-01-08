#ifndef FROZENBITS_GENERATOR_FILE_HPP_
#define FROZENBITS_GENERATOR_FILE_HPP_

#include <string>
#include <vector>

#include "Frozenbits_generator.hpp"

namespace aff3ct
{
namespace tools
{
class Frozenbits_generator_file : public Frozenbits_generator
{
private:
	const std::string filename;

protected:
	Frozenbits_generator_file(const int K, const int N);

public:
	Frozenbits_generator_file(const int K, const int N, const std::string& filename);

	virtual ~Frozenbits_generator_file() = default;

protected:
	void evaluate();
	bool load_channels_file(const std::string& filename);
	virtual void check_noise();
};
}
}

#endif /* FROZENBITS_GENERATOR_FILE_HPP_ */
