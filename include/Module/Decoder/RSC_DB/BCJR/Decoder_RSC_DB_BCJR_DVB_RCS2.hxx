#include <string>
#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Module/Decoder/RSC_DB/BCJR/Decoder_RSC_DB_BCJR_DVB_RCS2.hpp"

namespace aff3ct
{
namespace module
{
template <typename B, typename R, tools::proto_max<R> MAX>
Decoder_RSC_DB_BCJR_DVB_RCS2<B,R,MAX>
::Decoder_RSC_DB_BCJR_DVB_RCS2(const int K,
                               const std::vector<std::vector<int>> &trellis,
                               const bool buffered_encoding)
: Decoder_RSC_DB_BCJR<B,R>(K, trellis, buffered_encoding)
{
	const std::string name = "Decoder_RSC_DB_BCJR_DVB_RCS2";
	this->set_name(name);

	if (this->n_states != 16)
	{
		std::stringstream message;
		message << "'n_states' has to be equal to 16 ('n_states' = " << this->n_states << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename B, typename R, tools::proto_max<R> MAX>
Decoder_RSC_DB_BCJR_DVB_RCS2<B,R,MAX>* Decoder_RSC_DB_BCJR_DVB_RCS2<B,R,MAX>
::clone() const
{
	auto m = new Decoder_RSC_DB_BCJR_DVB_RCS2(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B, typename R, tools::proto_max<R> MAX>
void Decoder_RSC_DB_BCJR_DVB_RCS2<B,R,MAX>
::__fwd_recursion(const R *sys, const R *par)
{
	for (auto k = 0; k < this->K/2; k++)
	{
		R w = par[2*k  ];
		R y = par[2*k+1];

		this->gamma[k][ 0] = sys[4*k + 0] + y + w;
		this->gamma[k][ 1] = sys[4*k + 0] + y - w;
		this->gamma[k][ 2] = sys[4*k + 0] - y + w;
		this->gamma[k][ 3] = sys[4*k + 0] - y - w;
		this->gamma[k][ 4] = sys[4*k + 1] + y + w;
		this->gamma[k][ 5] = sys[4*k + 1] + y - w;
		this->gamma[k][ 6] = sys[4*k + 1] - y + w;
		this->gamma[k][ 7] = sys[4*k + 1] - y - w;
		this->gamma[k][ 8] = sys[4*k + 2] + y + w;
		this->gamma[k][ 9] = sys[4*k + 2] + y - w;
		this->gamma[k][10] = sys[4*k + 2] - y + w;
		this->gamma[k][11] = sys[4*k + 2] - y - w;
		this->gamma[k][12] = sys[4*k + 3] + y + w;
		this->gamma[k][13] = sys[4*k + 3] + y - w;
		this->gamma[k][14] = sys[4*k + 3] - y + w;
		this->gamma[k][15] = sys[4*k + 3] - y - w;

		this->alpha[k+1][0 ] = MAX(MAX(this->alpha[k][ 0] + this->gamma[k][ 0], this->alpha[k][10] + this->gamma[k][ 7]),
		                           MAX(this->alpha[k][ 1] + this->gamma[k][11], this->alpha[k][11] + this->gamma[k][12]));

		this->alpha[k+1][1 ] = MAX(MAX(this->alpha[k][ 3] + this->gamma[k][ 1], this->alpha[k][ 9] + this->gamma[k][ 6]),
		                           MAX(this->alpha[k][ 2] + this->gamma[k][10], this->alpha[k][ 8] + this->gamma[k][13]));

		this->alpha[k+1][2 ] = MAX(MAX(this->alpha[k][ 4] + this->gamma[k][ 3], this->alpha[k][14] + this->gamma[k][ 4]),
		                           MAX(this->alpha[k][ 5] + this->gamma[k][ 8], this->alpha[k][15] + this->gamma[k][15]));

		this->alpha[k+1][3 ] = MAX(MAX(this->alpha[k][ 7] + this->gamma[k][ 2], this->alpha[k][13] + this->gamma[k][ 5]),
		                           MAX(this->alpha[k][ 6] + this->gamma[k][ 9], this->alpha[k][12] + this->gamma[k][14]));

		this->alpha[k+1][4 ] = MAX(MAX(this->alpha[k][ 8] + this->gamma[k][ 1], this->alpha[k][ 2] + this->gamma[k][ 6]),
		                           MAX(this->alpha[k][ 9] + this->gamma[k][10], this->alpha[k][ 3] + this->gamma[k][13]));

		this->alpha[k+1][5 ] = MAX(MAX(this->alpha[k][11] + this->gamma[k][ 0], this->alpha[k][ 1] + this->gamma[k][ 7]),
		                           MAX(this->alpha[k][10] + this->gamma[k][11], this->alpha[k][ 0] + this->gamma[k][12]));

		this->alpha[k+1][6 ] = MAX(MAX(this->alpha[k][12] + this->gamma[k][ 2], this->alpha[k][ 6] + this->gamma[k][ 5]),
		                           MAX(this->alpha[k][13] + this->gamma[k][ 9], this->alpha[k][ 7] + this->gamma[k][14]));

		this->alpha[k+1][7 ] = MAX(MAX(this->alpha[k][15] + this->gamma[k][ 3], this->alpha[k][ 5] + this->gamma[k][ 4]),
		                           MAX(this->alpha[k][14] + this->gamma[k][ 8], this->alpha[k][ 4] + this->gamma[k][15]));

		this->alpha[k+1][8 ] = MAX(MAX(this->alpha[k][ 1] + this->gamma[k][ 0], this->alpha[k][11] + this->gamma[k][ 7]),
		                           MAX(this->alpha[k][ 0] + this->gamma[k][11], this->alpha[k][10] + this->gamma[k][12]));

		this->alpha[k+1][9 ] = MAX(MAX(this->alpha[k][ 2] + this->gamma[k][ 1], this->alpha[k][ 8] + this->gamma[k][ 6]),
		                           MAX(this->alpha[k][ 3] + this->gamma[k][10], this->alpha[k][ 9] + this->gamma[k][13]));

		this->alpha[k+1][10] = MAX(MAX(this->alpha[k][ 5] + this->gamma[k][ 3], this->alpha[k][15] + this->gamma[k][ 4]),
		                           MAX(this->alpha[k][ 4] + this->gamma[k][ 8], this->alpha[k][14] + this->gamma[k][15]));

		this->alpha[k+1][11] = MAX(MAX(this->alpha[k][ 6] + this->gamma[k][ 2], this->alpha[k][12] + this->gamma[k][ 5]),
		                           MAX(this->alpha[k][ 7] + this->gamma[k][ 9], this->alpha[k][13] + this->gamma[k][14]));

		this->alpha[k+1][12] = MAX(MAX(this->alpha[k][ 9] + this->gamma[k][ 1], this->alpha[k][ 3] + this->gamma[k][ 6]),
		                           MAX(this->alpha[k][ 8] + this->gamma[k][10], this->alpha[k][ 2] + this->gamma[k][13]));

		this->alpha[k+1][13] = MAX(MAX(this->alpha[k][10] + this->gamma[k][ 0], this->alpha[k][ 0] + this->gamma[k][ 7]),
		                           MAX(this->alpha[k][11] + this->gamma[k][11], this->alpha[k][ 1] + this->gamma[k][12]));

		this->alpha[k+1][14] = MAX(MAX(this->alpha[k][13] + this->gamma[k][ 2], this->alpha[k][ 7] + this->gamma[k][ 5]),
		                           MAX(this->alpha[k][12] + this->gamma[k][ 9], this->alpha[k][ 6] + this->gamma[k][14]));

		this->alpha[k+1][15] = MAX(MAX(this->alpha[k][14] + this->gamma[k][ 3], this->alpha[k][ 4] + this->gamma[k][ 4]),
		                           MAX(this->alpha[k][15] + this->gamma[k][ 8], this->alpha[k][ 5] + this->gamma[k][15]));

		R norm = this->alpha[k+1][0];
		for (auto s = 0; s < 16; s++)
			this->alpha[k+1][s] -= norm;
	}
}

template <typename B, typename R, tools::proto_max<R> MAX>
void Decoder_RSC_DB_BCJR_DVB_RCS2<B,R,MAX>
::__bwd_recursion(const R *sys, const R *par, R* ext)
{
	for (auto k = this->K/2 - 1; k >= 0; k--)
	{
		this->beta[k][0 ] = MAX(MAX(this->beta[k+1][ 0] + this->gamma[k][ 0], this->beta[k+1][13] + this->gamma[k][ 7]),
		                        MAX(this->beta[k+1][ 8] + this->gamma[k][11], this->beta[k+1][ 5] + this->gamma[k][12]));

		this->beta[k][1 ] = MAX(MAX(this->beta[k+1][ 8] + this->gamma[k][ 0], this->beta[k+1][ 5] + this->gamma[k][ 7]),
		                        MAX(this->beta[k+1][ 0] + this->gamma[k][11], this->beta[k+1][13] + this->gamma[k][12]));

		this->beta[k][2 ] = MAX(MAX(this->beta[k+1][ 9] + this->gamma[k][ 1], this->beta[k+1][ 4] + this->gamma[k][ 6]),
		                        MAX(this->beta[k+1][ 1] + this->gamma[k][10], this->beta[k+1][12] + this->gamma[k][13]));

		this->beta[k][3 ] = MAX(MAX(this->beta[k+1][ 1] + this->gamma[k][ 1], this->beta[k+1][12] + this->gamma[k][ 6]),
		                        MAX(this->beta[k+1][ 9] + this->gamma[k][10], this->beta[k+1][ 4] + this->gamma[k][13]));

		this->beta[k][4 ] = MAX(MAX(this->beta[k+1][ 2] + this->gamma[k][ 3], this->beta[k+1][15] + this->gamma[k][ 4]),
		                        MAX(this->beta[k+1][10] + this->gamma[k][ 8], this->beta[k+1][ 7] + this->gamma[k][15]));

		this->beta[k][5 ] = MAX(MAX(this->beta[k+1][10] + this->gamma[k][ 3], this->beta[k+1][ 7] + this->gamma[k][ 4]),
		                        MAX(this->beta[k+1][ 2] + this->gamma[k][ 8], this->beta[k+1][15] + this->gamma[k][15]));

		this->beta[k][6 ] = MAX(MAX(this->beta[k+1][11] + this->gamma[k][ 2], this->beta[k+1][ 6] + this->gamma[k][ 5]),
		                        MAX(this->beta[k+1][ 3] + this->gamma[k][ 9], this->beta[k+1][14] + this->gamma[k][14]));

		this->beta[k][7 ] = MAX(MAX(this->beta[k+1][ 3] + this->gamma[k][ 2], this->beta[k+1][14] + this->gamma[k][ 5]),
		                        MAX(this->beta[k+1][11] + this->gamma[k][ 9], this->beta[k+1][ 6] + this->gamma[k][14]));

		this->beta[k][8 ] = MAX(MAX(this->beta[k+1][ 4] + this->gamma[k][ 1], this->beta[k+1][ 9] + this->gamma[k][ 6]),
		                        MAX(this->beta[k+1][12] + this->gamma[k][10], this->beta[k+1][ 1] + this->gamma[k][13]));

		this->beta[k][9 ] = MAX(MAX(this->beta[k+1][12] + this->gamma[k][ 1], this->beta[k+1][ 1] + this->gamma[k][ 6]),
		                        MAX(this->beta[k+1][ 4] + this->gamma[k][10], this->beta[k+1][ 9] + this->gamma[k][13]));

		this->beta[k][10] = MAX(MAX(this->beta[k+1][13] + this->gamma[k][ 0], this->beta[k+1][ 0] + this->gamma[k][ 7]),
		                        MAX(this->beta[k+1][ 5] + this->gamma[k][11], this->beta[k+1][ 8] + this->gamma[k][12]));

		this->beta[k][11] = MAX(MAX(this->beta[k+1][ 5] + this->gamma[k][ 0], this->beta[k+1][ 8] + this->gamma[k][ 7]),
		                        MAX(this->beta[k+1][13] + this->gamma[k][11], this->beta[k+1][ 0] + this->gamma[k][12]));

		this->beta[k][12] = MAX(MAX(this->beta[k+1][ 6] + this->gamma[k][ 2], this->beta[k+1][11] + this->gamma[k][ 5]),
		                        MAX(this->beta[k+1][14] + this->gamma[k][ 9], this->beta[k+1][ 3] + this->gamma[k][14]));

		this->beta[k][13] = MAX(MAX(this->beta[k+1][14] + this->gamma[k][ 2], this->beta[k+1][ 3] + this->gamma[k][ 5]),
		                        MAX(this->beta[k+1][ 6] + this->gamma[k][ 9], this->beta[k+1][11] + this->gamma[k][14]));

		this->beta[k][14] = MAX(MAX(this->beta[k+1][15] + this->gamma[k][ 3], this->beta[k+1][ 2] + this->gamma[k][ 4]),
		                        MAX(this->beta[k+1][ 7] + this->gamma[k][ 8], this->beta[k+1][10] + this->gamma[k][15]));

		this->beta[k][15] = MAX(MAX(this->beta[k+1][ 7] + this->gamma[k][ 3], this->beta[k+1][10] + this->gamma[k][ 4]),
		                        MAX(this->beta[k+1][15] + this->gamma[k][ 8], this->beta[k+1][ 2] + this->gamma[k][15]));

		R norm = this->beta[k][0];
		for (auto s = 0; s < 16; s++)
			this->beta[k][s] -= norm;
	}

	R post[4][16];

	for (auto k = 0; k < this->K/2; k++)
	{
		post[0][ 0] = this->alpha[k][ 0] + this->beta[k+1][ 0] + this->gamma[k][ 0];
		post[0][ 1] = this->alpha[k][ 1] + this->beta[k+1][ 8] + this->gamma[k][ 0];
		post[0][ 2] = this->alpha[k][ 2] + this->beta[k+1][ 9] + this->gamma[k][ 1];
		post[0][ 3] = this->alpha[k][ 3] + this->beta[k+1][ 1] + this->gamma[k][ 1];
		post[0][ 4] = this->alpha[k][ 4] + this->beta[k+1][ 2] + this->gamma[k][ 3];
		post[0][ 5] = this->alpha[k][ 5] + this->beta[k+1][10] + this->gamma[k][ 3];
		post[0][ 6] = this->alpha[k][ 6] + this->beta[k+1][11] + this->gamma[k][ 2];
		post[0][ 7] = this->alpha[k][ 7] + this->beta[k+1][ 3] + this->gamma[k][ 2];
		post[0][ 8] = this->alpha[k][ 8] + this->beta[k+1][ 4] + this->gamma[k][ 1];
		post[0][ 9] = this->alpha[k][ 9] + this->beta[k+1][12] + this->gamma[k][ 1];
		post[0][10] = this->alpha[k][10] + this->beta[k+1][13] + this->gamma[k][ 0];
		post[0][11] = this->alpha[k][11] + this->beta[k+1][ 5] + this->gamma[k][ 0];
		post[0][12] = this->alpha[k][12] + this->beta[k+1][ 6] + this->gamma[k][ 2];
		post[0][13] = this->alpha[k][13] + this->beta[k+1][14] + this->gamma[k][ 2];
		post[0][14] = this->alpha[k][14] + this->beta[k+1][15] + this->gamma[k][ 3];
		post[0][15] = this->alpha[k][15] + this->beta[k+1][ 7] + this->gamma[k][ 3];

		post[1][ 0] = this->alpha[k][ 0] + this->beta[k+1][13] + this->gamma[k][ 7];
		post[1][ 1] = this->alpha[k][ 1] + this->beta[k+1][ 5] + this->gamma[k][ 7];
		post[1][ 2] = this->alpha[k][ 2] + this->beta[k+1][ 4] + this->gamma[k][ 6];
		post[1][ 3] = this->alpha[k][ 3] + this->beta[k+1][12] + this->gamma[k][ 6];
		post[1][ 4] = this->alpha[k][ 4] + this->beta[k+1][15] + this->gamma[k][ 4];
		post[1][ 5] = this->alpha[k][ 5] + this->beta[k+1][ 7] + this->gamma[k][ 4];
		post[1][ 6] = this->alpha[k][ 6] + this->beta[k+1][ 6] + this->gamma[k][ 5];
		post[1][ 7] = this->alpha[k][ 7] + this->beta[k+1][14] + this->gamma[k][ 5];
		post[1][ 8] = this->alpha[k][ 8] + this->beta[k+1][ 9] + this->gamma[k][ 6];
		post[1][ 9] = this->alpha[k][ 9] + this->beta[k+1][ 1] + this->gamma[k][ 6];
		post[1][10] = this->alpha[k][10] + this->beta[k+1][ 0] + this->gamma[k][ 7];
		post[1][11] = this->alpha[k][11] + this->beta[k+1][ 8] + this->gamma[k][ 7];
		post[1][12] = this->alpha[k][12] + this->beta[k+1][11] + this->gamma[k][ 5];
		post[1][13] = this->alpha[k][13] + this->beta[k+1][ 3] + this->gamma[k][ 5];
		post[1][14] = this->alpha[k][14] + this->beta[k+1][ 2] + this->gamma[k][ 4];
		post[1][15] = this->alpha[k][15] + this->beta[k+1][10] + this->gamma[k][ 4];

		post[2][ 0] = this->alpha[k][ 0] + this->beta[k+1][ 8] + this->gamma[k][11];
		post[2][ 1] = this->alpha[k][ 1] + this->beta[k+1][ 0] + this->gamma[k][11];
		post[2][ 2] = this->alpha[k][ 2] + this->beta[k+1][ 1] + this->gamma[k][10];
		post[2][ 3] = this->alpha[k][ 3] + this->beta[k+1][ 9] + this->gamma[k][10];
		post[2][ 4] = this->alpha[k][ 4] + this->beta[k+1][10] + this->gamma[k][ 8];
		post[2][ 5] = this->alpha[k][ 5] + this->beta[k+1][ 2] + this->gamma[k][ 8];
		post[2][ 6] = this->alpha[k][ 6] + this->beta[k+1][ 3] + this->gamma[k][ 9];
		post[2][ 7] = this->alpha[k][ 7] + this->beta[k+1][11] + this->gamma[k][ 9];
		post[2][ 8] = this->alpha[k][ 8] + this->beta[k+1][12] + this->gamma[k][10];
		post[2][ 9] = this->alpha[k][ 9] + this->beta[k+1][ 4] + this->gamma[k][10];
		post[2][10] = this->alpha[k][10] + this->beta[k+1][ 5] + this->gamma[k][11];
		post[2][11] = this->alpha[k][11] + this->beta[k+1][13] + this->gamma[k][11];
		post[2][12] = this->alpha[k][12] + this->beta[k+1][14] + this->gamma[k][ 9];
		post[2][13] = this->alpha[k][13] + this->beta[k+1][ 6] + this->gamma[k][ 9];
		post[2][14] = this->alpha[k][14] + this->beta[k+1][ 7] + this->gamma[k][ 8];
		post[2][15] = this->alpha[k][15] + this->beta[k+1][15] + this->gamma[k][ 8];

		post[3][ 0] = this->alpha[k][ 0] + this->beta[k+1][ 5] + this->gamma[k][12];
		post[3][ 1] = this->alpha[k][ 1] + this->beta[k+1][13] + this->gamma[k][12];
		post[3][ 2] = this->alpha[k][ 2] + this->beta[k+1][12] + this->gamma[k][13];
		post[3][ 3] = this->alpha[k][ 3] + this->beta[k+1][ 4] + this->gamma[k][13];
		post[3][ 4] = this->alpha[k][ 4] + this->beta[k+1][ 7] + this->gamma[k][15];
		post[3][ 5] = this->alpha[k][ 5] + this->beta[k+1][15] + this->gamma[k][15];
		post[3][ 6] = this->alpha[k][ 6] + this->beta[k+1][14] + this->gamma[k][14];
		post[3][ 7] = this->alpha[k][ 7] + this->beta[k+1][ 6] + this->gamma[k][14];
		post[3][ 8] = this->alpha[k][ 8] + this->beta[k+1][ 1] + this->gamma[k][13];
		post[3][ 9] = this->alpha[k][ 9] + this->beta[k+1][ 9] + this->gamma[k][13];
		post[3][10] = this->alpha[k][10] + this->beta[k+1][ 8] + this->gamma[k][12];
		post[3][11] = this->alpha[k][11] + this->beta[k+1][ 0] + this->gamma[k][12];
		post[3][12] = this->alpha[k][12] + this->beta[k+1][ 3] + this->gamma[k][14];
		post[3][13] = this->alpha[k][13] + this->beta[k+1][11] + this->gamma[k][14];
		post[3][14] = this->alpha[k][14] + this->beta[k+1][10] + this->gamma[k][15];
		post[3][15] = this->alpha[k][15] + this->beta[k+1][ 2] + this->gamma[k][15];

		R post0 = post[0][0];
		for (auto s = 1; s<16; s++)
			post0 = MAX(post0, post[0][s]);

		R post1 = post[1][0];
		for (auto s = 1; s<16; s++)
			post1 = MAX(post1, post[1][s]);

		R post2 = post[2][0];
		for (auto s = 1; s<16; s++)
			post2 = MAX(post2, post[2][s]);

		R post3 = post[3][0];
		for (auto s = 1; s<16; s++)
			post3 = MAX(post3, post[3][s]);

		ext[4*k + 0] = post0 - sys[4*k + 0];
		ext[4*k + 1] = post1 - sys[4*k + 1];
		ext[4*k + 2] = post2 - sys[4*k + 2];
		ext[4*k + 3] = post3 - sys[4*k + 3];
	}
}
}
}
