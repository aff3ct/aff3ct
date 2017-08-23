#ifndef FACTORY_SCALING_FACTOR_HPP
#define FACTORY_SCALING_FACTOR_HPP

#include <string>

#include "Tools/Code/Turbo/Post_processing_SISO/Scaling_factor/Scaling_factor.hpp"

#include "../../../Factory.hpp"

namespace aff3ct
{
namespace factory
{
struct Scaling_factor : public Factory
{
	static const std::string name;
	static const std::string prefix;

	struct parameters
	{
		template <typename B = int, typename Q = float>
		tools::Scaling_factor<B,Q>* build() const;

		std::string         type        = "LTE_VEC";
		int                 n_ite       = 6;
		bool                enable      = false;
		float               cst         = 0.75f;
		mipp::vector<float> alpha_array = {0.15f, 0.15f,  // ite 1
		                                   0.25f, 0.25f,  // ite 2
		                                   0.30f, 0.30f,  // ite 3
		                                   0.40f, 0.40f,  // ite 4
		                                   0.70f, 0.70f,  // ite 5
		                                   0.80f, 0.80f,  // ite 6
		                                   0.90f, 0.90f,  // ite 7
		                                   0.95f, 0.95f}; // ite 8
	};

	template <typename B = int, typename Q = float>
	static tools::Scaling_factor<B,Q>* build(const parameters& params);

	static void build_args(arg_map &req_args, arg_map &opt_args, const std::string p = prefix);
	static void store_args(const arg_val_map &vals, parameters &params, const std::string p = prefix);
	static void make_header(params_list& head_sf, const parameters& params, const bool full = true);
};
}
}

#endif /* FACTORY_SCALING_FACTOR_HPP */
