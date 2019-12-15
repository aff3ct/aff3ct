/*!
 * \file
 * \brief Class tools::Frozenbits_generator_TV.
 */
#ifndef FROZENBITS_GENERATOR_TV_HPP_
#define FROZENBITS_GENERATOR_TV_HPP_

#include <string>

#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator_file.hpp"

namespace aff3ct
{
namespace tools
{
class Frozenbits_generator_TV : public Frozenbits_generator_file
{
private:
	const int m;
	const std::string awgn_codes_dir;
	const std::string bin_pb_path;

	const static int Mu; // quality of channels generated

public:
	Frozenbits_generator_TV(const int K, const int N,
	                        const std::string &awgn_codes_dir,
	                        const std::string &bin_pb_path);

	virtual ~Frozenbits_generator_TV() = default;

	virtual Frozenbits_generator_TV* clone() const;

protected:
	void evaluate();
	virtual void check_noise();
};
}
}

#endif /* FROZENBITS_GENERATOR_TV_HPP_ */
