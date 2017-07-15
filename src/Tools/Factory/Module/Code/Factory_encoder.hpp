#ifndef FACTORY_ENCODER_HPP
#define FACTORY_ENCODER_HPP

#include <string>

#include "Tools/Arguments_reader.hpp"

#include "Module/Encoder/Encoder.hpp"

#include "../../Factory.hpp"

namespace aff3ct
{
namespace tools
{
struct Factory_encoder : public Factory
{
	struct parameters
	{
		virtual ~parameters() {}

		int         K          = -1;
		int         N_cw       = -1;

		std::string type       = "NO";
		std::string path       = "";
		float       R          = -1.f;
		bool        systematic = true;
		bool        coset      = false;
		int         n_frames   = 1;
		int         seed       = 0;
	};

	template <typename B = int>
	static module::Encoder<B>* build(const parameters &params);

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, parameters &params);
	static void group_args(Arguments_reader::arg_grp& ar);

	static void header(params_list& head_enc, const parameters& params);
};
}
}

#endif /* FACTORY_ENCODER_HPP */
