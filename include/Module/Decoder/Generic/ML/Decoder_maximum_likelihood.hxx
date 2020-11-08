#include <sstream>
#include <cstdint>
#include <string>

#include "Tools/Exception/exception.hpp"
#include "Module/Decoder/Generic/ML/Decoder_maximum_likelihood.hpp"

namespace aff3ct
{
namespace module
{
template <typename B, typename R>
Decoder_maximum_likelihood<B,R>
::Decoder_maximum_likelihood(const int K, const int N, const Encoder<B> &encoder)
: Decoder_SIHO<B,R>(K, N),
  encoder(encoder.clone()),
  U_K(K+7), // +7 to avoid segmentation fault when casting B to uint64_t
  X_N(N+7), // +7 to avoid segmentation fault when casting B to uint64_t
  best_U_K(K),
  best_X_N(N),
  hard_Y_N(N)
{
	const std::string name = "Decoder_maximum_likelihood";
	this->set_name(name);

	if (encoder.get_K() != K)
	{
		std::stringstream message;
		message << "'encoder.get_K()' has to be equal to 'K' ('encoder.get_K()' = " << encoder.get_K()
		        << ", 'K' = " << K << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (encoder.get_N() != N)
	{
		std::stringstream message;
		message << "'encoder.get_N()' has to be equal to 'N' ('encoder.get_N()' = " << encoder.get_N()
		        << ", 'N' = " << N << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename B, typename R>
Decoder_maximum_likelihood<B,R>* Decoder_maximum_likelihood<B,R>
::clone() const
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename B, typename R>
void Decoder_maximum_likelihood<B,R>
::deep_copy(const Decoder_maximum_likelihood<B,R> &m)
{
	Module::deep_copy(m);
	if (m.encoder != nullptr) this->encoder.reset(m.encoder->clone());
}

template <typename B, typename R>
float Decoder_maximum_likelihood<B,R>
::compute_euclidean_dist(const B *X_N, const R *Y_N) const
{
	// compute the Euclidean distance between the LLR (Y_N) and the current codeword (X_N)
	float euclidean_dist = 0.f;
	for (auto n = 0; n < this->N; n++)
	{
		auto tmp = (X_N[n] ? -1.f : +1.f) - (float)Y_N[n];
		euclidean_dist += tmp * tmp;
		// euclidean_dist += std::abs(tmp); // performance degradation with the abs
	}

	// we can avoid the sqrt because of the monotonicity of the sqrt function
	// euclidean_dist = std::sqrt(euclidean_dist);

	return euclidean_dist;
}

template <typename B, typename R>
uint32_t Decoder_maximum_likelihood<B,R>
::compute_hamming_dist(const B *X_N, const B *Y_N) const
{
	// compute the Hamming distance between the input bits (Y_N) and the current codeword (X_N)
	uint32_t hamming_dist = 0;
	for (auto n = 0; n < this->N; n++)
		hamming_dist += X_N[n] != Y_N[n];

	return hamming_dist;
}
}
}
