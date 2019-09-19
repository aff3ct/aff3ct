#ifndef FACTORY_CHANNEL_HPP
#define FACTORY_CHANNEL_HPP

#include <string>
#include <map>

#include "Tools/Arguments/Argument_tools.hpp"
#include "Tools/Math/Distribution/Distributions.hpp"
#include "Module/Channel/Channel.hpp"
#include "Factory/Factory.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Channel_name;
extern const std::string Channel_prefix;
struct Channel : public Factory
{
	class parameters : public Factory::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// required parameters
		int         N            = 0;

		// optional parameters
		std::string type         = "AWGN";
		std::string implem       = "STD";
		std::string path         = "";
		std::string block_fading = "NO";
		bool        add_users    = false;
		bool        complex      = false;
		int         n_frames     = 1;
		int         seed         = 0;
		int         gain_occur   = 1;
		float       noise        = -1.f;

		// ---------------------------------------------------------------------------------------------------- METHODS
		explicit parameters(const std::string &p = Channel_prefix);
		virtual ~parameters() = default;
		Channel::parameters* clone() const;

		// parameters construction
		void get_description(tools::Argument_map_info &args) const;
		void store          (const tools::Argument_map_value &vals);
		void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		template <typename R = float>
		module::Channel<R>* build_gaussian() const;
		template <typename R = float>
		module::Channel<R>* build_event() const;
		template <typename R = float>
		module::Channel<R>* build_userpdf(const tools::Distributions<R>& dist) const;
		template <typename R = float>
		module::Channel<R>* build() const;
		template <typename R = float>
		module::Channel<R>* build(const tools::Distributions<R>& dist) const;
	};

	template <typename R = float>
	static module::Channel<R>* build(const parameters &params);
	template <typename R = float>
	static module::Channel<R>* build(const parameters &params, const tools::Distributions<R>& dist);
};
}
}

#endif /* FACTORY_CHANNEL_HPP */
