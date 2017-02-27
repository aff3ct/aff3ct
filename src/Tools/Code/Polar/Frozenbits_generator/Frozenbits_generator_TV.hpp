#ifndef FROZENBITS_GENERATOR_TV_HPP_
#define FROZENBITS_GENERATOR_TV_HPP_

#include <string>
#include <vector>

#include "Frozenbits_generator_file.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int>
class Frozenbits_generator_TV : public Frozenbits_generator_file<B>
{
private:
	const int m;
	const std::string awgn_codes_dir;
	const std::string bin_pb_path;

	const static int Mu; // quality of channels generated

public:
	Frozenbits_generator_TV(const int K, const int N,
	                        const std::string awgn_codes_dir,
	                        const std::string bin_pb_path,
	                        float sigma = 0.f);

	virtual ~Frozenbits_generator_TV();

protected:
	void evaluate();
};
}
}

#endif /* FROZENBITS_GENERATOR_TV_HPP_ */
