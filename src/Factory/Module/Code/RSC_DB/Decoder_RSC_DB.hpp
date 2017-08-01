#ifndef FACTORY_DECODER_RSC_DB_HPP
#define FACTORY_DECODER_RSC_DB_HPP

#include <string>
#include <vector>
#include <iostream>
#include <sstream>

#include "Tools/Math/max.h"

#include "Module/Decoder/RSC_DB/BCJR/Decoder_RSC_DB_BCJR.hpp"

#include "../Decoder.hpp"

namespace aff3ct
{
namespace factory
{
struct Decoder_RSC_DB : public Decoder
{
	static const std::string name;
	static const std::string prefix;

	struct parameters : Decoder::parameters
	{
		virtual ~parameters() {}

		std::string max      = "MAX";
		bool        buffered = true;
	};

	template <typename B = int, typename Q = float>
	static module::Decoder_RSC_DB_BCJR<B,Q>* build(const parameters &dec_par, const std::vector<std::vector<int>> &trellis);

	static void build_args(arg_map &req_args, arg_map &opt_args, const std::string p = prefix);
	static void store_args(const arg_val_map &vals, parameters &params, const std::string p = prefix);
	static void make_header(params_list& head_dec, const parameters& params, const bool full = true);

private:
	template <typename B = int, typename Q = float, tools::proto_max<Q> MAX>
	static module::Decoder_RSC_DB_BCJR<B,Q>* _build(const parameters &dec_par, const std::vector<std::vector<int>> &trellis);
};
}
}

#endif /* FACTORY_DECODER_RSC_DB_HPP */
