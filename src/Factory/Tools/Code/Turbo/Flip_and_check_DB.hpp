#ifndef FACTORY_FLIP_AND_CHECK_DB_HPP
#define FACTORY_FLIP_AND_CHECK_DB_HPP

#include <string>

#include "Tools/Code/Turbo/Post_processing_SISO/Flip_and_check/Flip_and_check_DB.hpp"

#include "Flip_and_check.hpp"

namespace aff3ct
{
namespace factory
{
struct Flip_and_check_DB : public Flip_and_check
{
	static const std::string name;
	static const std::string prefix;

	struct parameters : Flip_and_check::parameters
	{
		virtual ~parameters() {}
	};

	template <typename B = int, typename Q = float>
	static tools::Flip_and_check_DB<B,Q>* build(const parameters& params, module::CRC<B> &crc);

	static void build_args(arg_map &req_args, arg_map &opt_args, const std::string p = prefix);
	static void store_args(const arg_val_map &vals, parameters &params, const std::string p = prefix);
	static void make_header(params_list& head_fnc, const parameters& params, const bool full = true);

};
}
}

#endif /* FACTORY_FLIP_AND_CHECK_DB_HPP */
