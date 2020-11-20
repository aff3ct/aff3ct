#include <sstream>
#include <algorithm>

#include "Tools/Exception/exception.hpp"
#include "Module/Encoder/RSC_DB/Encoder_RSC_DB.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B>
Encoder_RSC_DB<B>
::Encoder_RSC_DB(const int& K, const int& N, const std::string standard, const bool buffered_encoding)
: Encoder<B>       (K, N),
  n_ff             (standard == "DVB-RCS1" ? 3 : 4),
  n_states         (1 << n_ff),
  buffered_encoding(buffered_encoding),
  next_state       (4, std::vector<int>(n_states, 0)),
  out_parity       (4, std::vector<int>(n_states, 0))
{
	const std::string name = "Encoder_RSC_DB";
	this->set_name(name);

	if (N !=  2 * K)
	{
		std::stringstream message;
		message << "'N' has to be equal to 2 * 'K' ('N' = " << N << ", 'K' = " << K << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	// poly: A,B,Y,W,Feed : all in binary
	if (standard == "DVB-RCS1")
	{
		poly = {{1,0,0}  , // A
		        {1,1,1}  , // B
		        {1,0,1,1}, // Y
		        {1,0,0,1}, // W
		        {1,0,1} }; // Feedback
		circ_states = {{0,6,4,2,7,1,3,5},
		               {0,3,7,4,5,6,2,1},
		               {0,5,3,6,2,7,1,4},
		               {0,4,1,5,6,2,7,3},
		               {0,2,5,7,1,3,4,6},
		               {0,7,6,1,3,4,5,2}};
	}
	else if (standard == "DVB-RCS2")
	{
		poly = {{1,0,0,0}  ,
		        {1,1,0,1}  ,
		        {1,1,1,0,1},
		        {1,0,1,1,1},
		        {0,0,1,1} };

		circ_states = {{0,14,3,13,7,9,4,10,15,1,12,2,8,6,11,5},
		               {0,11,13,6,10,1,7,12,5,14,8,3,15,4,2,9},
		               {0,8,9,1,2,10,11,3,4,12,13,5,6,14,15,7},
		               {0,3,4,7,8,11,12,15,1,2,5,6,9,10,13,14},
		               {0,12,5,9,11,7,14,2,6,10,3,15,13,1,8,4},
		               {0,4,12,8,9,13,5,1,2,6,14,10,11,15,7,3},
		               {0,6,10,12,5,3,15,9,11,13,1,7,14,8,4,2},
		               {0,7,8,15,1,6,9,14,3,4,11,12,2,5,10,13},
		               {0,5,14,11,13,8,3,6,10,15,4,1,7,2,9,12},
		               {0,13,7,10,15,2,8,5,14,3,9,4,1,12,6,11},
		               {0,2,6,4,12,14,10,8,9,11,15,13,5,7,3,1},
		               {0,9,11,2,6,15,13,4,12,5,7,14,10,3,1,8},
		               {0,10,15,5,14,4,1,11,13,7,2,8,3,9,12,6},
		               {0,15,1,14,3,12,2,13,7,8,6,9,4,11,5,10}};
	}
	else
	{
		std::stringstream message;
		message << "'standard' has to be equal to DVB-RCS1 or DVB-RCS2 ('standard' = " << standard << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if ((int)poly.size() != 5)
	{
		std::stringstream message;
		message << "'poly.size()' has to be equal to 5 ('poly.size()' = " << poly.size() << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if ((int)poly[0].size() != n_ff)
	{
		std::stringstream message;
		message << "'poly[0].size()' has to be equal to 'n_ff' ('poly[0].size()' = " << poly[0].size()
		        << ", 'n_ff' = " << n_ff << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if ((int)poly[1].size() != n_ff)
	{
		std::stringstream message;
		message << "'poly[1].size()' has to be equal to 'n_ff' ('poly[1].size()' = " << poly[1].size()
		        << ", 'n_ff' = " << n_ff << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if ((int)poly[2].size() != n_ff+1)
	{
		std::stringstream message;
		message << "'poly[2].size()' has to be equal to 'n_ff+1' ('poly[2].size()' = " << poly[2].size()
		        << ", 'n_ff+1' = " << (n_ff+1) << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if ((int)poly[3].size() != n_ff+1)
	{
		std::stringstream message;
		message << "'poly[3].size()' has to be equal to 'n_ff+1' ('poly[3].size()' = " << poly[3].size()
		        << ", 'n_ff+1' = " << (n_ff+1) << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if ((int)poly[4].size() != n_ff)
	{
		std::stringstream message;
		message << "'poly[4].size()' has to be equal to 'n_ff' ('poly[4].size()' = " << poly[4].size()
		        << ", 'n_ff' = " << n_ff << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (K % 2)
	{
		std::stringstream message;
		message << "'K' has to be even ('K' = " << K << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	int p0, p1, p2, p3;
	for (auto s = 0; s < n_states; s++)
	{
		int s0 = calc_state_trans(s, 0, p0);
		next_state[0][s] = s0;
		out_parity[0][s] = p0;

		int s1 = calc_state_trans(s, 1, p1);
		next_state[1][s] = s1;
		out_parity[1][s] = p1;

		int s2 = calc_state_trans(s, 2, p2);
		next_state[2][s] = s2;
		out_parity[2][s] = p2;

		int s3 = calc_state_trans(s, 3, p3);
		next_state[3][s] = s3;
		out_parity[3][s] = p3;
	}

	if (!buffered_encoding)
		for (auto k = 0; k < this->K; k += 2)
		{
			this->info_bits_pos[k   ] = 2 * k;
			this->info_bits_pos[k +1] = 2 * k +1;
		}
}

template <typename B>
Encoder_RSC_DB<B>* Encoder_RSC_DB<B>
::clone() const
{
	auto m = new Encoder_RSC_DB(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B>
int Encoder_RSC_DB<B>
::calc_state_trans(const int in_state, const int in, int& par)
{
	std::vector<int> FF (n_ff, 0);

	for(auto i = 0; i < n_ff; i++)
		FF[0]^= ( ((in_state >> i) &1) * poly[4][n_ff - 1 - i] );
	FF[0]^= poly[0][0] * ((in >> 1) &1);
	FF[0]^= poly[1][0] * ((in     ) &1);

	for (auto i = 1; i < n_ff; i++)
	{
		FF[i] = (in_state >> (n_ff - i)) &1;
		FF[i]^= poly[0][i] * ((in >> 1) &1);
		FF[i]^= poly[1][i] * ((in     ) &1);
	}

	int parY = FF[0] * poly[2][0];
	for (auto i = 1; i < n_ff + 1; i++)
		parY ^= ((in_state >> (n_ff - i)) &1) * poly[2][i];

	int parW = FF[0] * poly[3][0];
	for (auto i = 1; i < n_ff + 1; i++)
		parW ^= ((in_state >> (n_ff - i)) &1) * poly[3][i];

	par = (parY << 1) | parW;

	int next_state = 0;
	for (auto i = 0; i < n_ff; i++)
		next_state = (next_state << 1) | FF[i];

	return next_state;
}

template <typename B>
void Encoder_RSC_DB<B>
::__encode_from_state(const B* U_K, B* X_N, const bool only_parity, const int init_state, int& end_state)
{
	auto j = 0; // cur offset in X_N buffer
	auto state = init_state;
	for (auto i = 0; i < this->K; i+=2)
	{
		auto in = U_K[i]*2 + U_K[i+1];
		auto parity = out_parity[in][state];
		state = next_state[in][state];
		if(!only_parity)
		{
			X_N[j++] = U_K[i];
			X_N[j++] = U_K[i+1];
		}
		X_N[j++] = (parity>>1) &1;
		X_N[j++] = (parity   ) &1;
	}
	end_state = state;
}

template <typename B>
void Encoder_RSC_DB<B>
::__pre_encode(const B* U_K, int& end_state)
{
	auto state = 0;
	for (auto i = 0; i < this->K; i+=2)
	{
		auto in = U_K[i]*2 + U_K[i+1];
		state = next_state[in][state];
	}
	end_state = state;
}

template <typename B>
void Encoder_RSC_DB<B>
::_encode(const B *U_K, B *X_N, const size_t frame_id)
{
	int circ_state, end_state;
	__pre_encode(U_K, circ_state);
	int init_state = circ_states[( (this->K/2) % (n_states-1) ) -1][circ_state];
	if (this->buffered_encoding)
	{
		std::copy(U_K, U_K + this->K, X_N);
		__encode_from_state(U_K, X_N + this->K, true, init_state, end_state);
	}
	else
		__encode_from_state(U_K, X_N, false, init_state, end_state);

	if (init_state != end_state)
	{
		std::stringstream message;
		message << "'end_state' should match 'init_state' ('end_state' = " << end_state
		        << ", 'init_state' = " << init_state << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename B>
std::vector<std::vector<int>> Encoder_RSC_DB<B>
::get_trellis()
{
	// trellis : next_state, rev_state, out_par_y, out_par_w
	std::vector<std::vector<int>> trellis(4, std::vector<int>(n_states*4));

	int par;
	for (auto state = 0; state < n_states; state++)
	{
		for (auto symb = 0; symb < 4; symb++)
		{
			int s_prim = calc_state_trans(state, symb, par);
			trellis[0][state*4  + symb] = s_prim;
			trellis[1][s_prim*4 + symb] = state;
			trellis[2][state*4  + symb] = -(((par >> 1) &1) *2) +1;
			trellis[3][state*4  + symb] = -(( par       &1) *2) +1;
		}
	}

	return trellis;
}

template <typename B>
bool Encoder_RSC_DB<B>
::is_codeword(const B *X_N)
{
	int circ_state = 0;
	if (this->buffered_encoding)
		for (auto i = 0; i < this->K; i+=2)
			circ_state = next_state[X_N[i]*2 + X_N[i+1]][circ_state];
	else
		for (auto i = 0; i < this->K; i+=2)
			circ_state = next_state[X_N[i*2]*2 + X_N[i*2+1]][circ_state];

	auto init_state = circ_states[( (this->K/2) % (n_states-1) ) -1][circ_state];
	auto state = init_state;
	if (this->buffered_encoding)
	{
		for (auto i = 0; i < this->K; i+=2)
		{
			auto in = X_N[i+0]*2 + X_N[i+1];
			auto parity = out_parity[in][state];
			state = next_state[in][state];
			if (X_N[this->K + i+0] != ((parity>>1) &1)) return false;
			if (X_N[this->K + i+1] != ((parity   ) &1)) return false;
		}
	}
	else
	{
		for (auto i = 0; i < this->K; i+=2)
		{
			auto in = X_N[i*2+0]*2 + X_N[i*2+1];
			auto parity = out_parity[in][state];
			state = next_state[in][state];
			if (X_N[i*2+2] != ((parity>>1) &1)) return false;
			if (X_N[i*2+3] != ((parity   ) &1)) return false;
		}
	}

	if (init_state != state)
		return false;

	return true;
}

template <typename B>
bool Encoder_RSC_DB<B>
::is_buffered() const
{
	return this->buffered_encoding;
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Encoder_RSC_DB<B_8>;
template class aff3ct::module::Encoder_RSC_DB<B_16>;
template class aff3ct::module::Encoder_RSC_DB<B_32>;
template class aff3ct::module::Encoder_RSC_DB<B_64>;
#else
template class aff3ct::module::Encoder_RSC_DB<B>;
#endif
// ==================================================================================== explicit template instantiation
