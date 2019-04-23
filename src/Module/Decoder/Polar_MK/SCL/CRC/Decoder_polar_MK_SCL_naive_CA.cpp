#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Decoder_polar_MK_SCL_naive_CA.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename R>
Decoder_polar_MK_SCL_naive_CA<B,R>
::Decoder_polar_MK_SCL_naive_CA(const int& K,
                                const int& N,
                                const int& L,
                                const tools::Polar_code& code,
                                const std::vector<bool>& frozen_bits,
                                const std::vector<std::vector<std::function<R(const std::vector<R> &LLRs,
                                                                              const std::vector<B> &bits)>>> &lambdas,
                                CRC<B>& crc, const int n_frames)
: Decoder                        (K, N,                                n_frames, 1),
  Decoder_polar_MK_SCL_naive<B,R>(K, N, L, code, frozen_bits, lambdas, n_frames   ),
  crc(crc)
{
	const std::string name = "Decoder_polar_MK_SCL_naive_CA";
	this->set_name(name);

	if (crc.get_size() > K)
	{
		std::stringstream message;
		message << "'crc.get_size()' has to be equal or smaller than 'K' ('crc.get_size()' = " << crc.get_size()
		        << ", 'K' = " << K << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename B, typename R>
Decoder_polar_MK_SCL_naive_CA<B,R>
::Decoder_polar_MK_SCL_naive_CA(const int& K,
                                const int& N,
                                const int& L,
                                const tools::Polar_code& code,
                                const std::vector<bool>& frozen_bits,
                                CRC<B>& crc, const int n_frames)
: Decoder_polar_MK_SCL_naive_CA<B,R>(K, N, L, code, frozen_bits, {}, crc, n_frames)
{
}

template <typename B, typename R>
void Decoder_polar_MK_SCL_naive_CA<B,R>
::select_best_path()
{
	std::vector<B> U_test;
	std::set<int> active_paths_before_crc = this->active_paths;
	for (auto path : active_paths_before_crc)
	{
		U_test.clear();

		for (auto leaf = 0 ; leaf < this->N ; leaf++)
			if (!this->frozen_bits[leaf])
				U_test.push_back(this->leaves_array[path][leaf]->get_c()->s[0]);

		bool decode_result = crc.check(U_test, this->get_simd_inter_frame_level());
		if (!decode_result)
			this->active_paths.erase(path);
	}

	this->Decoder_polar_MK_SCL_naive<B,R>::select_best_path();
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Decoder_polar_MK_SCL_naive_CA<B_8,Q_8>;
template class aff3ct::module::Decoder_polar_MK_SCL_naive_CA<B_16,Q_16>;
template class aff3ct::module::Decoder_polar_MK_SCL_naive_CA<B_32,Q_32>;
template class aff3ct::module::Decoder_polar_MK_SCL_naive_CA<B_64,Q_64>;
#else
template class aff3ct::module::Decoder_polar_MK_SCL_naive_CA<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
