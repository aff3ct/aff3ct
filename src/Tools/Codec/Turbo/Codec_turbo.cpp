#include <stdexcept>

#include "Tools/Factory/Factory_interleaver.hpp"
#include "Tools/Factory/RSC/Factory_encoder_RSC.hpp"
#include "Tools/Factory/Turbo/Factory_encoder_turbo.hpp"
#include "Tools/Factory/Turbo/Factory_puncturer_turbo.hpp"
#include "Tools/Factory/Turbo/Factory_scaling_factor.hpp"
#include "Tools/Factory/RSC/Factory_decoder_RSC.hpp"
#include "Tools/Factory/Turbo/Factory_decoder_turbo.hpp"

#include "Codec_turbo.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B, typename Q, typename QD>
Codec_turbo<B,Q,QD>
::Codec_turbo(const parameters& params)
: Codec<B,Q>(params),
  sub_enc(this->params.simulation.n_threads, nullptr),
  siso   (this->params.simulation.n_threads, nullptr),
  sf     (this->params.simulation.n_threads, nullptr)
{
	if (!params.simulation.json_path.empty())
	{
		if (this->params.simulation.n_threads != 1)
			throw std::runtime_error("aff3ct::tools::Codec_turbo: only single-threaded simulation is "
			                         "available with JSON trace.");

		json_stream.open(params.simulation.json_path.c_str(), std::ios::out | std::ios::trunc);

		json_stream << "[" << std::endl;
	}

	auto encoder_RSC = Factory_encoder_RSC<B>::build(this->params);
	if (encoder_RSC != nullptr)
	{
		trellis = encoder_RSC->get_trellis();
		delete encoder_RSC;
	}
	else
		throw std::runtime_error("aff3ct::tools::Codec_turbo: \"trellis\" can't be created.");
}

template <typename B, typename Q, typename QD>
Codec_turbo<B,Q,QD>
::~Codec_turbo()
{
	int tid = 0;
	const int nthr = this->params.simulation.n_threads;
	for (tid = 0; tid < nthr; tid++) if (sub_enc[tid] != nullptr) { delete sub_enc[tid]; sub_enc[tid] = nullptr; }
	for (tid = 0; tid < nthr; tid++) if (siso   [tid] != nullptr) { delete siso   [tid]; siso   [tid] = nullptr; }
	for (tid = 0; tid < nthr; tid++) if (sf     [tid] != nullptr) { delete sf     [tid]; sf     [tid] = nullptr; }

	if (json_stream.is_open())
	{
		json_stream << "[{\"stage\": \"end\"}]]" << std::endl;
		json_stream.close();
	}
}

template <typename B, typename Q, typename QD>
Interleaver<int>* Codec_turbo<B,Q,QD>
::build_interleaver(const int tid, const int seed)
{
	return Factory_interleaver<int>::build(this->params, this->params.code.K, seed);
}

template <typename B, typename Q, typename QD>
Encoder<B>* Codec_turbo<B,Q,QD>
::build_encoder(const int tid, const module::Interleaver<int>* itl)
{
	if (itl == nullptr)
		throw std::runtime_error("aff3ct::tools::Codec_turbo: \"itl\" should not be null.");

	if (sub_enc[tid] != nullptr)
	{
		delete sub_enc[tid];
		sub_enc[tid] = nullptr;
	}

	sub_enc[tid] = Factory_encoder_RSC<B>::build(this->params, json_stream);

	if (sub_enc[tid] == nullptr)
		throw std::runtime_error("aff3ct::tools::Codec_turbo: \"sub_enc\" can't be created.");

	return Factory_encoder_turbo<B>::build(this->params, itl, sub_enc[tid], sub_enc[tid]);
}

template <typename B, typename Q, typename QD>
Puncturer<B,Q>* Codec_turbo<B,Q,QD>
::build_puncturer(const int tid)
{
	return Factory_puncturer_turbo<B,Q>::build(this->params);
}

template <typename B, typename Q, typename QD>
Decoder<B,Q>* Codec_turbo<B,Q,QD>
::build_decoder(const int tid, const module::Interleaver<int>* itl, module::CRC<B>* crc)
{
	if (itl == nullptr)
		throw std::runtime_error("aff3ct::tools::Codec_turbo: \"itl\" should not be null.");

	if (sf[tid] != nullptr)
	{
		delete sf[tid];
		sf[tid] = nullptr;
	}

	sf[tid] = Factory_scaling_factor<Q>::build(this->params);

	if (sf[tid] == nullptr)
		throw std::runtime_error("aff3ct::tools::Codec_turbo: \"sf\" can't be created.");

	if (siso[tid] != nullptr)
	{
		delete siso[tid];
		siso[tid] = nullptr;
	}

	siso[tid] = Factory_decoder_RSC<B,Q,QD>::build_siso(this->params, trellis, json_stream);

	if (siso[tid] == nullptr)
		throw std::runtime_error("aff3ct::tools::Codec_turbo: \"siso\" can't be created.");

	return Factory_decoder_turbo<B,Q>::build(this->params, itl, siso[tid], siso[tid], sf[tid], crc);
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::tools::Codec_turbo<B_8,Q_8,QD_8>;
template class aff3ct::tools::Codec_turbo<B_16,Q_16,QD_16>;
template class aff3ct::tools::Codec_turbo<B_32,Q_32,QD_32>;
template class aff3ct::tools::Codec_turbo<B_64,Q_64,QD_64>;
#else
template class aff3ct::tools::Codec_turbo<B,Q,QD>;
#endif
// ==================================================================================== explicit template instantiation
