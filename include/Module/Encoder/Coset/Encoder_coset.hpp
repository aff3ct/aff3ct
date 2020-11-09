/*!
 * \file
 * \brief Class module::Encoder_coset.
 */
#ifndef ENCODER_COSET_HPP_
#define ENCODER_COSET_HPP_

#include <cstdint>
#include <vector>
#include <random>

#include "Module/Encoder/Encoder.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int>
class Encoder_coset : public Encoder<B>
{
private:
	std::mt19937 rd_engine; // Mersenne Twister 19937
#ifdef _MSC_VER
	std::uniform_int_distribution<short> uniform_dist;
#else
	std::uniform_int_distribution<B> uniform_dist;
#endif

public:
	Encoder_coset(const int K, const int N, const int seed = 0);
	virtual ~Encoder_coset() = default;

	virtual Encoder_coset<B>* clone() const;

	const std::vector<uint32_t>& get_info_bits_pos() const;

	bool is_sys() const;

	virtual void set_seed(const int seed);

protected:
	void _encode(const B *U_K, B *X_N, const size_t frame_id);
};
}
}

#endif /* ENCODER_COSET_HPP_ */
