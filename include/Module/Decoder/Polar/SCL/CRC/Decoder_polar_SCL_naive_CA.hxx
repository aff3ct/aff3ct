#include <string>
#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Module/Decoder/Polar/SCL/CRC/Decoder_polar_SCL_naive_CA.hpp"

namespace aff3ct
{
namespace module
{
template <typename B, typename R, tools::proto_f<R> F, tools::proto_g<B,R> G>
Decoder_polar_SCL_naive_CA<B,R,F,G>
::Decoder_polar_SCL_naive_CA(const int& K, const int& N, const int& L, const std::vector<bool>& frozen_bits,
                             const CRC<B>& crc)
: Decoder_polar_SCL_naive<B,R,F,G>(K, N, L, frozen_bits), crc(crc.clone())
{
	const std::string name = "Decoder_polar_SCL_naive_CA";
	this->set_name(name);

	if (this->crc->get_size() > K)
	{
		std::stringstream message;
		message << "'crc->get_size()' has to be equal or smaller than 'K' ('crc->get_size()' = " << this->crc->get_size()
		        << ", 'K' = " << K << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename B, typename R, tools::proto_f<R> F, tools::proto_g<B,R> G>
Decoder_polar_SCL_naive_CA<B,R,F,G>* Decoder_polar_SCL_naive_CA<B,R,F,G>
::clone() const
{
	auto m = new Decoder_polar_SCL_naive_CA(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B, typename R, tools::proto_f<R> F, tools::proto_g<B,R> G>
void Decoder_polar_SCL_naive_CA<B,R,F,G>
::deep_copy(const Decoder_polar_SCL_naive_CA<B,R,F,G> &m)
{
	Decoder_polar_SCL_naive<B,R,F,G>::deep_copy(m);
	if (m.crc != nullptr) this->crc.reset(m.crc->clone());
}

template <typename B, typename R, tools::proto_f<R> F, tools::proto_g<B,R> G>
void Decoder_polar_SCL_naive_CA<B,R,F,G>
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

	this->Decoder_polar_SCL_naive<B,R,F,G>::select_best_path(frame_id);
}
}
}
