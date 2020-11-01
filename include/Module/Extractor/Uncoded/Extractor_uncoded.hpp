/*!
 * \file
 * \brief Class module::Extractor_uncoded.
 */
#ifndef EXTRACTOR_UNCODED_HPP_
#define EXTRACTOR_UNCODED_HPP_

#include <cstdint>
#include <vector>

#include "Module/Extractor/Extractor.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename Q = float>
class Extractor_uncoded : public Extractor<B,Q>
{
protected:
	std::vector<uint32_t> info_bits_pos;

public:
	Extractor_uncoded(const int K);
	virtual ~Extractor_uncoded() = default;

	virtual Extractor_uncoded<B,Q>* clone() const;

protected:
	const std::vector<uint32_t>& get_info_bits_pos();
};
}
}

#endif /* EXTRACTOR_UNCODED_HPP_ */
