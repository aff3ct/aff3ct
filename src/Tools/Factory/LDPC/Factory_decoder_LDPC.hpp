#ifndef FACTORY_DECODER_LDPC_HPP
#define FACTORY_DECODER_LDPC_HPP

#include <string>

#include "Module/Decoder/Decoder.hpp"
#include "Module/Decoder/Decoder_SISO.hpp"
#include "Tools/params.h"

#include "../Factory.hpp"

template <typename B, typename R>
struct Factory_decoder_LDPC : public Factory
{
	static Decoder_SISO<B,R>* build(const parameters                  &params,
	                                const mipp::vector<unsigned char> &n_variables_per_parity,
	                                const mipp::vector<unsigned char> &n_parities_per_variable,
	                                const mipp::vector<unsigned int > &transpose);
};

#endif /* FACTORY_DECODER_LDPC_HPP */
