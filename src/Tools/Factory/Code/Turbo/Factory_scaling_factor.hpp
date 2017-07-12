#ifndef FACTORY_SCALING_FACTOR_HPP
#define FACTORY_SCALING_FACTOR_HPP

#include <string>

#include "Tools/Code/Turbo/Post_processing_SISO/Scaling_factor/Scaling_factor.hpp"
#include "Tools/Arguments_reader.hpp"
#include "Tools/Header.hpp"

#include "../../Factory.hpp"

namespace aff3ct
{
namespace tools
{
struct Factory_scaling_factor : public Factory
{
	struct parameters
	{
		std::string         declaration;
		std::string         type;
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
	static Scaling_factor<B,Q>* build(const parameters& params, const int n_ite);

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, parameters &params);
	static void group_args(Arguments_reader::arg_grp& ar);

	static void header(Header::params_list& head_sf, const parameters& params);
};
}
}

#endif /* FACTORY_SCALING_FACTOR_HPP */
