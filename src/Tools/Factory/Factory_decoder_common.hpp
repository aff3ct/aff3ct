#ifndef FACTORY_DECODER_COMMON_HPP_
#define FACTORY_DECODER_COMMON_HPP_

#include <string>

#include "Tools/Arguments_reader.hpp"
#include "Tools/Header.hpp"
#include "Factory.hpp"

namespace aff3ct
{
namespace tools
{
struct Factory_decoder_common : Factory
{
	struct parameters
	{
		virtual ~parameters() {}

		std::string type   = "";
		std::string implem = "";

		// ------- code
		int K;
		int N;

		// ------- simu
		int n_frames = 1;
	};

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, parameters& params,
	                       const int K, const int N, const int n_frames = 1);
	static void group_args(Arguments_reader::arg_grp& ar);

	static void header(Header::params_list& head_dec, const parameters& params);
};

} /* namespace tools */
} /* namespace aff3ct */

#endif /* FACTORY_DECODER_COMMON_HPP_ */
