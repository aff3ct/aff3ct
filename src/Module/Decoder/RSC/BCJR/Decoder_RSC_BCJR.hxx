#include "Decoder_RSC_BCJR.hpp"

#include <limits>

#include "Tools/Display/bash_tools.h"
#include "Tools/Perf/MIPP/mipp.h"
#include "Tools/Perf/Reorderer/Reorderer.hpp"

namespace aff3ct
{
template <typename B, typename R>
Decoder_RSC_BCJR<B,R>
::Decoder_RSC_BCJR(const int K, 
                   const std::vector<std::vector<int>> &trellis, 
                   const bool buffered_encoding,
                   const int n_frames,
                   const int simd_inter_frame_level,
                   const std::string name)
: Decoder_SISO<B,R>(K, 2*(K + (int)std::log2(trellis[0].size())), n_frames, simd_inter_frame_level, name),
  n_states((int)trellis[0].size()),
  n_ff((int)std::log2(n_states)),
  buffered_encoding(buffered_encoding),
  trellis(trellis),
  sys((K+n_ff) * simd_inter_frame_level + mipp::nElReg<R>()),
  par((K+n_ff) * simd_inter_frame_level + mipp::nElReg<R>()),
  ext( K       * simd_inter_frame_level + mipp::nElReg<R>()),
  s  ( K       * simd_inter_frame_level + mipp::nElReg<B>())
{
	assert(is_power_of_2(n_states));
}

template <typename B, typename R>
Decoder_RSC_BCJR<B,R>
::~Decoder_RSC_BCJR()
{
}

template <typename B, typename R>
void Decoder_RSC_BCJR<B,R>
::load(const mipp::vector<R>& Y_N)
{
	if (buffered_encoding)
	{
		const auto tail     = this->tail_length();
		const auto n_frames = Decoder<B,R>::get_simd_inter_frame_level();

		if (n_frames == 1)
		{
			std::copy(Y_N.begin() + 0*this->K, Y_N.begin() + 1*this->K, sys.begin());
			std::copy(Y_N.begin() + 1*this->K, Y_N.begin() + 2*this->K, par.begin());

			// tails bit
			std::copy(Y_N.begin() + 2*this->K         , Y_N.begin() + 2*this->K + tail/2, par.begin() +this->K);
			std::copy(Y_N.begin() + 2*this->K + tail/2, Y_N.begin() + 2*this->K + tail  , sys.begin() +this->K);
		}
		else
		{
			const auto frame_size = 2*this->K + tail;

			std::vector<const R*> frames(n_frames);
			for (auto f = 0; f < n_frames; f++)
				frames[f] = Y_N.data() + f*frame_size;
			Reorderer<R>::apply(frames, sys.data(), this->K);

			for (auto f = 0; f < n_frames; f++)
				frames[f] = Y_N.data() + f*frame_size +this->K;
			Reorderer<R>::apply(frames, par.data(), this->K);

			// tails bit
			for (auto f = 0; f < n_frames; f++)
				frames[f] = Y_N.data() + f*frame_size + 2*this->K + tail/2;
			Reorderer<R>::apply(frames, &sys[this->K*n_frames], tail/2);

			for (auto f = 0; f < n_frames; f++)
				frames[f] = Y_N.data() + f*frame_size + 2*this->K;
			Reorderer<R>::apply(frames, &par[this->K*n_frames], tail/2);
		}
	}
	else
	{
		const auto n_frames = this->get_simd_inter_frame_level();

		// reordering
		for (auto i = 0; i < this->K + n_ff; i++)
		{
			for (auto f = 0; f < n_frames; f++)
			{
				sys[(i*n_frames) +f] = Y_N[f*2*(this->K +n_ff) + i*2 +0];
				par[(i*n_frames) +f] = Y_N[f*2*(this->K +n_ff) + i*2 +1];
			}
		}
	}
}

template <typename B, typename R>
void Decoder_RSC_BCJR<B,R>
::hard_decode()
{
	soft_decode(sys, par, ext);

	// take the hard decision
	for (auto i = 0; i < this->K * this->simd_inter_frame_level; i += mipp::nElReg<R>())
	{
		const auto r_post = mipp::Reg<R>(&ext[i]) + mipp::Reg<R>(&sys[i]);

		// s[i] = post[i] < 0;
#if defined(MIPP_NO_INTRINSICS) && defined(_MSC_VER) 
		const auto r_s = mipp::Reg<B>((B)r_post.sign().r) >> (sizeof(B) * 8 -1);
#else
		const auto r_s = mipp::Reg<B>(r_post.sign().r) >> (sizeof(B) * 8 - 1);
#endif
		r_s.store(&s[i]);
	}
}

template <typename B, typename R>
void Decoder_RSC_BCJR<B,R>
::store(mipp::vector<B>& V_K) const
{
	if (this->get_simd_inter_frame_level() == 1)
	{
		std::copy(s.begin(), s.begin() + this->K, V_K.begin());
	}
	else // inter frame => output reordering
	{
		const auto n_frames = this->get_simd_inter_frame_level();

		std::vector<B*> frames(n_frames);
		for (auto f = 0; f < n_frames; f++)
			frames[f] = V_K.data() + f*this->K;
		Reorderer<B>::apply_rev(s.data(), frames, this->K);
	}
}

template <typename B, typename R>
void Decoder_RSC_BCJR<B,R>
::_soft_decode(const mipp::vector<R> &Y_N1, mipp::vector<R> &Y_N2)
{
	std::cerr << bold_red("(EE) This decoder does not support this interface.") << std::endl;
	std::exit(-1);
}
}
