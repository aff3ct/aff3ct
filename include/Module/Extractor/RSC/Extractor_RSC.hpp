/*!
 * \file
 * \brief Class module::Extractor_RSC.
 */
#ifndef EXTRACTOR_RSC_HPP_
#define EXTRACTOR_RSC_HPP_

#include <cstdint>
#include <vector>

#include "Module/Extractor/Extractor.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename Q = float>
class Extractor_RSC : public Extractor<B,Q>
{
protected:
	const bool buffered_encoding;
	std::vector<uint32_t> info_bits_pos;

public:
	Extractor_RSC(const int K, const int N, const int tail_length, const bool buffered_encoding = true,
	              const int n_frames = 1);
	virtual ~Extractor_RSC() = default;


protected:
	const std::vector<uint32_t>& get_info_bits_pos();

	void _get_sys_and_par_llr(const Q *Y_N, Q *sys, Q *par, const int frame_id);
	void _get_sys_llr        (const Q *Y_N, Q *sys,         const int frame_id);
	void _add_sys_and_ext_llr(const Q *ext, Q *Y_N,         const int frame_id);
	void _get_sys_bit        (const Q *Y_N, B *V_K,         const int frame_id);
};
}
}

#endif /* EXTRACTOR_RSC_HPP_ */
