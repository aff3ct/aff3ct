#include <cassert>

#include "Tools/Display/bash_tools.h"
#include "Tools/Algo/Bit_packer.hpp"

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
  buff_crc         (n_frames * K                      )
{
	if (poly_key.empty())
	{
		std::cerr << bold_red("(EE) Please choose a CRC.") << std::endl;
		std::exit(EXIT_FAILURE);
	}

	if (!polynomial_packed)
	{
		std::cerr << bold_red("(EE) CRC \"") << bold_red(poly_key) << bold_red("\" is not supported.") << std::endl;
		std::exit(EXIT_FAILURE);
	}


	auto crc_name = CRC_polynomial<B>::name(poly_key);
	if (size == 0 && crc_name.empty())
	{
		std::cerr << bold_red("(EE) You have to specify a size with the \"--crc-size\" option.") << std::endl;
		std::exit(EXIT_FAILURE);
	}

	if (size)
		poly_size = size;
	else
		poly_size = CRC_polynomial<B>::size(crc_name);

	if (!crc_name.empty() && CRC_polynomial<B>::size(crc_name) != poly_size)
	{
		std::clog << bold_yellow("(WW) You specified \"")
				  << bold_yellow(std::to_string(poly_size))
				  << bold_yellow(" bits\" for your CRC size but the database advise you to use \"")
				  << bold_yellow(std::to_string(std::get<1>(known_polynomials.at(crc_name))))
				  << bold_yellow(" bits\", are you sure?")
				  << std::endl;
	}

	polynomial.push_back(1);
	for (auto i = 0; i < poly_size; i++)
		polynomial.push_back((polynomial_packed >> ((poly_size -1) -i)) & 1);

	assert(K > this->size());
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
::size() const
{
	return this->poly_size;
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

	if (U_out.data() != buff_crc.data())
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

template <typename B>
bool CRC_polynomial<B>
::check_packed(const mipp::vector<B>& V_K, const int n_frames)
{
	const int real_n_frames = (n_frames != -1) ? n_frames : this->n_frames;

	mipp::vector<B> V_K_unpack = V_K;
	Bit_packer<B>::unpack(V_K_unpack, real_n_frames);
	return check(V_K_unpack, n_frames);
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
