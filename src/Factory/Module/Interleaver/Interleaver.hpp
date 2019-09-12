#ifndef FACTORY_INTERLEAVER_HPP
#define FACTORY_INTERLEAVER_HPP

#include <string>
#include <map>

#include "Tools/Arguments/Argument_tools.hpp"
#include "Tools/auto_cloned_unique_ptr.hpp"
#include "Tools/Interleaver/Interleaver_core.hpp"
#include "Module/Interleaver/Interleaver.hpp"
#include "Factory/Tools/Interleaver/Interleaver_core.hpp"
#include "Factory/Factory.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Interleaver_name;
extern const std::string Interleaver_prefix;
struct Interleaver : public Factory
{
	class parameters : public Factory::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// depending parameters
		tools::auto_cloned_unique_ptr<factory::Interleaver_core::parameters> core;

		// ---------------------------------------------------------------------------------------------------- METHODS
		explicit parameters(const std::string &p = Interleaver_prefix);
		virtual ~parameters() = default;
		Interleaver::parameters* clone() const;

		// parameters construction
		void get_description(tools::Argument_map_info &args) const;
		void store          (const tools::Argument_map_value &vals);
		void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		template <typename D = int32_t>
		module::Interleaver<D>* build(const tools::Interleaver_core<>& itl_core) const;
	};

	template <typename D = int32_t>
	static module::Interleaver<D>* build(const tools::Interleaver_core<>& itl_core);
};
}
}

#endif /* FACTORY_INTERLEAVER_HPP */
