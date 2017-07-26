#ifndef FACTORY_ENCODER_HPP
#define FACTORY_ENCODER_HPP

#include <string>

#include "Module/Encoder/Encoder.hpp"

#include "../../Factory.hpp"

namespace aff3ct
{
namespace factory
{
struct Encoder : public Factory
{
	static const std::string name;
	static const std::string prefix;

	struct parameters
	{
		virtual ~parameters() {}

		int         K           = 0;
		int         N_cw        = 0;

		std::string type        = "NO";
		std::string path        = "";
		float       R           = -1.f;
		bool        systematic  = true;
		int         n_frames    = 1;
		int         seed        = 0;
		int         tail_length = 0;
	};

	template <typename B = int>
	static module::Encoder<B>* build(const parameters &params);

	static void build_args(arg_map &req_args, arg_map &opt_args, const std::string p = prefix);
	static void store_args(const arg_val_map &vals, parameters &params, const std::string p = prefix);
	static void make_header(params_list& head_enc, const parameters& params, const bool full = true);
};
}
}

#endif /* FACTORY_ENCODER_HPP */
