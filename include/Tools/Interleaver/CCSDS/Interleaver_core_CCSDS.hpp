/*!
 * \file
 * \brief Class tools::Interleaver_core_CCSDS.
 */
#ifndef INTERLEAVER_CORE_CCSDS_HPP
#define INTERLEAVER_CORE_CCSDS_HPP

#include <cstdint>

#include "Tools/Interleaver/Interleaver_core.hpp"

namespace aff3ct
{
namespace tools
{
template <typename T = uint32_t>
class Interleaver_core_CCSDS : public Interleaver_core<T>
{
public:
	Interleaver_core_CCSDS(const int size);
	virtual ~Interleaver_core_CCSDS() = default;

	virtual Interleaver_core_CCSDS<T>* clone() const;

protected:
	void gen_lut(T *lut, const size_t frame_id);

private:
	static inline int pi_CCSDS(const int &index, const int &k_1, const int &k_2);
};
}
}

#endif	/* INTERLEAVER_CORE_CCSDS_HPP */
