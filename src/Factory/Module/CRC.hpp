#ifndef FACTORY_CRC_HPP
#define FACTORY_CRC_HPP

#include <string>
#include <sstream>

#include "Module/CRC/CRC.hpp"

#include "../Factory.hpp"

namespace aff3ct
{
namespace factory
{
struct CRC : public Factory
{
	static const std::string name;
	static const std::string prefix;

	struct parameters
	{
		int         K             = 0;

		std::string type          = "FAST";
		std::string poly          = "";
		int         size          = 0;
		int         n_frames      = 1;
	};

	template <typename B = int>
	static module::CRC<B>* build(const parameters &params);

	static void build_args(arg_map &req_args, arg_map &opt_args, const std::string p = prefix);
	static void store_args(const arg_val_map &vals, parameters &params, const std::string p = prefix);
	static void header(params_list& head_crc, const parameters& params);
};
}
}

#endif /* FACTORY_CRC_HPP */
