#include <cassert>

#include "Tools/Display/bash_tools.h"

#include "CRC_polynomial.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B>
CRC_polynomial<B>
::CRC_polynomial(const int K, std::string poly_key, const int n_frames, const std::string name)
: CRC<B>(K, n_frames, name), polynomial(0), buff_crc(n_frames * K)
{
	if (poly_key.empty())
	{
		std::cerr << bold_red("(EE) Please choose a CRC.") << std::endl;
		exit(EXIT_FAILURE);
	}

	if (polynomials.find(poly_key) != polynomials.end())
		polynomial = polynomials.at(poly_key);
	else
	{
		std::cerr << bold_red("(EE) CRC \"") << bold_red(poly_key) << bold_red("\" is not supported.") 
		          << std::endl;
		exit(EXIT_FAILURE);
	}

	assert(K > this->size());
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
::build(mipp::vector<B>& U_K)
{
	assert(U_K.size() >  (unsigned)(this->n_frames * this->size()));
	assert(U_K.size() == (unsigned)(this->n_frames * this->K));

	for (auto f = 0; f < this->n_frames; f++)
		this->_generate(U_K, U_K, 
		                this->K * f, 
		                this->K * f + (this->K - this->size()), 
		                this->K - this->size());
}

template <typename B>
void CRC_polynomial<B>
::_generate(const mipp::vector<B>& U_in, 
                  mipp::vector<B>& U_out, 
            const int off_in, 
            const int off_out, 
            const int loop_size)
{
	std::copy(U_in.begin() + off_in, U_in.begin() + off_in + loop_size, buff_crc.begin());
	std::fill(buff_crc.begin() + loop_size, buff_crc.begin() + loop_size + this->size(), (B)0);

	for (auto i = 0; i < loop_size; i++)
		if (buff_crc[i])
			for (auto j = 0; j <= this->size(); j++)
				if (this->polynomial[j])
					buff_crc[i+j] = !buff_crc[i+j];

	std::copy(buff_crc.begin() + loop_size, buff_crc.begin() + loop_size + this->size(), U_out.begin() + off_out);
}

template <typename B>
bool CRC_polynomial<B>
::check(const mipp::vector<B>& V_K, const int n_frames)
{
	const int real_n_frames = (n_frames != -1) ? n_frames : this->n_frames;
	assert(V_K.size() > (unsigned)(real_n_frames * this->size()));
	auto real_frame_size = (int)(V_K.size() / real_n_frames);

	auto i = 0;
	auto f = 0;
	do
	{
		this->_generate(V_K, this->buff_crc, 
		                real_frame_size * f, 
		                real_frame_size * (f +1) - this->size(),
		                real_frame_size - this->size());

		i = 0;
		auto off1 = real_frame_size * (f +1) - this->size();
		auto off2 = real_frame_size * (f +1) - this->size();
		while ((i < this->size()) &&
		       // because the position of the bit in a variable can vary,
		       // the idea is to test: (this->buff_crc[off1 +i] == V_K[off2 +i])
		       ((this->buff_crc[off1 +i] || !V_K[off2 +i]) && (!this->buff_crc[off1 +i] || V_K[off2 +i])))
			i++;
		f++;
	}
	while ((f < real_n_frames) && (i == this->size()));

	return (f == real_n_frames) && (i == this->size());
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class CRC_polynomial<B_8>;
template class CRC_polynomial<B_16>;
template class CRC_polynomial<B_32>;
template class CRC_polynomial<B_64>;
#else
template class CRC_polynomial<B>;
#endif
// ==================================================================================== explicit template instantiation
