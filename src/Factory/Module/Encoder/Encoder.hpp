#ifndef FACTORY_ENCODER_HPP
#define FACTORY_ENCODER_HPP

#include <string>
#include <map>

#include "Tools/Arguments/Argument_tools.hpp"
#include "Module/Encoder/Encoder.hpp"
#include "Factory/Factory.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Encoder_name;
extern const std::string Encoder_prefix;
struct Encoder : public Factory
{
	class parameters : public Factory::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// required parameters
		int         K           = 0;
		int         N_cw        = 0;

		// optional parameters
		std::string type        = "AZCW";
		std::string path        = "";
		bool        systematic  = true;
		int         n_frames    = 1;
		int         seed        = 0;
		int         tail_length = 0;
		int         start_idx   = 0;

		// deduced parameters
		float       R           = -1.f;

		// ---------------------------------------------------------------------------------------------------- METHODS
		explicit parameters(const std::string &p = Encoder_prefix);
		virtual ~parameters() = default;
		virtual Encoder::parameters* clone() const;

		// parameters construction
		virtual void get_description(tools::Argument_map_info &args) const;
		virtual void store          (const tools::Argument_map_value &vals);
		virtual void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		template <typename B = int>
		module::Encoder<B>* build() const;

	protected:
		parameters(const std::string &n, const std::string &p);
	};

	template <typename B = int>
	static module::Encoder<B>* build(const parameters &params);
};
}
}

#endif /* FACTORY_ENCODER_HPP */
