#ifndef FACTORY_FLIP_AND_CHECK_DB_HPP
#define FACTORY_FLIP_AND_CHECK_DB_HPP

#include <string>
#include <map>

#include "Tools/Arguments/Argument_tools.hpp"
#include "Tools/Code/Turbo/Post_processing_SISO/Flip_and_check/Flip_and_check_DB.hpp"
#include "Module/CRC/CRC.hpp"
#include "Factory/Tools/Code/Turbo/Flip_and_check.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Flip_and_check_DB_name;
extern const std::string Flip_and_check_DB_prefix;
struct Flip_and_check_DB : public Flip_and_check
{
	class parameters : public Flip_and_check::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// empty

		// ---------------------------------------------------------------------------------------------------- METHODS
		explicit parameters(const std::string &p = Flip_and_check_DB_prefix);
		virtual ~parameters() = default;
		Flip_and_check_DB::parameters* clone() const;

		// parameters construction
		virtual void get_description(tools::Argument_map_info &args) const;
		virtual void store          (const tools::Argument_map_value &vals);
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
