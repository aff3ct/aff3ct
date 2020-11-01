/*!
 * \file
 * \brief Class module::Extractor_LDPC.
 */
#ifndef EXTRACTOR_LDPC_HPP_
#define EXTRACTOR_LDPC_HPP_

#include <cstdint>
#include <vector>

#include "Module/Extractor/Extractor.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename Q = float>
class Extractor_LDPC : public Extractor<B,Q>
{
protected:
	const std::vector<uint32_t> &info_bits_pos;

public:
	Extractor_LDPC(const int K, const int N, const std::vector<uint32_t> &info_bits_pos);
	virtual ~Extractor_LDPC() = default;

	virtual Extractor_LDPC<B,Q>* clone() const;

protected:
	const std::vector<uint32_t>& get_info_bits_pos();
};
}
}

#endif /* EXTRACTOR_LDPC_HPP_ */
