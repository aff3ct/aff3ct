#ifndef FACTORY_FLIP_AND_CHECK_HPP
#define FACTORY_FLIP_AND_CHECK_HPP

#include <string>

#include "Tools/Code/Turbo/Post_processing_SISO/Flip_and_check/Flip_and_check.hpp"
#include "Tools/Arguments_reader.hpp"
#include "Tools/Header.hpp"

#include "../Factory.hpp"

namespace aff3ct
{
namespace tools
{
struct Factory_flip_and_check : public Factory
{
	struct parameters
	{
		bool enable              = false;
		int  q                   = 10;
		int  ite_min             = 3;
		int  ite_max             = 10;
		int  ite_step            = 1;
		int  start_crc_check_ite = 2;
		int  size;
		int  n_ite;
		int  n_frames;
	};

	template <typename B = int, typename Q = float>
	static Flip_and_check<B,Q>* build(const parameters& params, module::CRC<B> &crc, const int n_frames = 1);

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, parameters &params,
	                       const int size, const int n_ite, const int start_crc_check_ite = 2);
	static void group_args(Arguments_reader::arg_grp& ar);

	static void header(Header::params_list& head_fnc, const parameters& params);

};
}
}

#endif /* FACTORY_FLIP_AND_CHECK_HPP */
