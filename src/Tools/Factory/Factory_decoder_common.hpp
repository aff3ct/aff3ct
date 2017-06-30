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
	struct decoder_parameters
	{
		virtual ~decoder_parameters() {}

		std::string type   = "";
		std::string implem = "";
	};

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, decoder_parameters& params);
	static void group_args(Arguments_reader::arg_grp& ar);

	static void header(Header::params_list& head_dec, const decoder_parameters& params);
};

} /* namespace tools */
} /* namespace aff3ct */

#endif /* FACTORY_DECODER_COMMON_HPP_ */
