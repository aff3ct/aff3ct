/*!
 * \file
 * \brief Class tools::Interleaver_core_NO.
 */
#ifndef INTERLEAVER_CORE_NO_HPP
#define INTERLEAVER_CORE_NO_HPP

#include <cstdint>

#include "Tools/Interleaver/Interleaver_core.hpp"

namespace aff3ct
{
namespace tools
{
template <typename T = uint32_t>
class Interleaver_core_NO : public Interleaver_core<T>
{
public:
	Interleaver_core_NO(const int size);
	virtual ~Interleaver_core_NO() = default;

	virtual Interleaver_core_NO<T>* clone() const;

protected:
	void gen_lut(T *lut, const size_t frame_id);
};
}
}

#endif	/* INTERLEAVER_CORE_NO_HPP */
