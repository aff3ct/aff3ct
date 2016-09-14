#ifndef FACTORY_DECODER_RSC_HPP
#define FACTORY_DECODER_RSC_HPP

#include "Tools/params.h"

#include "Module/Decoder/Decoder.hpp"
#include "Module/Decoder/SISO.hpp"

template <typename B, typename R, typename RD>
struct Factory_decoder_RSC
{
	static SISO<R>* build_siso(const parameters &params, const std::vector<std::vector<int>> &trellis);

	static Decoder<B,R>* build(const parameters &params, const std::vector<std::vector<int>> &trellis);
};

#endif /* FACTORY_DECODER_RSC_HPP */
