#include <algorithm>
#include <sstream>
#include <string>
#include <mipp.h>

#include "Tools/Exception/exception.hpp"
#include "Module/CRC/Polynomial/CRC_polynomial_inter.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B>
CRC_polynomial_inter<B>
::CRC_polynomial_inter(const int K, const std::string &poly_key, const int size)
: CRC_polynomial<B>(K, poly_key, size), buff_crc_inter((this->K + this->size) * mipp::nElReg<B>())
{
	const std::string name = "CRC_polynomial_inter";
	this->set_name(name);
	this->set_n_frames_per_wave(mipp::nElReg<B>());
}

template <typename B>
CRC_polynomial_inter<B>* CRC_polynomial_inter<B>
::clone() const
{
	auto m = new CRC_polynomial_inter(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B>
void CRC_polynomial_inter<B>
::_build(const B *U_K1, B *U_K2, const size_t frame_id)
{
	for (size_t f = 0; f < this->get_n_frames_per_wave(); f++)
		CRC_polynomial<B>::_build(U_K1 + f *  this->K,
			                      U_K2 + f * (this->K + this->size),
			                      frame_id + f);
}

template <typename B>
bool CRC_polynomial_inter<B>
::_check(const B *V_K, const size_t frame_id)
{
	this->_generate_INTER(V_K, this->buff_crc_inter.data(),
	                      0,
	                      this->K * this->get_n_frames_per_wave(),
	                      this->K * this->get_n_frames_per_wave(),
	                      this->get_n_frames_per_wave());

	size_t i = 0;
	const auto off = this->K * this->get_n_frames_per_wave();
	const auto total_crc_size = this->get_n_frames_per_wave() * this->size;
	while ((i < total_crc_size) && (this->buff_crc_inter[off +i] == V_K[off +i]))
		i++;

	return (i == total_crc_size);
}

template <typename B>
bool CRC_polynomial_inter<B>
::_check_packed(const B *V_K, const size_t frame_id)
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename B>
void CRC_polynomial_inter<B>
::_extract(const B *V_K1, B *V_K2, const size_t frame_id)
{
	for (size_t f = 0; f < this->get_n_frames_per_wave(); f++)
		CRC_polynomial<B>::_extract(V_K1 + f * (this->K + this->size),
			                        V_K2 + f *  this->K,
			                        frame_id + f);
}

template <typename B>
void CRC_polynomial_inter<B>
::_generate_INTER(const B *U_in,
                        B *U_out,
                  const int off_in,
                  const int off_out,
                  const int loop_size,
                  const int n_frames)
{
	std::copy(U_in + off_in, U_in + off_in + loop_size, this->buff_crc_inter.begin());
	std::fill(this->buff_crc_inter.end() - (n_frames * this->size), this->buff_crc_inter.end(), (B)0);

	const auto r_zero = mipp::set0<B>();
	const auto crc_size = this->size;
	for (auto i = 0; i < loop_size; i += n_frames)
	{
		auto r_buff_crc = mipp::loadu<B>(&this->buff_crc_inter[i]);
		auto r_mask     = mipp::toreg<mipp::N<B>()>(mipp::cmpeq<B>(r_buff_crc, r_zero));

		for (auto j = 0; j <= crc_size; j++)
		{
			r_buff_crc = mipp::loadu<B>(&this->buff_crc_inter[i + n_frames * j]);
			auto r_sav = mipp::andb <B>(r_mask, r_buff_crc);
			r_buff_crc = mipp::xorb <B>(r_buff_crc, mipp::set1<B>(this->polynomial[j]));
			r_buff_crc = mipp::andnb<B>(r_mask, r_buff_crc);
			r_buff_crc = mipp::orb  <B>(r_sav, r_buff_crc);
			mipp::storeu<B>(&this->buff_crc_inter[i + n_frames * j], r_buff_crc);
		}
	}

	std::copy(this->buff_crc_inter.end() - n_frames * this->size, this->buff_crc_inter.end(), U_out + off_out);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::CRC_polynomial_inter<B_8>;
template class aff3ct::module::CRC_polynomial_inter<B_16>;
template class aff3ct::module::CRC_polynomial_inter<B_32>;
template class aff3ct::module::CRC_polynomial_inter<B_64>;
#else
template class aff3ct::module::CRC_polynomial_inter<B>;
#endif
// ==================================================================================== explicit template instantiation
