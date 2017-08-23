#ifndef FACTORY_ENCODER_POLAR_HPP
#define FACTORY_ENCODER_POLAR_HPP

#include <string>
#include <vector>

#include "Module/Encoder/Encoder.hpp"

#include "../Encoder.hpp"

namespace aff3ct
{
namespace factory
{
struct Encoder_polar : public Encoder
{
	static const std::string name;
	static const std::string prefix;

	struct parameters : Encoder::parameters
	{
		virtual ~parameters() {}

		template <typename B = int>
		module::Encoder<B>* build(const std::vector<bool> &frozen_bits) const;
	};

	template <typename B = int>
	static module::Encoder<B>* build(const parameters        &params,
	                                 const std::vector<bool> &frozen_bits);

	static void build_args(arg_map &req_args, arg_map &opt_args, const std::string p = prefix);
	static void store_args(const arg_val_map &vals, parameters &params, const std::string p = prefix);
	static void make_header(params_list& head_enc, const parameters& params, const bool full = true);
};
}
}

#endif /* FACTORY_ENCODER_POLAR_HPP */
