#ifndef FACTORY_CRC_HPP
#define FACTORY_CRC_HPP

#include <string>

#include "Module/CRC/CRC.hpp"

#include "Factory.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int>
struct Factory_CRC : public Factory
{
	static module::CRC<B>* build(const std::string type,
	                             const int         K,
	                             const int         size,
	                             const std::string poly,
	                             const int         n_frames = 1);

	static void build_args(tools::Arguments_reader::arg_map &req_args, tools::Arguments_reader::arg_map &opt_args);
	static void store_args(const tools::Arguments_reader& ar, tools::parameters &params);
	static void group_args(tools::Arguments_reader::arg_grp& ar);
};
}
}

#endif /* FACTORY_CRC_HPP */
