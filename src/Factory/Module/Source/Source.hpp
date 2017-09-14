#ifndef FACTORY_SOURCE_HPP
#define FACTORY_SOURCE_HPP

#include <string>

#include "Module/Source/Source.hpp"

#include "../../Factory.hpp"

namespace aff3ct
{
namespace factory
{
struct Source : public Factory
{
	static const std::string name;
	static const std::string prefix;

	class parameters : public Factory::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// required parameters
		int         K        = 0;

		// optional parameters
		std::string type     = "RAND";
		std::string path     = "";
		int         n_frames = 1;
		int         seed     = 0;

		// ---------------------------------------------------------------------------------------------------- METHODS
		parameters(const std::string p = Source::prefix);
		virtual ~parameters();
		Source::parameters* clone() const;

		// parameters construction
		virtual void get_description(arg_map &req_args, arg_map &opt_args                              ) const;
		virtual void store          (const arg_val_map &vals                                           );
		virtual void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		template <typename B = int>
		module::Source<B>* build() const;
	};

	template <typename B = int>
	static module::Source<B>* build(const parameters &params);
};
}
}

#endif /* FACTORY_SOURCE_HPP */
