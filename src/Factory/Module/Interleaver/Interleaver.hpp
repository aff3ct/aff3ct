#ifndef FACTORY_INTERLEAVER_HPP
#define FACTORY_INTERLEAVER_HPP

#include <cstdint>
#include <string>

#include "Tools/Interleaver/Interleaver_core.hpp"

#include "Factory/Tools/Interleaver/Interleaver_core.hpp"

#include "Module/Interleaver/Interleaver.hpp"

#include "../../Factory.hpp"

namespace aff3ct
{
namespace factory
{
struct Interleaver : public Factory
{
	static const std::string name;
	static const std::string prefix;

	class parameters : public Factory::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// depending parameters
		factory::Interleaver_core::parameters *core;

		// ---------------------------------------------------------------------------------------------------- METHODS
		parameters(const std::string p = Interleaver::prefix);
		virtual ~parameters();
		Interleaver::parameters* clone() const;

		// parameters construction
		void get_description(arg_map &req_args, arg_map &opt_args                              ) const;
		void store          (const arg_val_map &vals                                           );
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
