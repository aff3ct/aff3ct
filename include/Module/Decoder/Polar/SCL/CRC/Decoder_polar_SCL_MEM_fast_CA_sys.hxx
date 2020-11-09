#include <string>
#include <sstream>
#include <algorithm>

#include "Tools/Exception/exception.hpp"
#include "Tools/Code/Polar/fb_extract.h"

#include "Module/Decoder/Polar/SCL/CRC/Decoder_polar_SCL_MEM_fast_CA_sys.hpp"

namespace aff3ct
{
namespace module
{
template <typename B, typename R, class API_polar>
Decoder_polar_SCL_MEM_fast_CA_sys<B,R,API_polar>
::Decoder_polar_SCL_MEM_fast_CA_sys(const int& K, const int& N, const int& L, const std::vector<bool>& frozen_bits,
                                    const CRC<B>& crc)
: Decoder_polar_SCL_MEM_fast_sys<B,R,API_polar>(K, N, L, frozen_bits),
  fast_store(false), crc(crc.clone()), U_test(K)
{
	const std::string name = "Decoder_polar_SCL_MEM_fast_CA_sys";
	this->set_name(name);

	if (this->crc->get_size() > K)
	{
		std::stringstream message;
		message << "'crc->get_size()' has to be equal or smaller than 'K' ('crc->get_size()' = " << this->crc->get_size()
		        << ", 'K' = " << K << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename B, typename R, class API_polar>
Decoder_polar_SCL_MEM_fast_CA_sys<B,R,API_polar>
::Decoder_polar_SCL_MEM_fast_CA_sys(const int& K, const int& N, const int& L, const std::vector<bool>& frozen_bits,
                                    const std::vector<tools::Pattern_polar_i*> &polar_patterns,
                                    const int idx_r0, const int idx_r1,
                                    const CRC<B>& crc)
: Decoder_polar_SCL_MEM_fast_sys<B,R,API_polar>(K, N, L, frozen_bits, polar_patterns, idx_r0, idx_r1),
  fast_store(false), crc(crc.clone()), U_test(K)
{
	const std::string name = "Decoder_polar_SCL_MEM_fast_CA_sys";
	this->set_name(name);

	if (this->crc->get_size() > K)
	{
		std::stringstream message;
		message << "'crc->get_size()' has to be equal or smaller than 'K' ('crc->get_size()' = " << this->crc->get_size()
		        << ", 'K' = " << K << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename B, typename R, class API_polar>
Decoder_polar_SCL_MEM_fast_CA_sys<B,R,API_polar>* Decoder_polar_SCL_MEM_fast_CA_sys<B,R,API_polar>
::clone() const
{
	auto m = new Decoder_polar_SCL_MEM_fast_CA_sys(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SCL_MEM_fast_CA_sys<B,R,API_polar>
::deep_copy(const Decoder_polar_SCL_MEM_fast_CA_sys<B,R,API_polar> &m)
{
	Decoder_polar_SCL_MEM_fast_sys<B,R,API_polar>::deep_copy(m);
	if (m.crc != nullptr) this->crc.reset(m.crc->clone());
}

template <typename B, typename R, class API_polar>
bool Decoder_polar_SCL_MEM_fast_CA_sys<B,R,API_polar>
::crc_check(mipp::vector<B> &s, const size_t frame_id)
{
	tools::fb_extract(this->polar_patterns.get_leaves_pattern_types(), s.data(), U_test.data());

	// check the CRC
	return crc->check(U_test, frame_id);
}

template <typename B, typename R, class API_polar>
int Decoder_polar_SCL_MEM_fast_CA_sys<B,R,API_polar>
::select_best_path(const size_t frame_id)
{
	std::sort(this->paths.begin(), this->paths.begin() + this->n_active_paths,
		[this](int x, int y){
			return this->metrics[x] < this->metrics[y];
		});

	auto i = 0;
	while (i < this->n_active_paths && !crc_check(this->s[this->paths[i]], frame_id)) i++;

	this->best_path = (i == this->n_active_paths) ? this->paths[0] : this->paths[i];
	fast_store = i != this->n_active_paths;

	return this->n_active_paths -i;
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SCL_MEM_fast_CA_sys<B,R,API_polar>
::init_buffers()
{
	Decoder_polar_SCL_MEM_fast_sys<B,R,API_polar>::init_buffers();
	fast_store = false;
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SCL_MEM_fast_CA_sys<B,R,API_polar>
::_store(B *V_K) const
{
	if (fast_store)
		std::copy(U_test.begin(), U_test.begin() + this->K, V_K);
	else
		Decoder_polar_SCL_MEM_fast_sys<B,R,API_polar>::_store(V_K);
}
}
}
