#include <sstream>

#include "Tools/Noise/noise_utils.h"
#include "Tools/Exception/exception.hpp"
#include "Factory/Module/Interleaver/Interleaver.hpp"
#include "Tools/Codec/Codec.hpp"

namespace aff3ct
{
namespace tools
{

template <typename B, typename Q>
Codec<B,Q>
::Codec(const int K, const int N_cw, const int N)
: K(K), N_cw(N_cw), N(N), noise(nullptr)
{
	if (K <= 0)
	{
		std::stringstream message;
		message << "'K' has to be greater than 0 ('K' = " << K << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (N <= 0)
	{
		std::stringstream message;
		message << "'N' has to be greater than 0 ('N' = " << N << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (N_cw <= 0)
	{
		std::stringstream message;
		message << "'N_cw' has to be greater than 0 ('N_cw' = " << N_cw << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (K > N)
	{
		std::stringstream message;
		message << "'K' has to be smaller or equal to 'N' ('K' = " << K << ", 'N' = " << N << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (N > N_cw)
	{
		std::stringstream message;
		message << "'N' has to be smaller or equal to 'N_cw' ('N' = " << N << ", 'N_cw' = " << N_cw << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename B, typename Q>
Codec<B,Q>* Codec<B,Q>
::clone() const
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q>
void Codec<B,Q>
::deep_copy(const Codec<B,Q> &t)
{
	if (t.encoder   != nullptr) this->encoder  .reset(t.encoder  ->clone());
	if (t.puncturer != nullptr) this->puncturer.reset(t.puncturer->clone());
	if (t.extractor != nullptr) this->extractor.reset(t.extractor->clone());
}

template <typename B, typename Q>
Interleaver_core<>& Codec<B,Q>
::get_interleaver()
{
	if (this->interleaver_core == nullptr)
	{
		std::stringstream message;
		message << "'interleaver_core' is NULL.";
		throw runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	return *this->interleaver_core;
}

template <typename B, typename Q>
module::Encoder<B>& Codec<B,Q>
::get_encoder()
{
	if (this->encoder == nullptr)
	{
		std::stringstream message;
		message << "'encoder' is NULL.";
		throw runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	return *this->encoder;
}

template <typename B, typename Q>
module::Puncturer<B,Q>& Codec<B,Q>
::get_puncturer()
{
	if (this->puncturer == nullptr)
	{
		std::stringstream message;
		message << "'puncturer' is NULL.";
		throw runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	return *this->puncturer;
}

template <typename B, typename Q>
module::Extractor<B,Q>& Codec<B,Q>
::get_extractor()
{
	if (this->extractor == nullptr)
	{
		std::stringstream message;
		message << "'extractor' is NULL.";
		throw runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	return *this->extractor;
}

template <typename B, typename Q>
const Noise<>& Codec<B,Q>
::get_noise() const
{
	if (this->noise == nullptr)
	{
		std::stringstream message;
		message << "'noise' should not be nullptr.";
		throw runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	return *this->noise;
}

template <typename B, typename Q>
void Codec<B,Q>
::set_noise(const Noise<>& noise)
{
	this->noise = &noise;
	if (this->noise->is_set())
		this->notify_noise_update();
}

template <typename B, typename Q>
void Codec<B,Q>
::notify_noise_update()
{
	this->check_noise();
}

template <typename B, typename Q>
size_t Codec<B,Q>
::get_n_frames() const
{
	if (this->encoder == nullptr)
	{
		std::stringstream message;
		message << "'encoder' is NULL.";
		throw runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	return this->encoder->get_n_frames();
}

template <typename B, typename Q>
void Codec<B,Q>
::set_n_frames(const size_t n_frames)
{
	if (this->interleaver_core != nullptr)
		this->interleaver_core->set_n_frames(n_frames);
	if (this->interleaver_bit != nullptr)
		this->interleaver_bit->set_n_frames(n_frames);
	if (this->interleaver_llr != nullptr)
		this->interleaver_llr->set_n_frames(n_frames);
	if (this->encoder != nullptr)
		this->encoder->set_n_frames(n_frames);
	if (this->puncturer != nullptr)
		this->puncturer->set_n_frames(n_frames);
	if (this->extractor != nullptr)
		this->extractor->set_n_frames(n_frames);
}

template <typename B, typename Q>
void Codec<B,Q>
::check_noise()
{
	if (this->noise == nullptr)
	{
		std::stringstream message;
		message << "'noise' should not be nullptr.";
		throw runtime_error(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename B, typename Q>
void Codec<B,Q>
::set_interleaver(Interleaver_core<>* itl)
{
	this->set_interleaver(std::unique_ptr<Interleaver_core<>>(itl));
}

template <typename B, typename Q>
void Codec<B,Q>
::set_encoder(module::Encoder<B>* enc)
{
	this->set_encoder(std::unique_ptr<module::Encoder<B>>(enc));
}

template <typename B, typename Q>
void Codec<B,Q>
::set_puncturer(module::Puncturer<B,Q>* pct)
{
	this->set_puncturer(std::unique_ptr<module::Puncturer<B,Q>>(pct));
}

template <typename B, typename Q>
void Codec<B,Q>
::set_extractor(module::Extractor<B,Q>* ext)
{
	this->set_extractor(std::unique_ptr<module::Extractor<B,Q>>(ext));
}


template <typename B, typename Q>
void Codec<B,Q>
::set_interleaver(std::unique_ptr<Interleaver_core<>>&& itl)
{
	this->interleaver_core = std::move(itl);
	this->interleaver_bit.reset(factory::Interleaver::build<B>(*this->interleaver_core));
	this->interleaver_llr.reset(factory::Interleaver::build<Q>(*this->interleaver_core));
}

template <typename B, typename Q>
void Codec<B,Q>
::set_encoder(std::unique_ptr<module::Encoder<B>>&& enc)
{
	this->encoder = std::move(enc);
}

template <typename B, typename Q>
void Codec<B,Q>
::set_puncturer(std::unique_ptr<module::Puncturer<B,Q>>&& pct)
{
	this->puncturer = std::move(pct);
}

template <typename B, typename Q>
void Codec<B,Q>
::set_extractor(std::unique_ptr<module::Extractor<B,Q>>&& ext)
{
	this->extractor = std::move(ext);
}

template <typename B, typename Q>
module::Interleaver<B>& Codec<B,Q>
::get_interleaver_bit()
{
	return *this->interleaver_bit;
}

template <typename B, typename Q>
module::Interleaver<Q>& Codec<B,Q>
::get_interleaver_llr()
{
	return *this->interleaver_llr;
}

}
}
