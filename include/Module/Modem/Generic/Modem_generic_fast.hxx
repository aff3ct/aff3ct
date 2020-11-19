#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <cmath>
#include <algorithm>
#include <limits>
#include <string>
#include <sstream>
#include <type_traits>
#include <complex>
#include <mipp.h>

#include "Tools/Noise/Noise.hpp"
#include "Tools/Exception/exception.hpp"
#include "Module/Modem/Generic/Modem_generic_fast.hpp"

namespace aff3ct
{
namespace module
{
template <typename B, typename R, typename Q, tools::proto_max<Q> MAX, tools::proto_max_i<Q> MAXI>
Modem_generic_fast<B,R,Q,MAX,MAXI>
::Modem_generic_fast(const int N, const tools::Constellation<R>& _cstl, const bool disable_sig2)
: Modem_generic<B,R,Q,MAX>(N, _cstl, disable_sig2),
  cstl_bis(this->cstl.size())
{
	const std::string name = "Modem_generic_fast<" + this->cstl.get_name() + ">";
	this->set_name(name);

	for (size_t i = 0; i < this->cstl.size(); i++)
	{
		this->cstl_bis[i].real((Q)this->cstl[i].real());
		this->cstl_bis[i].imag((Q)this->cstl[i].imag());
	}
}

template <typename B, typename R, typename Q, tools::proto_max<Q> MAX, tools::proto_max_i<Q> MAXI>
Modem_generic_fast<B,R,Q,MAX,MAXI>* Modem_generic_fast<B,R,Q,MAX,MAXI>
::clone() const
{
	auto m = new Modem_generic_fast(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B, typename R, typename Q, tools::proto_max<Q> MAX, tools::proto_max_i<Q> MAXI>
void Modem_generic_fast<B,R,Q,MAX,MAXI>
::_tdemodulate(const float *CP, const Q *Y_N1, const Q *Y_N2, Q *Y_N3, const size_t frame_id)
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename B, typename R, typename Q, tools::proto_max<Q> MAX, tools::proto_max_i<Q> MAXI>
void Modem_generic_fast<B,R,Q,MAX,MAXI>
::_tdemodulate_wg(const float *CP, const R *H_N, const Q *Y_N1, const Q *Y_N2, Q *Y_N3, const size_t frame_id)
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename Q, typename I, tools::proto_max_i<Q> MAXI>
size_t kernel_demodulate_complex_SIMD(const Q *Y_N1,
                                            Q *Y_N2,
                                      const int N,
                                      const std::vector<std::complex<Q>> &cstl,
                                      const int bits_per_symbol,
                                      const int nbr_symbols,
                                      const Q inv_sigma2)
{
	const auto size_vec_loop = (N / mipp::N<Q>()) * mipp::N<Q>();

	const mipp::Reg<Q> reg_zero       = (Q)0;
	const mipp::Reg<Q> reg_two        = (Q)2;
	const mipp::Reg<I> regi_zero      = (I)0;
	const mipp::Reg<I> regi_one       = (I)1;
	const mipp::Reg<Q> reg_inv_sigma2 = (Q)inv_sigma2;

	I arr_b   [mipp::N<Q>()];
	Q arr_Y_re[mipp::N<Q>()];
	Q arr_Y_im[mipp::N<Q>()];

	for (auto n = 0; n < size_vec_loop; n += mipp::N<Q>()) // loop upon the LLRs
	{
		mipp::Reg<Q> reg_L0 = -std::numeric_limits<Q>::infinity();
		mipp::Reg<Q> reg_L1 = -std::numeric_limits<Q>::infinity();

		for (auto i = 0; i < mipp::N<Q>(); i++)
			arr_b[i] = (n +i) % bits_per_symbol;
		mipp::Reg<I> regi_b = arr_b;

		for (auto i = 0; i < mipp::N<Q>(); i++)
		{
			auto k = (n +i) / bits_per_symbol;
			arr_Y_re[i] = Y_N1[2 * k +0];
			arr_Y_im[i] = Y_N1[2 * k +1];
		}
		const mipp::Reg<Q> reg_Y_re = arr_Y_re;
		const mipp::Reg<Q> reg_Y_im = arr_Y_im;
		const mipp::Regx2<Q> reg_complex_Yk(reg_Y_re, reg_Y_im);

		mipp::Reg<I> regi_j = regi_zero;
		for (auto j = 0; j < nbr_symbols; j++)
		{
			const mipp::Reg<Q> reg_cstl_re = cstl[j].real();
			const mipp::Reg<Q> reg_cstl_im = cstl[j].imag();
			const mipp::Regx2<Q> reg_cstl(reg_cstl_re, reg_cstl_im);

			const auto reg_res = (reg_zero - mipp::norm(reg_complex_Yk - reg_cstl)) * reg_inv_sigma2;

			const auto reg_max_L0 = MAXI(reg_L0, reg_res);
			const auto reg_max_L1 = MAXI(reg_L1, reg_res);

			const auto msk_L0_or_L1 = ((regi_j >> regi_b) & regi_one) == regi_zero;

			reg_L0 = mipp::blend(reg_max_L0, reg_L0,     msk_L0_or_L1);
			reg_L1 = mipp::blend(reg_L1,     reg_max_L1, msk_L0_or_L1);

			regi_j += regi_one;
		}

		auto reg_Y_N2 = reg_L0 - reg_L1;
		reg_Y_N2.store(&Y_N2[n]);
	}

	return (size_t)size_vec_loop;
}

template <typename Q, tools::proto_max_i<Q> MAXI>
struct demodulate_complex_SIMD
{
	static size_t compute(const Q *Y_N1,
	                            Q *Y_N2,
	                      const int N,
	                      const std::vector<std::complex<Q>> &cstl,
	                      const int bits_per_symbol,
	                      const int nbr_symbols,
	                      const Q inv_sigma2)
	{
		throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
	}
};

template <tools::proto_max_i<float> MAXI>
struct demodulate_complex_SIMD<float, MAXI>
{
	using Q = float;
	using I = int32_t;
	static size_t compute(const Q *Y_N1,
	                            Q *Y_N2,
	                      const int N,
	                      const std::vector<std::complex<Q>> &cstl,
	                      const int bits_per_symbol,
	                      const int nbr_symbols,
	                      const Q inv_sigma2)
	{
		return kernel_demodulate_complex_SIMD<Q,I,MAXI>(Y_N1, Y_N2, N, cstl, bits_per_symbol, nbr_symbols, inv_sigma2);
	}
};

template <tools::proto_max_i<double> MAXI>
struct demodulate_complex_SIMD<double, MAXI>
{
	using Q = double;
	using I = int64_t;
	static size_t compute(const Q *Y_N1,
	                            Q *Y_N2,
	                      const int N,
	                      const std::vector<std::complex<Q>> &cstl,
	                      const int bits_per_symbol,
	                      const int nbr_symbols,
	                      const Q inv_sigma2)
	{
		return kernel_demodulate_complex_SIMD<Q,I,MAXI>(Y_N1, Y_N2, N, cstl, bits_per_symbol, nbr_symbols, inv_sigma2);
	}
};

template <typename B,typename R, typename Q, tools::proto_max<Q> MAX, tools::proto_max_i<Q> MAXI>
void Modem_generic_fast<B,R,Q,MAX,MAXI>
::_demodulate_complex(const Q *Y_N1, Q *Y_N2, const size_t frame_id)
{
	if (!std::is_same<R,Q>::value)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Type 'R' and 'Q' have to be the same.");

	if (!std::is_floating_point<Q>::value)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Type 'Q' has to be float or double.");

	const auto size_vec_loop = demodulate_complex_SIMD<Q, MAXI>::compute(Y_N1,
	                                                                     Y_N2,
	                                                                     this->N,
	                                                                     this->cstl_bis,
	                                                                     this->bits_per_symbol,
	                                                                     this->nbr_symbols,
	                                                                     (Q)this->inv_sigma2);

	for (auto n = (int)size_vec_loop; n < this->N; n++) // loop upon the LLRs
	{
		auto L0 = -std::numeric_limits<Q>::infinity();
		auto L1 = -std::numeric_limits<Q>::infinity();
		auto b  = n % this->bits_per_symbol; // bit position in the symbol
		auto k  = n / this->bits_per_symbol; // symbol position

		auto complex_Yk = std::complex<Q>(Y_N1[2*k], Y_N1[2*k+1]);

		for (auto j = 0; j < this->nbr_symbols; j++)
			if (((j>>b) & 1) == 0)
				L0 = MAX(L0, -std::norm(complex_Yk - std::complex<Q>((Q)this->cstl[j].real(),
				                                                     (Q)this->cstl[j].imag())) * (Q)this->inv_sigma2);
			else
				L1 = MAX(L1, -std::norm(complex_Yk - std::complex<Q>((Q)this->cstl[j].real(),
				                                                     (Q)this->cstl[j].imag())) * (Q)this->inv_sigma2);

		Y_N2[n] = (L0 - L1);
	}
}

template <typename Q, typename I, tools::proto_max_i<Q> MAXI>
size_t kernel_demodulate_wg_complex_SIMD(const Q *H_N,
                                         const Q *Y_N1,
                                               Q *Y_N2,
                                         const int N,
                                         const std::vector<std::complex<Q>> &cstl,
                                         const int bits_per_symbol,
                                         const int nbr_symbols,
                                         const Q inv_sigma2)
{
	const auto size_vec_loop = (N / mipp::N<Q>()) * mipp::N<Q>();

	const mipp::Reg<Q> reg_zero       = (Q)0;
	const mipp::Reg<Q> reg_two        = (Q)2;
	const mipp::Reg<I> regi_zero      = (I)0;
	const mipp::Reg<I> regi_one       = (I)1;
	const mipp::Reg<Q> reg_inv_sigma2 = (Q)inv_sigma2;

	I arr_b   [mipp::N<Q>()];
	Q arr_Y_re[mipp::N<Q>()];
	Q arr_Y_im[mipp::N<Q>()];
	Q arr_H_re[mipp::N<Q>()];
	Q arr_H_im[mipp::N<Q>()];

	for (auto n = 0; n < size_vec_loop; n += mipp::N<Q>()) // loop upon the LLRs
	{
		mipp::Reg<Q> reg_L0 = -std::numeric_limits<Q>::infinity();
		mipp::Reg<Q> reg_L1 = -std::numeric_limits<Q>::infinity();

		for (auto i = 0; i < mipp::N<Q>(); i++)
			arr_b[i] = (n +i) % bits_per_symbol;
		mipp::Reg<I> regi_b = arr_b;

		for (auto i = 0; i < mipp::N<Q>(); i++)
		{
			auto k = (n +i) / bits_per_symbol;
			arr_Y_re[i] = Y_N1[2 * k +0];
			arr_Y_im[i] = Y_N1[2 * k +1];
			arr_H_re[i] = H_N [2 * k +0];
			arr_H_im[i] = H_N [2 * k +1];
		}
		const mipp::Reg<Q> reg_Y_re = arr_Y_re;
		const mipp::Reg<Q> reg_Y_im = arr_Y_im;
		const mipp::Regx2<Q> reg_complex_Yk(reg_Y_re, reg_Y_im);

		const mipp::Reg<Q> reg_H_re = arr_H_re;
		const mipp::Reg<Q> reg_H_im = arr_H_im;
		const mipp::Regx2<Q> reg_complex_Hk(reg_H_re, reg_H_im);

		mipp::Reg<I> regi_j = regi_zero;
		for (auto j = 0; j < nbr_symbols; j++)
		{
			const mipp::Reg<Q> reg_cstl_re = cstl[j].real();
			const mipp::Reg<Q> reg_cstl_im = cstl[j].imag();
			const mipp::Regx2<Q> reg_cstl(reg_cstl_re, reg_cstl_im);

			const auto reg_res = (reg_zero - mipp::norm(reg_complex_Yk - reg_complex_Hk * reg_cstl)) * reg_inv_sigma2;

			const auto reg_max_L0 = MAXI(reg_L0, reg_res);
			const auto reg_max_L1 = MAXI(reg_L1, reg_res);

			const auto msk_L0_or_L1 = ((regi_j >> regi_b) & regi_one) == regi_zero;

			reg_L0 = mipp::blend(reg_max_L0, reg_L0,     msk_L0_or_L1);
			reg_L1 = mipp::blend(reg_L1,     reg_max_L1, msk_L0_or_L1);

			regi_j += regi_one;
		}

		auto reg_Y_N2 = reg_L0 - reg_L1;
		reg_Y_N2.store(&Y_N2[n]);
	}

	return (size_t)size_vec_loop;
}

template <typename R, typename Q, tools::proto_max_i<Q> MAXI>
struct demodulate_wg_complex_SIMD
{
	static size_t compute(const R *H_N,
	                      const Q *Y_N1,
	                            Q *Y_N2,
	                      const int N,
	                      const std::vector<std::complex<Q>> &cstl,
	                      const int bits_per_symbol,
	                      const int nbr_symbols,
	                      const Q inv_sigma2)
	{
		throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
	}
};

template <tools::proto_max_i<float> MAXI>
struct demodulate_wg_complex_SIMD<float, float, MAXI>
{
	using Q = float;
	using I = int32_t;
	static size_t compute(const Q *H_N,
	                      const Q *Y_N1,
	                            Q *Y_N2,
	                      const int N,
	                      const std::vector<std::complex<Q>> &cstl,
	                      const int bits_per_symbol,
	                      const int nbr_symbols,
	                      const Q inv_sigma2)
	{
		return kernel_demodulate_wg_complex_SIMD<Q,I,MAXI>(H_N, Y_N1, Y_N2, N, cstl, bits_per_symbol, nbr_symbols, inv_sigma2);
	}
};

template <tools::proto_max_i<double> MAXI>
struct demodulate_wg_complex_SIMD<double, double, MAXI>
{
	using Q = double;
	using I = int64_t;
	static size_t compute(const Q *H_N,
	                      const Q *Y_N1,
	                            Q *Y_N2,
	                      const int N,
	                      const std::vector<std::complex<Q>> &cstl,
	                      const int bits_per_symbol,
	                      const int nbr_symbols,
	                      const Q inv_sigma2)
	{
		return kernel_demodulate_wg_complex_SIMD<Q,I,MAXI>(H_N, Y_N1, Y_N2, N, cstl, bits_per_symbol, nbr_symbols, inv_sigma2);
	}
};

template <typename B,typename R, typename Q, tools::proto_max<Q> MAX, tools::proto_max_i<Q> MAXI>
void Modem_generic_fast<B,R,Q,MAX,MAXI>
::_demodulate_wg_complex(const R *H_N, const Q *Y_N1, Q *Y_N2, const size_t frame_id)
{
	if (!std::is_same<R,Q>::value)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Type 'R' and 'Q' have to be the same.");

	if (!std::is_floating_point<Q>::value)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Type 'Q' has to be float or double.");

	const auto size_vec_loop = demodulate_wg_complex_SIMD<R, Q, MAXI>::compute(H_N,
	                                                                           Y_N1,
	                                                                           Y_N2,
	                                                                           this->N,
	                                                                           this->cstl_bis,
	                                                                           this->bits_per_symbol,
	                                                                           this->nbr_symbols,
	                                                                           (Q)this->inv_sigma2);

	for (auto n = (int)size_vec_loop; n < this->N; n++) // loop upon the LLRs
	{
		auto L0 = -std::numeric_limits<Q>::infinity();
		auto L1 = -std::numeric_limits<Q>::infinity();
		auto b  = n % this->bits_per_symbol; // bit position in the symbol
		auto k  = n / this->bits_per_symbol; // symbol position

		auto complex_Yk = std::complex<Q>(   Y_N1[2*k],    Y_N1[2*k+1]);
		auto complex_Hk = std::complex<Q>((Q)H_N [2*k], (Q)H_N [2*k+1]);

		for (auto j = 0; j < this->nbr_symbols; j++)
			if (((j>>b) & 1) == 0)
				L0 = MAX(L0, -std::norm(complex_Yk -
				                        complex_Hk * std::complex<Q>((Q)this->cstl[j].real(),
				                                                     (Q)this->cstl[j].imag())) * (Q)this->inv_sigma2);
			else
				L1 = MAX(L1, -std::norm(complex_Yk -
				                        complex_Hk * std::complex<Q>((Q)this->cstl[j].real(),
				                                                     (Q)this->cstl[j].imag())) * (Q)this->inv_sigma2);

		Y_N2[n] = (L0 - L1);
	}
}

template <typename Q, typename I, tools::proto_max_i<Q> MAXI>
size_t kernel_demodulate_real_SIMD(const Q *Y_N1,
                                         Q *Y_N2,
                                   const int N,
                                   const std::vector<std::complex<Q>> &cstl,
                                   const int bits_per_symbol,
                                   const int nbr_symbols,
                                   const Q inv_sigma2)
{
	const auto size_vec_loop = (N / mipp::N<Q>()) * mipp::N<Q>();

	const mipp::Reg<Q> reg_zero       = (Q)0;
	const mipp::Reg<Q> reg_two        = (Q)2;
	const mipp::Reg<I> regi_zero      = (I)0;
	const mipp::Reg<I> regi_one       = (I)1;
	const mipp::Reg<Q> reg_inv_sigma2 = (Q)inv_sigma2;

	I arr_b   [mipp::N<Q>()];
	Q arr_Y_re[mipp::N<Q>()];

	for (auto n = 0; n < size_vec_loop; n += mipp::N<Q>()) // loop upon the LLRs
	{
		mipp::Reg<Q> reg_L0 = -std::numeric_limits<Q>::infinity();
		mipp::Reg<Q> reg_L1 = -std::numeric_limits<Q>::infinity();

		for (auto i = 0; i < mipp::N<Q>(); i++)
			arr_b[i] = (n +i) % bits_per_symbol;
		mipp::Reg<I> regi_b = arr_b;

		for (auto i = 0; i < mipp::N<Q>(); i++)
		{
			auto k = (n +i) / bits_per_symbol;
			arr_Y_re[i] = Y_N1[2 * k];
		}
		const mipp::Reg<Q> reg_Y_re = arr_Y_re;

		mipp::Reg<I> regi_j = regi_zero;
		for (auto j = 0; j < nbr_symbols; j++)
		{
			const mipp::Reg<Q> reg_cstl_re = cstl[j].real();

			const auto reg_tmp = reg_Y_re - reg_cstl_re;
			const auto reg_res = reg_zero - reg_tmp * reg_tmp * reg_inv_sigma2;

			const auto reg_max_L0 = MAXI(reg_L0, reg_res);
			const auto reg_max_L1 = MAXI(reg_L1, reg_res);

			const auto msk_L0_or_L1 = ((regi_j >> regi_b) & regi_one) == regi_zero;

			reg_L0 = mipp::blend(reg_max_L0, reg_L0,     msk_L0_or_L1);
			reg_L1 = mipp::blend(reg_L1,     reg_max_L1, msk_L0_or_L1);

			regi_j += regi_one;
		}

		auto reg_Y_N2 = reg_L0 - reg_L1;
		reg_Y_N2.store(&Y_N2[n]);
	}

	return (size_t)size_vec_loop;
}

template <typename Q, tools::proto_max_i<Q> MAXI>
struct demodulate_real_SIMD
{
	static size_t compute(const Q *Y_N1,
	                            Q *Y_N2,
	                      const int N,
	                      const std::vector<std::complex<Q>> &cstl,
	                      const int bits_per_symbol,
	                      const int nbr_symbols,
	                      const Q inv_sigma2)
	{
		throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
	}
};

template <tools::proto_max_i<float> MAXI>
struct demodulate_real_SIMD<float, MAXI>
{
	using Q = float;
	using I = int32_t;
	static size_t compute(const Q *Y_N1,
	                            Q *Y_N2,
	                      const int N,
	                      const std::vector<std::complex<Q>> &cstl,
	                      const int bits_per_symbol,
	                      const int nbr_symbols,
	                      const Q inv_sigma2)
	{
		return kernel_demodulate_real_SIMD<Q,I,MAXI>(Y_N1, Y_N2, N, cstl, bits_per_symbol, nbr_symbols, inv_sigma2);
	}
};

template <tools::proto_max_i<double> MAXI>
struct demodulate_real_SIMD<double, MAXI>
{
	using Q = double;
	using I = int64_t;
	static size_t compute(const Q *Y_N1,
	                            Q *Y_N2,
	                      const int N,
	                      const std::vector<std::complex<Q>> &cstl,
	                      const int bits_per_symbol,
	                      const int nbr_symbols,
	                      const Q inv_sigma2)
	{
		return kernel_demodulate_real_SIMD<Q,I,MAXI>(Y_N1, Y_N2, N, cstl, bits_per_symbol, nbr_symbols, inv_sigma2);
	}
};

template <typename B, typename R, typename Q, tools::proto_max<Q> MAX, tools::proto_max_i<Q> MAXI>
void Modem_generic_fast<B,R,Q,MAX,MAXI>
::_demodulate_real(const Q *Y_N1, Q *Y_N2, const size_t frame_id)
{
	if (!std::is_same<R,Q>::value)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Type 'R' and 'Q' have to be the same.");

	if (!std::is_floating_point<Q>::value)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Type 'Q' has to be float or double.");

	const auto size_vec_loop = demodulate_real_SIMD<Q, MAXI>::compute(Y_N1,
	                                                                  Y_N2,
	                                                                  this->N,
	                                                                  this->cstl_bis,
	                                                                  this->bits_per_symbol,
	                                                                  this->nbr_symbols,
	                                                                  (Q)this->inv_sigma2);

	for (auto n = (int)size_vec_loop; n < this->N; n++) // loop upon the LLRs
	{
		auto L0 = -std::numeric_limits<Q>::infinity();
		auto L1 = -std::numeric_limits<Q>::infinity();
		auto b  = n % this->bits_per_symbol; // bit position in the symbol
		auto k  = n / this->bits_per_symbol; // symbol position

		for (auto j = 0; j < this->nbr_symbols; j++)
			if (((j>>b) & 1) == 0)
				L0 = MAX(L0, -(Y_N1[k] - (Q)this->cstl.get_real(j)) *
				              (Y_N1[k] - (Q)this->cstl.get_real(j)) * (Q)this->inv_sigma2);
			else
				L1 = MAX(L1, -(Y_N1[k] - (Q)this->cstl.get_real(j)) *
				              (Y_N1[k] - (Q)this->cstl.get_real(j)) * (Q)this->inv_sigma2);

		Y_N2[n] = (L0 - L1);
	}
}

template <typename Q, typename I, tools::proto_max_i<Q> MAXI>
size_t kernel_demodulate_wg_real_SIMD(const Q *H_N,
                                      const Q *Y_N1,
                                            Q *Y_N2,
                                      const int N,
                                      const std::vector<std::complex<Q>> &cstl,
                                      const int bits_per_symbol,
                                      const int nbr_symbols,
                                      const Q inv_sigma2)
{
	const auto size_vec_loop = (N / mipp::N<Q>()) * mipp::N<Q>();

	const mipp::Reg<Q> reg_zero       = (Q)0;
	const mipp::Reg<Q> reg_two        = (Q)2;
	const mipp::Reg<I> regi_zero      = (I)0;
	const mipp::Reg<I> regi_one       = (I)1;
	const mipp::Reg<Q> reg_inv_sigma2 = (Q)inv_sigma2;

	I arr_b   [mipp::N<Q>()];
	Q arr_Y_re[mipp::N<Q>()];
	Q arr_H_re[mipp::N<Q>()];

	for (auto n = 0; n < size_vec_loop; n += mipp::N<Q>()) // loop upon the LLRs
	{
		mipp::Reg<Q> reg_L0 = -std::numeric_limits<Q>::infinity();
		mipp::Reg<Q> reg_L1 = -std::numeric_limits<Q>::infinity();

		for (auto i = 0; i < mipp::N<Q>(); i++)
			arr_b[i] = (n +i) % bits_per_symbol;
		mipp::Reg<I> regi_b = arr_b;

		for (auto i = 0; i < mipp::N<Q>(); i++)
		{
			auto k = (n +i) / bits_per_symbol;
			arr_Y_re[i] = Y_N1[2 * k];
			arr_H_re[i] = H_N [2 * k];
		}
		const mipp::Reg<Q> reg_Y_re = arr_Y_re;
		const mipp::Reg<Q> reg_H_re = arr_H_re;

		mipp::Reg<I> regi_j = regi_zero;
		for (auto j = 0; j < nbr_symbols; j++)
		{
			const mipp::Reg<Q> reg_cstl_re = cstl[j].real();

			const auto reg_tmp = reg_Y_re - reg_H_re  * reg_cstl_re;
			const auto reg_res = reg_zero - reg_tmp * reg_tmp * reg_inv_sigma2;

			const auto reg_max_L0 = MAXI(reg_L0, reg_res);
			const auto reg_max_L1 = MAXI(reg_L1, reg_res);

			const auto msk_L0_or_L1 = ((regi_j >> regi_b) & regi_one) == regi_zero;

			reg_L0 = mipp::blend(reg_max_L0, reg_L0,     msk_L0_or_L1);
			reg_L1 = mipp::blend(reg_L1,     reg_max_L1, msk_L0_or_L1);

			regi_j += regi_one;
		}

		auto reg_Y_N2 = reg_L0 - reg_L1;
		reg_Y_N2.store(&Y_N2[n]);
	}

	return (size_t)size_vec_loop;
}

template <typename R, typename Q, tools::proto_max_i<Q> MAXI>
struct demodulate_wg_real_SIMD
{
	static size_t compute(const R *H_N,
	                      const Q *Y_N1,
	                            Q *Y_N2,
	                      const int N,
	                      const std::vector<std::complex<Q>> &cstl,
	                      const int bits_per_symbol,
	                      const int nbr_symbols,
	                      const Q inv_sigma2)
	{
		throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
	}
};

template <tools::proto_max_i<float> MAXI>
struct demodulate_wg_real_SIMD<float, float, MAXI>
{
	using Q = float;
	using I = int32_t;
	static size_t compute(const Q *H_N,
	                      const Q *Y_N1,
	                            Q *Y_N2,
	                      const int N,
	                      const std::vector<std::complex<Q>> &cstl,
	                      const int bits_per_symbol,
	                      const int nbr_symbols,
	                      const Q inv_sigma2)
	{
		return kernel_demodulate_wg_real_SIMD<Q,I,MAXI>(H_N, Y_N1, Y_N2, N, cstl, bits_per_symbol, nbr_symbols, inv_sigma2);
	}
};

template <tools::proto_max_i<double> MAXI>
struct demodulate_wg_real_SIMD<double, double, MAXI>
{
	using Q = double;
	using I = int64_t;
	static size_t compute(const Q *H_N,
	                      const Q *Y_N1,
	                            Q *Y_N2,
	                      const int N,
	                      const std::vector<std::complex<Q>> &cstl,
	                      const int bits_per_symbol,
	                      const int nbr_symbols,
	                      const Q inv_sigma2)
	{
		return kernel_demodulate_wg_real_SIMD<Q,I,MAXI>(H_N, Y_N1, Y_N2, N, cstl, bits_per_symbol, nbr_symbols, inv_sigma2);
	}
};

template <typename B, typename R, typename Q, tools::proto_max<Q> MAX, tools::proto_max_i<Q> MAXI>
void Modem_generic_fast<B,R,Q,MAX,MAXI>
::_demodulate_wg_real(const R *H_N, const Q *Y_N1, Q *Y_N2, const size_t frame_id)
{
	if (!std::is_same<R,Q>::value)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Type 'R' and 'Q' have to be the same.");

	if (!std::is_floating_point<Q>::value)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Type 'Q' has to be float or double.");

	const auto size_vec_loop = demodulate_wg_real_SIMD<R, Q, MAXI>::compute(H_N,
	                                                                        Y_N1,
	                                                                        Y_N2,
	                                                                        this->N,
	                                                                        this->cstl_bis,
	                                                                        this->bits_per_symbol,
	                                                                        this->nbr_symbols,
	                                                                        (Q)this->inv_sigma2);

	for (auto n = (int)size_vec_loop; n < this->N; n++) // loop upon the LLRs
	{
		auto L0 = -std::numeric_limits<Q>::infinity();
		auto L1 = -std::numeric_limits<Q>::infinity();
		auto b  = n % this->bits_per_symbol; // bit position in the symbol
		auto k  = n / this->bits_per_symbol; // symbol position

		for (auto j = 0; j < this->nbr_symbols; j++)
			if (((j>>b) & 1) == 0)
				L0 = MAX(L0, -(Y_N1[k] - (Q)H_N[k] * (Q)this->cstl.get_real(j)) *
				              (Y_N1[k] - (Q)H_N[k] * (Q)this->cstl.get_real(j)) * (Q)this->inv_sigma2);
			else
				L1 = MAX(L1, -(Y_N1[k] - (Q)H_N[k] * (Q)this->cstl.get_real(j)) *
				              (Y_N1[k] - (Q)H_N[k] * (Q)this->cstl.get_real(j)) * (Q)this->inv_sigma2);

		Y_N2[n] = (L0 - L1);
	}
}
}
}
