#ifndef FACTORY_DECODER_POLAR_GEN_HPP
#define FACTORY_DECODER_POLAR_GEN_HPP

#include <string>
#include <mipp.h>

#include "Module/Decoder/Decoder_SIHO.hpp"
#include "Module/CRC/CRC.hpp"

#include "Decoder_polar.hpp"

namespace aff3ct
{
namespace factory
{
struct Decoder_polar_gen : public Decoder_polar
{
	static const std::string name;
	static const std::string prefix;

	template <typename B = int, typename Q = float>
	static module::Decoder_SIHO<B,Q>* build(const parameters        &params,
	                                        const std::vector<bool> &frozen_bits,
	                                              module::CRC<B>    *crc = nullptr);

	template <typename B>
	static void get_frozen_bits(const std::string        implem,
	                            const int                N,
	                                  std::vector<bool> &frozen_bits);

	static void build_args(arg_map &req_args, arg_map &opt_args, const std::string p = prefix);
	static void store_args(const arg_val_map &vals, parameters &params, const std::string p = prefix);
	static void make_header(params_list& head_dec, const parameters& params, const bool full = true);

private:
	template <typename B = int, typename Q = float, class API_polar>
	static module::Decoder_SIHO<B,Q>* _build(const parameters        &params,
	                                         const std::vector<bool> &frozen_bits,
	                                               module::CRC<B>    *crc = nullptr);
};
}
}

#endif /* FACTORY_DECODER_POLAR_GEN_HPP */
