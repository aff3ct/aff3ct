#include <sstream>
#include <string>
#include <set>

#include "Tools/Exception/exception.hpp"
#include "Module/Decoder/Polar_MK/SCL/CRC/Decoder_polar_MK_SCL_naive_CA.hpp"

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
                                const CRC<B>& crc)
: Decoder_polar_MK_SCL_naive<B,R>(K, N, L, code, frozen_bits, lambdas),
  crc(crc.clone())
{
	const std::string name = "Decoder_polar_MK_SCL_naive_CA";
	this->set_name(name);

	if (this->crc->get_size() > K)
	{
		std::stringstream message;
		message << "'crc->get_size()' has to be equal or smaller than 'K' ('crc->get_size()' = "
		        << this->crc->get_size() << ", 'K' = " << K << ").";
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
                                const CRC<B>& crc)
: Decoder_polar_MK_SCL_naive_CA<B,R>(K, N, L, code, frozen_bits, {}, crc)
{
}

template <typename B, typename R>
Decoder_polar_MK_SCL_naive_CA<B,R>* Decoder_polar_MK_SCL_naive_CA<B,R>
::clone() const
{
	auto m = new Decoder_polar_MK_SCL_naive_CA(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B, typename R>
void Decoder_polar_MK_SCL_naive_CA<B,R>
::deep_copy(const Decoder_polar_MK_SCL_naive_CA<B,R> &m)
{
	Decoder_polar_MK_SCL_naive<B,R>::deep_copy(m);
	if (m.crc != nullptr) this->crc.reset(m.crc->clone());
}

template <typename B, typename R>
void Decoder_polar_MK_SCL_naive_CA<B,R>
::select_best_path(const size_t frame_id)
{
	std::vector<B> U_test;
	std::set<int> active_paths_before_crc = this->active_paths;
	for (auto path : active_paths_before_crc)
	{
		U_test.clear();

		for (auto leaf = 0 ; leaf < this->N ; leaf++)
			if (!this->frozen_bits[leaf])
				U_test.push_back(this->leaves_array[path][leaf]->get_c()->s[0]);

		bool decode_result = crc->check(U_test, frame_id);
		if (!decode_result)
			this->active_paths.erase(path);
	}

	this->Decoder_polar_MK_SCL_naive<B,R>::select_best_path(frame_id);
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
