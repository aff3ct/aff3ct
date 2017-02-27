#ifndef FACTORY_DECODER_LDPC_HPP
#define FACTORY_DECODER_LDPC_HPP

#include <string>

#include "Tools/params.h"
#include "Tools/Code/LDPC/AList_reader/AList_reader.hpp"

#include "Module/Decoder/Decoder.hpp"
#include "Module/Decoder/Decoder_SISO.hpp"

#include "../Factory.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename R = float>
struct Factory_decoder_LDPC : public Factory
{
	static module::Decoder_SISO<B,R>* build(const parameters &params, const AList_reader &alist_data);
};
}
}

#endif /* FACTORY_DECODER_LDPC_HPP */
