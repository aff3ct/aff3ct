#include <algorithm>
#include <sstream>
#include <string>

#include "Tools/Exception/exception.hpp"
#include "Tools/Algo/Bit_packer.hpp"
#include "Tools/Display/rang_format/rang_format.h"
#include "Module/CRC/Polynomial/CRC_polynomial.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B>
CRC_polynomial<B>
::CRC_polynomial(const int K, const std::string &poly_key, const int size)
: CRC<B>(K, size ? size : CRC_polynomial<B>::get_size(CRC_polynomial<B>::get_name(poly_key))),
  polynomial       (0                                     ),
  polynomial_packed(CRC_polynomial<B>::get_value(poly_key)),
  buff_crc         (0                                     )
{
	const std::string name = "CRC_polynomial";
	this->set_name(name);

	if (poly_key.empty())
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "'poly_key' can't be empty, choose a CRC.");

	if (!polynomial_packed)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "CRC '" + poly_key + "' is not supported.");

	auto crc_name = CRC_polynomial<B>::get_name(poly_key);
	if (this->size == 0 && crc_name.empty())
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Please specify the CRC 'size'.");

	if (!crc_name.empty() && CRC_polynomial<B>::get_size(crc_name) != this->size)
		std::clog << rang::tag::warning << "You specified \"" << this->size
		                                   << " bits\" for your CRC size but the database advise you to use \""
		                                   << std::get<1>(known_polynomials.at(crc_name))
		                                   << " bits\", are you sure?" << std::endl;

	polynomial.push_back(1);
	for (auto i = 0; i < this->size; i++)
		polynomial.push_back((polynomial_packed >> ((this->size -1) -i)) & 1);

	buff_crc.resize((this->K + this->size) * this->n_frames);
}

template <typename B>
CRC_polynomial<B>* CRC_polynomial<B>
::clone() const
{
	auto m = new CRC_polynomial(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B>
int CRC_polynomial<B>
::get_size(const std::string &poly_key)
{
	if (known_polynomials.find(poly_key) != known_polynomials.end())
		return std::get<1>(known_polynomials.at(poly_key));
	else
	{
		auto size = 0;
		auto name = CRC_polynomial<B>::get_name(poly_key);

		if (!name.empty())
			size = CRC_polynomial<B>::get_size(name);

		return size;
	}
}

template <typename B>
std::string CRC_polynomial<B>
::get_name(const std::string &poly_key)
{
	if (known_polynomials.find(poly_key) != known_polynomials.end())
		return poly_key;
	else
	{
		auto value = CRC_polynomial<B>::get_value(poly_key);
		std::string name = "";

		for (auto it = known_polynomials.begin(); it != known_polynomials.end(); ++it)
			if (std::get<0>(it->second) == value)
			{
				name = it->first;
				break;
			}

		return name;
	}
}

template <typename B>
unsigned CRC_polynomial<B>
::get_value(const std::string &poly_key)
{
	if (known_polynomials.find(poly_key) != known_polynomials.end())
		return std::get<0>(known_polynomials.at(poly_key));
	else if(poly_key.length() > 2 && poly_key[0] == '0' && poly_key[1] == 'x')
		return (unsigned)std::stoul(poly_key, 0, 16);
	else
		return 0;
}

template <typename B>
void CRC_polynomial<B>
::_build(const B *U_K1, B *U_K2, const size_t frame_id)
{
	std::copy(U_K1, U_K1 + this->K, U_K2);
	this->_generate(U_K1, U_K2, 0, this->K, this->K);
}

template <typename B>
void CRC_polynomial<B>
::_generate(const B *U_in,
                  B *U_out,
            const int off_in,
            const int off_out,
            const int loop_size)
{
	std::copy(U_in + off_in, U_in + off_in + loop_size, buff_crc.begin());
	std::fill(buff_crc.begin() + loop_size, buff_crc.begin() + loop_size + this->size, (B)0);

	for (auto i = 0; i < loop_size; i++)
		if (buff_crc[i])
			for (auto j = 0; j <= this->size; j++)
				if (this->polynomial[j])
					buff_crc[i+j] = !buff_crc[i+j];

	if (U_out != buff_crc.data())
		std::copy(buff_crc.begin() + loop_size, buff_crc.begin() + loop_size + this->size, U_out + off_out);
}

template <typename B>
void CRC_polynomial<B>
::_extract(const B *V_K1, B *V_K2, const size_t frame_id)
{
	std::copy(V_K1, V_K1 + this->K, V_K2);
}

template <typename B>
bool CRC_polynomial<B>
::_check(const B *V_K, const size_t frame_id)
{
	this->_generate(V_K, this->buff_crc.data(), 0, this->K, this->K);

	auto i = 0;
	auto off = this->K;
	while ((i < this->size) &&
	       // because the position of the bit in a variable can vary,
	       // the idea is to test: (this->buff_crc[off +i] == V_K[off +i])
	       ((this->buff_crc[off +i] || !V_K[off +i]) && (!this->buff_crc[off +i] || V_K[off +i])))
		i++;

	return (i == this->size);
}

template <typename B>
bool CRC_polynomial<B>
::_check_packed(const B *V_K, const size_t frame_id)
{
	std::vector<B> V_K_unpack(this->K + this->size);
	std::copy(V_K, V_K + this->K + this->size, V_K_unpack.begin());
	tools::Bit_packer::unpack(V_K_unpack, this->K + this->size);
	return _check(V_K_unpack.data(), frame_id);
}

template <typename B>
void CRC_polynomial<B>
::set_n_frames(const size_t n_frames)
{
	const auto old_n_frames = this->get_n_frames();
	if (old_n_frames != n_frames)
	{
		CRC<B>::set_n_frames(n_frames);

		const auto old_buff_crc_size = this->buff_crc.size();
		const auto new_buff_crc_size = (old_buff_crc_size / old_n_frames) * n_frames;
		this->buff_crc.resize(new_buff_crc_size);
	}
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::CRC_polynomial<B_8>;
template class aff3ct::module::CRC_polynomial<B_16>;
template class aff3ct::module::CRC_polynomial<B_32>;
template class aff3ct::module::CRC_polynomial<B_64>;
#else
template class aff3ct::module::CRC_polynomial<B>;
#endif
// ==================================================================================== explicit template instantiation
