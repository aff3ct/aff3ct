#ifndef FACTORY_CRC_HPP
#define FACTORY_CRC_HPP

#include <string>
#include <sstream>

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
	struct CRC_parameters
	{
		std::string type          = "FAST";
		std::string poly          = "";
		bool        inc_code_rate = false;
		int         size          = 0;
		int         K             = 0;
		int         R             = 0;
		int         n_frames      = 1;
	};

	static module::CRC<B>* build(const CRC_parameters &params);

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, CRC_parameters &params, const int K, const int N, const int n_frames = 1);
	static void group_args(Arguments_reader::arg_grp& ar);

	static void header(Header::params_list& head_crc, const CRC_parameters& params);
};
}
}

#endif /* FACTORY_CRC_HPP */
