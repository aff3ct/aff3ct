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

	class parameters : public Flip_and_check::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// empty

		// ---------------------------------------------------------------------------------------------------- METHODS
		parameters(const std::string p = Flip_and_check_DB::prefix);
		virtual ~parameters();
		Flip_and_check_DB::parameters* clone() const;

		// parameters construction
		virtual void get_description(arg_map &req_args, arg_map &opt_args                              ) const;
		virtual void store          (const arg_val_map &vals                                           );
		virtual void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		template <typename B = int, typename Q = float>
		tools::Flip_and_check_DB<B,Q>* build(module::CRC<B> &crc) const;
	};

	template <typename B = int, typename Q = float>
	static tools::Flip_and_check_DB<B,Q>* build(const parameters& params, module::CRC<B> &crc);
};
}
}

#endif /* FACTORY_FLIP_AND_CHECK_DB_HPP */
