#ifndef INTERLEAVER_NO_HPP
#define	INTERLEAVER_NO_HPP

#include <numeric>

#include "../Interleaver.hpp"

namespace aff3ct
{
namespace module
{
template <typename T = int>
class Interleaver_NO : public Interleaver<T>
{
public:
	Interleaver_NO(int size, const int n_frames = 1, const std::string name = "Interleaver_NO")
	: Interleaver<T>(size, false, n_frames, name)
	{
	}

	virtual ~Interleaver_NO()
	{
	}

protected:
	void gen_lut(T *lut, const int frame_id)
	{
		std::iota(lut, lut + this->get_size(), 0);
	}
};
}
}

#endif	/* INTERLEAVER_NO_HPP */
