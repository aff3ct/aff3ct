#ifndef FACTORY_DECODER_HPP_
#define FACTORY_DECODER_HPP_

#include <string>

#include "Tools/Arguments_reader.hpp"

#include "../../Factory.hpp"

namespace aff3ct
{
namespace factory
{
struct Decoder : Factory
{
	static const std::string name;
	static const std::string prefix;

	struct parameters
	{
		virtual ~parameters() {}

		int         K           = 0;
		int         N_cw        = 0;

		std::string type        = "";
		std::string implem      = "";
		float       R           = -1.f;
		bool        systematic  = true;
		int         n_frames    = 1;
		int         tail_length = 0;
	};

	static void build_args(arg_map &req_args, arg_map &opt_args, const std::string p = prefix);
	static void store_args(const tools::Arguments_reader& ar, parameters& params, const std::string p = prefix);
	static void header(params_list& head_dec, const parameters& params);
};
}
}

#endif /* FACTORY_DECODER_HPP_ */
