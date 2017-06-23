#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Tools/Algo/Bit_packer.hpp"
#include "Tools/Display/bash_tools.h"

#include "CRC_polynomial.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B>
CRC_polynomial<B>
::CRC_polynomial(const int K, std::string poly_key, const int size, const int n_frames, const std::string name)
: CRC<B>(K, n_frames, name),
  polynomial       (0                                 ),
  polynomial_packed(CRC_polynomial<B>::value(poly_key)),
  poly_size        (0                                 ),
  buff_crc         (0                                 )
{
	if (poly_key.empty())
		throw invalid_argument(__FILE__, __LINE__, __func__, "'poly_key' can't be empty, choose a CRC.");

	if (!polynomial_packed)
		throw invalid_argument(__FILE__, __LINE__, __func__, "CRC '" + poly_key + "' is not supported.");

	auto crc_name = CRC_polynomial<B>::name(poly_key);
	if (size == 0 && crc_name.empty())
		throw invalid_argument(__FILE__, __LINE__, __func__, "Please specify the CRC 'size'.");

	if (size)
		poly_size = size;
	else
		poly_size = CRC_polynomial<B>::size(crc_name);

	if (!crc_name.empty() && CRC_polynomial<B>::size(crc_name) != poly_size)
		std::clog << format_warning("You specified \"" + std::to_string(poly_size)
		                            + " bits\" for your CRC size but the database advise you to use \""
		                            + std::to_string(std::get<1>(known_polynomials.at(crc_name)))
		                            + " bits\", are you sure?")
		          << std::endl;

	polynomial.push_back(1);
	for (auto i = 0; i < poly_size; i++)
		polynomial.push_back((polynomial_packed >> ((poly_size -1) -i)) & 1);

	buff_crc.resize((this->K + this->get_size()) * this->n_frames);
}

template <typename B>
int CRC_polynomial<B>
::size(std::string poly_key)
{
	if (known_polynomials.find(poly_key) != known_polynomials.end())
		return std::get<1>(known_polynomials.at(poly_key));
	else
	{
		auto size = 0;
		auto name = CRC_polynomial<B>::name(poly_key);

		if (!name.empty())
			size = CRC_polynomial<B>::size(name);

		return size;
	}
}

template <typename B>
std::string CRC_polynomial<B>
::name(std::string poly_key)
{
	if (known_polynomials.find(poly_key) != known_polynomials.end())
		return poly_key;
	else
	{
		auto value = CRC_polynomial<B>::value(poly_key);
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
::value(std::string poly_key)
{
	if (known_polynomials.find(poly_key) != known_polynomials.end())
		return std::get<0>(known_polynomials.at(poly_key));
	else if(poly_key.length() > 2 && poly_key[0] == '0' && poly_key[1] == 'x')
		return (unsigned)std::stoul(poly_key, 0, 16);
	else
		return 0;
}

template <typename B>
int CRC_polynomial<B>
::get_size() const
{
	return this->poly_size;
}

template <typename B>
void CRC_polynomial<B>
::_build(const B *U_K1, B *U_K2, const int frame_id)
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
	std::fill(buff_crc.begin() + loop_size, buff_crc.begin() + loop_size + this->get_size(), (B)0);

	for (auto i = 0; i < loop_size; i++)
		if (buff_crc[i])
			for (auto j = 0; j <= this->get_size(); j++)
				if (this->polynomial[j])
					buff_crc[i+j] = !buff_crc[i+j];

	if (U_out != buff_crc.data())
		std::copy(buff_crc.begin() + loop_size, buff_crc.begin() + loop_size + this->get_size(), U_out + off_out);
}

template <typename B>
void CRC_polynomial<B>
::_extract(const B *V_K1, B *V_K2, const int frame_id)
{
	std::copy(V_K1, V_K1 + this->K, V_K2);
}

template <typename B>
bool CRC_polynomial<B>
::_check(const B *V_K, const int frame_id)
{
	this->_generate(V_K, this->buff_crc.data(), 0, this->K, this->K);

	auto i = 0;
	auto off = this->K;
	while ((i < this->get_size()) &&
	       // because the position of the bit in a variable can vary,
	       // the idea is to test: (this->buff_crc[off +i] == V_K[off +i])
	       ((this->buff_crc[off +i] || !V_K[off +i]) && (!this->buff_crc[off +i] || V_K[off +i])))
		i++;

	return (i == this->get_size());
}

template <typename B>
bool CRC_polynomial<B>
::_check_packed(const B *V_K, const int frame_id)
{
	mipp::vector<B> V_K_unpack(this->K + this->get_size());
	std::copy(V_K, V_K + this->K + this->get_size(), V_K_unpack.begin());
	Bit_packer<B>::unpack(V_K_unpack, this->K + this->get_size());
	return _check(V_K_unpack.data(), frame_id);
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::CRC_polynomial<B_8>;
template class aff3ct::module::CRC_polynomial<B_16>;
template class aff3ct::module::CRC_polynomial<B_32>;
template class aff3ct::module::CRC_polynomial<B_64>;
#else
template class aff3ct::module::CRC_polynomial<B>;
#endif
// ==================================================================================== explicit template instantiation
