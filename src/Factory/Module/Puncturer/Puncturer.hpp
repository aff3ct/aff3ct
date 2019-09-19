#ifndef FACTORY_PONCTURER_HPP_
#define FACTORY_PONCTURER_HPP_

#include <string>
#include <map>

#include "Tools/Arguments/Argument_tools.hpp"
#include "Module/Puncturer/Puncturer.hpp"
#include "Factory/Factory.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Puncturer_name;
extern const std::string Puncturer_prefix;
struct Puncturer : Factory
{
	class parameters : public Factory::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// required parameters
		int         K        = 0;
		int         N        = 0;

		// optional parameters
		std::string type     = "NO";
		int         N_cw     = 0;
		int         n_frames = 1;

		// ---------------------------------------------------------------------------------------------------- METHODS
		explicit parameters(const std::string &p = Puncturer_prefix);
		virtual ~parameters() = default;
		virtual Puncturer::parameters* clone() const;

		// parameters construction
		virtual void get_description(tools::Argument_map_info &args) const;
		virtual void store          (const tools::Argument_map_value &vals);
		virtual void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		template <typename B, typename Q>
		module::Puncturer<B,Q>* build() const;

	protected:
		parameters(const std::string &n, const std::string &p);
	};

	template <typename B, typename Q>
	static module::Puncturer<B,Q>* build(const parameters &params);
};
}
}

#endif /* FACTORY_PONCTURER_HPP_ */
