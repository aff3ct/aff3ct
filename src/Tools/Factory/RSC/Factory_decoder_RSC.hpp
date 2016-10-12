#ifndef FACTORY_DECODER_RSC_HPP
#define FACTORY_DECODER_RSC_HPP

#include "Module/Decoder/Decoder.hpp"
#include "Module/Decoder/SISO.hpp"
#include "Tools/params.h"

#include "../Factory.hpp"

template <typename B, typename R, typename RD>
struct Factory_decoder_RSC : public Factory
{
	static SISO<R>* build_siso(const parameters &params, const std::vector<std::vector<int>> &trellis,
	                           std::ostream &stream = std::cout);

	static Decoder<B,R>* build(const parameters &params, const std::vector<std::vector<int>> &trellis);
};

#endif /* FACTORY_DECODER_RSC_HPP */
