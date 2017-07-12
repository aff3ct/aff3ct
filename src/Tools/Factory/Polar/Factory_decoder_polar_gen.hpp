#ifndef FACTORY_DECODER_POLAR_GEN_HPP
#define FACTORY_DECODER_POLAR_GEN_HPP

#include <string>
#include <mipp.h>

#include "Module/Decoder/Decoder.hpp"
#include "Module/CRC/CRC.hpp"

#include "Factory_decoder_polar.hpp"

namespace aff3ct
{
namespace tools
{
struct Factory_decoder_polar_gen : public Factory_decoder_polar
{
	template <typename B = int, typename Q = float>
	static module::Decoder<B,Q>* build(const Factory_decoder_polar::parameters &params,
	                                   const mipp::vector<B>                   &frozen_bits,
	                                   const bool                               sys_encoding = true,
	                                         module::CRC<B>                    *crc = nullptr);

	template <typename B>
	static void get_frozen_bits(const std::string      implem,
	                            const int              N,
	                                  mipp::vector<B> &frozen_bits);

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, Factory_decoder_polar::parameters &params,
	                       const int K, const int N, const int n_frames = 1);
	static void group_args(Arguments_reader::arg_grp& ar);

	static void header(Header::params_list& head_dec, Header::params_list& head_cde,
	                   const Factory_decoder_polar::parameters& params);

private:
	template <typename B = int, typename Q = float, class API_polar>
	static module::Decoder<B,Q>* _build(const Factory_decoder_polar::parameters &params,
	                                    const mipp::vector<B>                   &frozen_bits,
	                                          module::CRC<B>                    *crc = nullptr);
};
}
}

#endif /* FACTORY_DECODER_POLAR_GEN_HPP */
