#include <stdexcept>

#include "CRC_polynomial.hpp"

using namespace aff3ct::module;

template <typename B>
CRC_polynomial<B>
::CRC_polynomial(const int K, std::string poly_key, const int n_frames, const std::string name)
: CRC<B>(K, n_frames, name), polynomial(0), buff_crc(n_frames * K)
{
	if (poly_key.empty())
		throw std::invalid_argument("aff3ct::module::CRC_polynomial: \"poly_key\" can't be empty, choose a CRC.");

	if (polynomials.find(poly_key) != polynomials.end())
		polynomial = polynomials.at(poly_key);
	else
	{
		throw std::invalid_argument("aff3ct::module::CRC_polynomial: \"poly_key\" = \"" + poly_key +
		                            "\" is not supported.");
	}

	if (K <= this->size())
		throw std::invalid_argument("aff3ct::module::CRC_polynomial: \"K\" has to be greater than \"this->size()\".");
}

template <typename B>
int CRC_polynomial<B>
::size(std::string poly_key)
{
	if (polynomials.find(poly_key) != polynomials.end())
		return (int)(polynomials.at(poly_key).size() == 0 ? 0 : polynomials.at(poly_key).size() -1); 
	else
		return 0;
}

template <typename B>
int CRC_polynomial<B>
::size() const
{
	// the number of generated crc bits is the size of the polynomial minus 1
	return (int)(polynomial.size() == 0 ? 0 : polynomial.size() -1);
}

template <typename B>
void CRC_polynomial<B>
::_build(B *U_K)
{
	this->_generate(U_K, U_K, 0, this->K - this->size(), this->K - this->size());
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
	std::fill(buff_crc.begin() + loop_size, buff_crc.begin() + loop_size + this->size(), (B)0);

	for (auto i = 0; i < loop_size; i++)
		if (buff_crc[i])
			for (auto j = 0; j <= this->size(); j++)
				if (this->polynomial[j])
					buff_crc[i+j] = !buff_crc[i+j];

	std::copy(buff_crc.begin() + loop_size, buff_crc.begin() + loop_size + this->size(), U_out + off_out);
}

template <typename B>
bool CRC_polynomial<B>
::_check(const B *V_K)
{
	this->_generate(V_K, this->buff_crc.data(), 0, this->K - this->size(), this->K - this->size());

	auto i = 0;
	auto off = this->K - this->size();
	while ((i < this->size()) &&
	       // because the position of the bit in a variable can vary,
	       // the idea is to test: (this->buff_crc[off +i] == V_K[off +i])
	       ((this->buff_crc[off +i] || !V_K[off +i]) && (!this->buff_crc[off +i] || V_K[off +i])))
		i++;

	return (i == this->size());
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
