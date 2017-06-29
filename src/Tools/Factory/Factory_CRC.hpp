#ifndef FACTORY_CRC_HPP
#define FACTORY_CRC_HPP

#include <string>

#include "Module/CRC/CRC.hpp"
#include "Tools/Header.hpp"
#include "Tools/Arguments_reader.hpp"

#include "Factory.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int>
struct Factory_CRC : public Factory
{
	struct CRC_params
	{
		std::string type          = "FAST";
		std::string poly          = "";
		bool        inc_code_rate = false;
		int         size          = 0;
		int         K_info        = 0;
		int         R             = 0;
	};

	static module::CRC<B>* build(const std::string type,
	                             const int         K,
	                             const int         size,
	                             const std::string poly,
	                             const int         n_frames = 1);

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, CRC_params &params, const int K, const int N);
	static void group_args(Arguments_reader::arg_grp& ar);
};
}
}

#endif /* FACTORY_CRC_HPP */
