#include "Tools/Algo/Bit_packer.hpp"

#include "Decoder_polar_SCL_fast_CA_sys.hpp"

template <typename B, typename R, class API_polar>
Decoder_polar_SCL_fast_CA_sys<B,R,API_polar>
::Decoder_polar_SCL_fast_CA_sys(const int& K, const int& N, const int& L, const mipp::vector<B>& frozen_bits, CRC<B>& crc,
                                const int n_frames, const std::string name)
: Decoder_polar_SCL_fast_sys<B,R,API_polar>(K, N, L, frozen_bits, n_frames, name), crc(crc), U_test(K)
{
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SCL_fast_CA_sys<B,R,API_polar>
::select_best_path()
{
	for (auto path = 0; path < this->L; path++)
	{
		// extract the info bits from the codeword
		auto k = 0;
		for (auto leaf = 0; leaf < this->N; leaf++)
			if (!this->frozen_bits[leaf])
				U_test[k++] = this->s[path][leaf];

//		// check the CRC (slower than the next version)
//		bool decode_result = crc.check(U_test);

		// check the CRC (faster than previous version)
		Bit_packer<B>::pack(U_test);
		bool decode_result = crc.check_packed(U_test);

		// delete the path if the CRC result is negative
		if (!decode_result)
			this->delete_path(path);
	}

	this->Decoder_polar_SCL_fast_sys<B,R,API_polar>::select_best_path();
}
