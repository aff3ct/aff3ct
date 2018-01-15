#ifndef CODEC_SISO_SIHO_HPP_
#define CODEC_SISO_SIHO_HPP_

#include "Module/Decoder/Decoder_SISO.hpp"

#include "Codec_SISO.hpp"
#include "Codec_SIHO.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename Q = float>
class Codec_SISO_SIHO : public Codec_SISO<B,Q>, public Codec_SIHO<B,Q>
{
public:
	Codec_SISO_SIHO(const int K, const int N_cw, const int N, const int tail_length = 0, const int n_frames = 1)
	: Codec     <B,Q>(K, N_cw, N, tail_length, n_frames),
	  Codec_SISO<B,Q>(K, N_cw, N, tail_length, n_frames),
	  Codec_SIHO<B,Q>(K, N_cw, N, tail_length, n_frames)
	{
		const std::string name = "Codec_SISO_SIHO";
		this->set_name(name);
	}

	virtual ~Codec_SISO_SIHO()
	{
		Decoder_SISO<Q  > *siso = nullptr;
		Decoder_SIHO<B,Q> *siho = nullptr;

		try
		{
			siso = this->get_decoder_siso();
		} catch (const std::exception&)
		{
		}

		try
		{
			siho = this->get_decoder_siho();
		} catch (const std::exception&)
		{
		}

		if      (siso == nullptr && siho != nullptr) { delete siho; this->set_decoder_siho(nullptr); }
		else if (siho == nullptr && siso != nullptr) { delete siso; this->set_decoder_siso(nullptr); }
		else if (siho != nullptr && siso != nullptr)
		{
			// do not delete the siso if the siho and the siso are the same pointers
			if (dynamic_cast<module::Decoder*>(siso) != dynamic_cast<module::Decoder*>(siho))
				delete siso;
			delete siho;
			this->set_decoder_siho(nullptr);
			this->set_decoder_siso(nullptr);
		}
	}

	virtual void reset()
	{
		this->get_decoder_siho()->reset();
	}
};
}
}

#endif /* CODEC_SISO_SIHO_HPP_ */
