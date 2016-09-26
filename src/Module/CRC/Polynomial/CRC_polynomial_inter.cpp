#include <cassert>

#include "Tools/bash_tools.h"
#include "Tools/MIPP/mipp.h"

#include "CRC_polynomial_inter.hpp"

template <typename B>
CRC_polynomial_inter<B>
::CRC_polynomial_inter(const int K, std::string poly_key, const int n_frames, const std::string name)
: CRC_polynomial<B>(K, poly_key, n_frames, name)
{
	assert(mipp::nElmtsPerRegister<B>() == n_frames);
}

template <typename B>
bool CRC_polynomial_inter<B>
::check(const mipp::vector<B>& V_K)
{
	assert(V_K.size() > (unsigned)(this->n_frames * this->size()));

	auto real_frame_size = V_K.size() / this->n_frames;
	this->_generate_INTER(V_K, this->buff_crc, 
	                      0, 
	                      this->buff_crc.size() - (this->n_frames * this->size()),
	                      this->n_frames * (real_frame_size - this->size()));

	auto i = 0;
	const auto off1 = this->buff_crc.size() - (this->n_frames * this->size());
	const auto off2 = V_K.size()            - (this->n_frames * this->size());
	const auto total_crc_size = this->n_frames * this->size();
	while ((i < total_crc_size) && (this->buff_crc[off1 +i] == V_K[off2 +i]))
		i++;

	return (i == total_crc_size);
}

template <typename B>
void CRC_polynomial_inter<B>
::_generate_INTER(const mipp::vector<B>& U_in, 
                        mipp::vector<B>& U_out, 
                  const int off_in, 
                  const int off_out, 
                  const int loop_size)
{
	assert(this->n_frames == mipp::nElmtsPerRegister<B>());
	assert(loop_size % this->n_frames == 0);

	std::copy(U_in.begin() + off_in, U_in.begin() + off_in + loop_size, this->buff_crc.begin());
	std::fill(this->buff_crc.end() - (this->n_frames * this->size()), this->buff_crc.end(), (B)0);

	const auto r_zero = mipp::set0<B>();
	const auto crc_size = this->size();
	for (auto i = 0; i < loop_size; i += this->n_frames)
	{
		auto r_buff_crc = mipp::load <B>(&this->buff_crc[i]);
		auto r_mask     = mipp::cmpeq<B>(r_buff_crc, r_zero);

		for (auto j = 0; j <= crc_size; j++)
		{
			r_buff_crc = mipp::load  <B>(&this->buff_crc[i + this->n_frames * j]);
			auto r_sav = mipp::andb  <B>(r_mask, r_buff_crc);
			r_buff_crc = mipp::xorb  <B>(r_buff_crc, mipp::set1<B>(this->polynomial[j]));
			r_buff_crc = mipp::andnb <B>(r_mask, r_buff_crc);
			r_buff_crc = mipp::orb   <B>(r_sav, r_buff_crc);
			mipp::store<B>(&this->buff_crc[i + this->n_frames * j], r_buff_crc);
		}
	}

	std::copy(this->buff_crc.end() - this->n_frames * this->size(), this->buff_crc.end(), U_out.begin() + off_out);
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class CRC_polynomial_inter<B_8>;
template class CRC_polynomial_inter<B_16>;
template class CRC_polynomial_inter<B_32>;
template class CRC_polynomial_inter<B_64>;
#else
template class CRC_polynomial_inter<B>;
#endif
// ==================================================================================== explicit template instantiation
