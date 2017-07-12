#ifndef FACTORY_ENCODER_COMMON_HPP
#define FACTORY_ENCODER_COMMON_HPP

#include <string>

#include "Tools/Arguments_reader.hpp"
#include "Tools/Header.hpp"
#include "Module/Encoder/Encoder.hpp"

#include "Factory.hpp"

namespace aff3ct
{
namespace tools
{
struct Factory_encoder_common : public Factory
{
	struct parameters
	{
		virtual ~parameters() {}

		std::string type       = "NO";
		std::string path       = "";
		bool        systematic = true;

		// ------- code
		int         K;
		int         N;
		bool        coset      = false;

		// ------- simu
		int         n_frames   = 1;
	};

	template <typename B = int>
	static module::Encoder<B>* build(const parameters &params, const int seed = 0);

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, parameters &params,
	                       const int K, const int N, const int n_frames = 1);
	static void group_args(Arguments_reader::arg_grp& ar);

	static void header(Header::params_list& head_enc, Header::params_list& head_cde, const parameters& params);
};
}
}

#endif /* FACTORY_ENCODER_COMMON_HPP */
