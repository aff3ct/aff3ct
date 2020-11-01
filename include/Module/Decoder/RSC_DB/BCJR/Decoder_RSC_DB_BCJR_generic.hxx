#include <string>
#include <limits>
#include <algorithm>

#include "Module/Decoder/RSC_DB/BCJR/Decoder_RSC_DB_BCJR_generic.hpp"

namespace aff3ct
{
namespace module
{
template <typename B, typename R, tools::proto_max<R> MAX>
Decoder_RSC_DB_BCJR_generic<B,R,MAX>
::Decoder_RSC_DB_BCJR_generic(const int K,
                              const std::vector<std::vector<int>> &trellis,
                              const bool buffered_encoding)
: Decoder_RSC_DB_BCJR<B,R>(K, trellis, buffered_encoding)
{
	const std::string name = "Decoder_RSC_DB_BCJR_generic";
	this->set_name(name);
}

template <typename B, typename R, tools::proto_max<R> MAX>
Decoder_RSC_DB_BCJR_generic<B,R,MAX>* Decoder_RSC_DB_BCJR_generic<B,R,MAX>
::clone() const
{
	auto m = new Decoder_RSC_DB_BCJR_generic(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B, typename R, tools::proto_max<R> MAX>
void Decoder_RSC_DB_BCJR_generic<B,R,MAX>
::__fwd_recursion(const R *sys, const R *par)
{
	for (auto k = 0; k < this->K/2; k++)
	{
		for (auto s = 0; s < this->n_states; s++)
		{
			int s0 = this->trellis[1][s*4 + 0];
			int s1 = this->trellis[1][s*4 + 1];
			int s2 = this->trellis[1][s*4 + 2];
			int s3 = this->trellis[1][s*4 + 3];

			R y = par[2*k  ];
			R w = par[2*k+1];

			R p0 = this->trellis[2][s0*4+0]*y + this->trellis[3][s0*4+0]*w;
			R p1 = this->trellis[2][s1*4+1]*y + this->trellis[3][s1*4+1]*w;
			R p2 = this->trellis[2][s2*4+2]*y + this->trellis[3][s2*4+2]*w;
			R p3 = this->trellis[2][s3*4+3]*y + this->trellis[3][s3*4+3]*w;

			this->gamma[k][4*s0 + 0] = p0 + sys[4*k + 0];
			this->gamma[k][4*s1 + 1] = p1 + sys[4*k + 1];
			this->gamma[k][4*s2 + 2] = p2 + sys[4*k + 2];
			this->gamma[k][4*s3 + 3] = p3 + sys[4*k + 3];

			this->alpha[k+1][s] = MAX(MAX(this->alpha[k][s0] + this->gamma[k][4*s0 + 0],
			                              this->alpha[k][s1] + this->gamma[k][4*s1 + 1]),
			                          MAX(this->alpha[k][s2] + this->gamma[k][4*s2 + 2],
			                              this->alpha[k][s3] + this->gamma[k][4*s3 + 3]));

		}

		R norm = this->alpha[k+1][0];
		for (auto s = 0; s < this->n_states; s++)
			this->alpha[k+1][s] -= norm;
	}
}

template <typename B, typename R, tools::proto_max<R> MAX>
void Decoder_RSC_DB_BCJR_generic<B,R,MAX>
::__bwd_recursion(const R *sys, const R *par, R* ext)
{
	for (auto k = this->K/2 - 1; k >= 0; k--)
	{
		for (auto s = 0; s < this->n_states; s++)
		{
			this->beta[k][s] = MAX(MAX(this->beta[k+1][this->trellis[0][4*s + 0]] + this->gamma[k][4*s + 0],
			                           this->beta[k+1][this->trellis[0][4*s + 1]] + this->gamma[k][4*s + 1]),
			                       MAX(this->beta[k+1][this->trellis[0][4*s + 2]] + this->gamma[k][4*s + 2],
			                           this->beta[k+1][this->trellis[0][4*s + 3]] + this->gamma[k][4*s + 3]));
		}

		R norm = this->beta[k][0];
		for (auto s = 0; s < this->n_states; s++)
			this->beta[k][s] -= norm;

		R post0 = std::numeric_limits<R>::lowest();
		R post1 = std::numeric_limits<R>::lowest();
		R post2 = std::numeric_limits<R>::lowest();
		R post3 = std::numeric_limits<R>::lowest();

		for (auto s = 0; s < this->n_states; s++)
		{
			post0 = MAX(post0, this->alpha[k][s] + this->gamma[k][4*s + 0] + this->beta[k+1][this->trellis[0][4*s + 0]]);
			post1 = MAX(post1, this->alpha[k][s] + this->gamma[k][4*s + 1] + this->beta[k+1][this->trellis[0][4*s + 1]]);
			post2 = MAX(post2, this->alpha[k][s] + this->gamma[k][4*s + 2] + this->beta[k+1][this->trellis[0][4*s + 2]]);
			post3 = MAX(post3, this->alpha[k][s] + this->gamma[k][4*s + 3] + this->beta[k+1][this->trellis[0][4*s + 3]]);
		}

		ext[4*k + 0] = post0 - sys[4*k + 0];
		ext[4*k + 1] = post1 - sys[4*k + 1];
		ext[4*k + 2] = post2 - sys[4*k + 2];
		ext[4*k + 3] = post3 - sys[4*k + 3];
	}
}
}
}
