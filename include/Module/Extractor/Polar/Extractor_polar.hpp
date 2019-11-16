/*!
 * \file
 * \brief Class module::Extractor_polar.
 */
#ifndef EXTRACTOR_POLAR_HPP_
#define EXTRACTOR_POLAR_HPP_

#include <cstdint>
#include <vector>

#include "Module/Extractor/Extractor.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename Q = float>
class Extractor_polar : public Extractor<B,Q>
{
protected:
	const std::vector<bool>& frozen_bits; // known bits (alias frozen bits) are set to true
	std::vector<uint32_t> info_bits_pos;

public:
	Extractor_polar(const int K, const int N, const std::vector<bool>& frozen_bits, const int n_frames = 1);
	virtual ~Extractor_polar() = default;

	virtual Extractor_polar<B,Q>* clone() const;

protected:
	const std::vector<uint32_t>& get_info_bits_pos();
};
}
}

#endif /* EXTRACTOR_POLAR_HPP_ */
