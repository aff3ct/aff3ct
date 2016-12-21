#include "Decoder_polar_SCL_fast_CA_sys.hpp"

template <typename B, typename R, class API_polar>
Decoder_polar_SCL_fast_CA_sys<B,R,API_polar>
::Decoder_polar_SCL_fast_CA_sys(const int& K, const int& N, const int& L, const mipp::vector<B>& frozen_bits, CRC<B>& crc,
                                const int n_frames, const std::string name)
: Decoder_polar_SCL_fast_sys<B,R,API_polar>(K, N, L, frozen_bits, n_frames, name), crc(crc)
{
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SCL_fast_CA_sys<B,R,API_polar>
::select_best_path()
{
	mipp::vector<B> U_test;
	for (auto path = 0 ; path < this->L ; path++)
	{
		U_test.clear();

		for (auto leaf = 0 ; leaf < this->N ; leaf++)
			if (!this->frozen_bits[leaf]) 
				U_test.push_back(this->s[path][leaf]);

		bool decode_result = crc.check(U_test);
		if (!decode_result)
			this->delete_path(path);
	}

	this->Decoder_polar_SCL_fast_sys<B,R,API_polar>::select_best_path();
}
