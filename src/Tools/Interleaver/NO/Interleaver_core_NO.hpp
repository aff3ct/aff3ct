#ifndef INTERLEAVER_CORE_NO_HPP
#define INTERLEAVER_CORE_NO_HPP

#include "../Interleaver_core.hpp"

namespace aff3ct
{
namespace tools
{
template <typename T = uint32_t>
class Interleaver_core_NO : public Interleaver_core<T>
{
public:
	Interleaver_core_NO(const int size, const int n_frames = 1);
	virtual ~Interleaver_core_NO() = default;

protected:
	void gen_lut(T *lut, const int frame_id);
};
}
}

#endif	/* INTERLEAVER_CORE_NO_HPP */
