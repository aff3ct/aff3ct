#ifndef FACTORY_FLIP_AND_CHECK_HPP
#define FACTORY_FLIP_AND_CHECK_HPP

#include <string>

#include "Tools/Code/Turbo/Post_processing_SISO/Flip_and_check/Flip_and_check.hpp"
#include "Tools/Arguments_reader.hpp"

#include "../../../Factory.hpp"

namespace aff3ct
{
namespace factory
{
struct Flip_and_check : public Factory
{
	struct parameters
	{
		int  size                = -1;

		bool enable              = false;
		int  q                   = 10;
		int  n_ite               = 6;
		int  ite_min             = 3;
		int  ite_max             = 10;
		int  ite_step            = 1;
		int  start_crc_check_ite = 2;
		int  n_frames            = 1;
	};

	template <typename B = int, typename Q = float>
	static tools::Flip_and_check<B,Q>* build(const parameters& params, module::CRC<B> &crc);

	static void build_args(arg_map &req_args, arg_map &opt_args);
	static void store_args(const tools::Arguments_reader& ar, parameters &params);
	static void group_args(arg_grp& ar);

	static void header(params_list& head_fnc, const parameters& params);

};
}
}

#endif /* FACTORY_FLIP_AND_CHECK_HPP */
