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
template <typename B = int>
struct Factory_encoder_common : public Factory
{
	struct encoder_parameters
	{
		virtual ~encoder_parameters() {}

		std::string type       = "NO";
		std::string path       = "";
		bool        systematic = true;

		// ------- code
		int K;
		int N;

		// ------- simu
		int n_frames = 1;
	};

	static module::Encoder<B>* build(const encoder_parameters &params, const int seed = 0);

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, encoder_parameters &params,
	                       const int K, const int N, const int n_frames = 1);
	static void group_args(Arguments_reader::arg_grp& ar);

	static void header(Header::params_list& head_enc, const encoder_parameters& params);
};
}
}

#endif /* FACTORY_ENCODER_COMMON_HPP */
