#ifdef CHANNEL_MKL

#ifndef User_pdf_noise_generator_MKL_HPP_
#define User_pdf_noise_generator_MKL_HPP_

#include <mkl_vsl.h>

#include "Tools/Math/interpolation.h"

#include "../User_pdf_noise_generator.hpp"

namespace aff3ct
{
namespace tools
{

template <typename R = float>
class User_pdf_noise_generator_MKL : public User_pdf_noise_generator<R>
{
private:
	VSLStreamStatePtr stream_state;
	bool              is_stream_alloc;

	R (*interp_function)(const R*, const R*, const unsigned, const R);

public:
	explicit User_pdf_noise_generator_MKL(const tools::Distributions<R>& dists, const int seed = 0, Interpolation_type inter_type = Interpolation_type::NEAREST);
	virtual ~User_pdf_noise_generator_MKL() = default;

	virtual void set_seed(const int seed);

	virtual void generate(                 R *draw, const unsigned length, const R noise_power);
	virtual void generate(const R* signal, R *draw, const unsigned length, const R noise_power);
};

template <typename R = float>
using User_pdf_gen_MKL = User_pdf_noise_generator_MKL<R>;
}
}

#endif /* User_pdf_noise_generator_MKL_HPP_ */

#endif // MKL
