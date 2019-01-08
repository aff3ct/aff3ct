#include <sstream>
#include <mipp.h>

#include "Tools/Exception/exception.hpp"

#include "CRC_polynomial_inter.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B>
CRC_polynomial_inter<B>
::CRC_polynomial_inter(const int K, std::string poly_key, const int size, const int n_frames)
: CRC_polynomial<B>(K, poly_key, size, n_frames)
{
	const std::string name = "CRC_polynomial_inter";
	this->set_name(name);

	this->buff_crc.resize((this->K + this->size) * mipp::nElReg<B>());
}

template <typename B>
bool CRC_polynomial_inter<B>
::check(const B *V_K, const int n_frames, const int frame_id)
{
	const int real_n_frames = (n_frames != -1) ? n_frames : this->n_frames;

	if (real_n_frames != mipp::nElReg<B>())
	{
		std::stringstream message;
		message << "'real_n_frames' has to be equal to 'mipp::nElReg<B>()' ('real_n_frames' = " << real_n_frames
		        << ", 'mipp::nElReg<B>()' = " << mipp::nElReg<B>() << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (frame_id != -1)
	{
		std::stringstream message;
		message << "'frame_id' has to be equal to -1 ('frame_id' = " << frame_id << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	this->_generate_INTER(V_K, this->buff_crc.data(),
	                      0,
	                      this->K * real_n_frames,
	                      this->K * real_n_frames,
	                      real_n_frames);

	auto i = 0;
	const auto off = this->K * real_n_frames;
	const auto total_crc_size = real_n_frames * this->size;
	while ((i < total_crc_size) && (this->buff_crc[off +i] == V_K[off +i]))
		i++;

	return (i == total_crc_size);
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
	std::copy(U_in + off_in, U_in + off_in + loop_size, this->buff_crc.begin());
	std::fill(this->buff_crc.end() - (n_frames * this->size), this->buff_crc.end(), (B)0);

	const auto r_zero = mipp::set0<B>();
	const auto crc_size = this->size;
	for (auto i = 0; i < loop_size; i += n_frames)
	{
		auto r_buff_crc = mipp::loadu<B>(&this->buff_crc[i]);
		auto r_mask     = mipp::toreg<mipp::N<B>()>(mipp::cmpeq<B>(r_buff_crc, r_zero));

		for (auto j = 0; j <= crc_size; j++)
		{
			r_buff_crc = mipp::loadu<B>(&this->buff_crc[i + n_frames * j]);
			auto r_sav = mipp::andb <B>(r_mask, r_buff_crc);
			r_buff_crc = mipp::xorb <B>(r_buff_crc, mipp::set1<B>(this->polynomial[j]));
			r_buff_crc = mipp::andnb<B>(r_mask, r_buff_crc);
			r_buff_crc = mipp::orb  <B>(r_sav, r_buff_crc);
			mipp::storeu<B>(&this->buff_crc[i + n_frames * j], r_buff_crc);
		}
	}

	std::copy(this->buff_crc.end() - n_frames * this->size, this->buff_crc.end(), U_out + off_out);
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
