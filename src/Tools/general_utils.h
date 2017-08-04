#ifndef GENERAL_UTILS_H_
#define GENERAL_UTILS_H_

#include <vector>
#include <string>
#include <mipp.h>

namespace aff3ct
{
namespace tools
{
std::vector<std::string> string_split(const std::string &s, char delim);

template <typename R = float>
R sigma_to_esn0(const R sigma, const int upsample_factor = 1);

template <typename R = float>
R esn0_to_sigma(const R esn0, const int upsample_factor = 1);

template <typename R = float>
R esn0_to_ebn0(const R esn0, const R bit_rate = 1, const int bps = 1);

template <typename R = float>
R ebn0_to_esn0(const R ebn0, const R bit_rate = 1, const int bps = 1);

template <typename B = int, typename R = float>
inline void hard_decide(const R *in, B *out, const int size)
{
	const auto vec_loop_size = (size / mipp::nElReg<R>()) * mipp::nElReg<R>();
	for (auto i = 0; i < vec_loop_size; i += mipp::nElReg<R>())
	{
		const auto r_in  = mipp::Reg<R>(&in[i]);
		const auto r_out = mipp::cast<R,B>(r_in) >> (sizeof(B) * 8 - 1);
		r_out.store(&out[i]);
	}
	for (auto i = vec_loop_size; i < size; i++)
		out[i] = in[i] < 0;
}
}
}

#endif /* GENERAL_UTILS_H_*/
