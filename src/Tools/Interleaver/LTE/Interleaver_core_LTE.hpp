#ifndef INTERLEAVER_CORE_LTE_HPP
#define INTERLEAVER_CORE_LTE_HPP

#include "../Interleaver_core.hpp"

namespace aff3ct
{
namespace tools
{
template <typename T = uint32_t>
class Interleaver_core_LTE : public Interleaver_core<T>
{
public:
	Interleaver_core_LTE(const int size, const int n_frames = 1);
	virtual ~Interleaver_core_LTE() = default;

protected:
	void gen_lut(T *lut, const int frame_id);

private:
	static inline int pi_LTE(const int &i, const int &f_1, const int &f_2, const int &K);
};
}
}

#endif	/* INTERLEAVER_CORE_LTE_HPP */
