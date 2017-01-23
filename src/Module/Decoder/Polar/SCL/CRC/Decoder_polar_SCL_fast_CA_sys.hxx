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
bool Decoder_polar_SCL_fast_CA_sys<B,R,API_polar>
::crc_check(mipp::vector<B> &s)
{
//	// extract the info bits from the codeword
//	auto k = 0;
//	for (auto leaf = 0; leaf < this->N; leaf++)
//		if (!this->frozen_bits[leaf])
//			U_test[k++] = s[leaf];

//	// check the CRC
//	return crc.check(U_test, this->get_simd_inter_frame_level());

	// extract the info bits (packed) from the codeword
	auto bytes = (unsigned char*)U_test.data();
	auto leaf = 0, B_pos = 0;
	while (leaf < this->N)
	{
		unsigned char byte = 0;
		auto b = 0;
		while (b < 8 && (leaf < this->N))
		{
			byte |= !this->frozen_bits[leaf] ? ((unsigned char)(s[leaf] != 0)) << b++ : 0;
			leaf++;
		}
		bytes[B_pos++] = byte;
	}

	// check the CRC
	return crc.check_packed(U_test, this->get_simd_inter_frame_level());
}

template <typename B, typename R, class API_polar>
int Decoder_polar_SCL_fast_CA_sys<B,R,API_polar>
::select_best_path()
{
	auto n_valid_paths = 0;
	auto n_active_paths_cpy = this->n_active_paths;
	for (auto i = 0; i < n_active_paths_cpy; i++)
	{
		const auto path = this->paths[n_valid_paths];

		// check the CRC
		auto decode_result = crc_check(this->s[path]);

		// delete the path if the CRC result is negative
		if (!decode_result)
			this->delete_path(n_valid_paths, this->m);
		else
			n_valid_paths++;
	}

	this->Decoder_polar_SCL_fast_sys<B,R,API_polar>::select_best_path();

	return n_valid_paths;
}
