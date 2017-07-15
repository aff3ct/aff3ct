#ifndef FACTORY_DECODER_HPP_
#define FACTORY_DECODER_HPP_

#include <string>

#include "Tools/Arguments_reader.hpp"

#include "../../Factory.hpp"

namespace aff3ct
{
namespace tools
{
struct Factory_decoder : Factory
{
	struct parameters
	{
		virtual ~parameters() {}

		int         K          = -1;
		int         N_cw       = -1;

		std::string type       = "";
		std::string implem     = "";
		float       R          = -1.f;
		bool        systematic = true;
		int         n_frames   = 1;
	};

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, parameters& params);
	static void group_args(Arguments_reader::arg_grp& ar);

	static void header(params_list& head_dec, const parameters& params);
};

} /* namespace tools */
} /* namespace aff3ct */

#endif /* FACTORY_DECODER_HPP_ */
