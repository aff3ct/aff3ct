#include <string>
#include <set>

#include "Module/Decoder/Polar/SCL/CRC/Decoder_polar_SCL_naive_CA_sys.hpp"

namespace aff3ct
{
namespace module
{
template <typename B, typename R, tools::proto_f<R> F, tools::proto_g<B,R> G>
Decoder_polar_SCL_naive_CA_sys<B,R,F,G>
::Decoder_polar_SCL_naive_CA_sys(const int& K, const int& N, const int& L, const std::vector<bool>& frozen_bits,
                                 const CRC<B>& crc)
: Decoder_polar_SCL_naive_CA<B,R,F,G>(K, N, L, frozen_bits, crc)
{
	const std::string name = "Decoder_polar_SCL_naive_CA_sys";
	this->set_name(name);
}

template <typename B, typename R, tools::proto_f<R> F, tools::proto_g<B,R> G>
Decoder_polar_SCL_naive_CA_sys<B,R,F,G>* Decoder_polar_SCL_naive_CA_sys<B,R,F,G>
::clone() const
{
	auto m = new Decoder_polar_SCL_naive_CA_sys(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B, typename R, tools::proto_f<R> F, tools::proto_g<B,R> G>
void Decoder_polar_SCL_naive_CA_sys<B,R,F,G>
::select_best_path(const size_t frame_id)
{
	std::vector<B> U_test;
	std::set<int> active_paths_before_crc = this->active_paths;
	for (auto path : active_paths_before_crc)
	{
		U_test.clear();

		for (auto i = 0 ; i < this->N ; i++)
			if (!this->frozen_bits[i]) U_test.push_back(this->polar_trees[path].get_root()->get_c()->s[i]);

		bool decode_result = this->crc->check(U_test, frame_id);
		if (!decode_result)
			this->active_paths.erase(path);
	}

	this->Decoder_polar_SCL_naive<B,R,F,G>::select_best_path(frame_id);
}


template <typename B, typename R, tools::proto_f<R> F, tools::proto_g<B,R> G>
void Decoder_polar_SCL_naive_CA_sys<B,R,F,G>
::_store(B *V, bool coded) const
{
	if (!coded)
	{
		auto k = 0;
		for (auto i = 0; i < this->N; i++)
			if (!this->frozen_bits[i])
				V[k++] = this->polar_trees[*this->active_paths.begin()].get_root()->get_c()->s[i] ? 1 : 0;
	}
	else
		for (auto i = 0; i < this->N; i++)
			V[i] = this->polar_trees[*this->active_paths.begin()].get_root()->get_c()->s[i] ? 1 : 0;
}
}
}
