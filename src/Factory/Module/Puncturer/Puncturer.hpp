#ifndef FACTORY_PONCTURER_HPP_
#define FACTORY_PONCTURER_HPP_

#include <string>

#include "Module/Puncturer/Puncturer.hpp"

#include "../../Factory.hpp"

namespace aff3ct
{
namespace factory
{
struct Puncturer : Factory
{
	static const std::string name;
	static const std::string prefix;

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
		parameters(const std::string &p = Puncturer::prefix);
		virtual ~parameters();
		virtual Puncturer::parameters* clone() const;

		// parameters construction
		virtual void get_description(arg_map &req_args, arg_map &opt_args                              ) const;
		virtual void store          (const arg_val_map &vals                                           );
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
