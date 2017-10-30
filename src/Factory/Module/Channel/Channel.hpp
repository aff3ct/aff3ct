#ifndef FACTORY_CHANNEL_HPP
#define FACTORY_CHANNEL_HPP

#include <string>

#include "Module/Channel/Channel.hpp"

#include "../../Factory.hpp"

namespace aff3ct
{
namespace factory
{
struct Channel : public Factory
{
	static const std::string name;
	static const std::string prefix;

	class parameters : public Factory::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// required parameters
		int         N            = 0;

		// optional parameters
		std::string type         = "AWGN";
		std::string path         = "";
		std::string block_fading = "NO";
		bool        add_users    = false;
		bool        complex      = false;
		int         n_frames     = 1;
		int         seed         = 0;
		int         gain_occur   = 1;
		float       sigma        = -1.f;

		// ---------------------------------------------------------------------------------------------------- METHODS
		parameters(const std::string p = Channel::prefix);
		virtual ~parameters();
		Channel::parameters* clone() const;

		// parameters construction
		void get_description(arg_map &req_args, arg_map &opt_args                              ) const;
		void store          (const arg_val_map &vals                                           );
		void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		template <typename R = float>
		module::Channel<R>* build() const;
	};

	template <typename R = float>
	static module::Channel<R>* build(const parameters &params);
};
}
}

#endif /* FACTORY_CHANNEL_HPP */
