#ifndef FACTORY_CODEC_POLAR_HPP
#define FACTORY_CODEC_POLAR_HPP

#include <vector>
#include <string>
#include <map>

#include "Tools/Arguments/Argument_tools.hpp"
#include "Module/CRC/CRC.hpp"
#include "Module/Codec/Polar/Codec_polar.hpp"
#include "Factory/Tools/Code/Polar/Frozenbits_generator.hpp"
#include "Factory/Module/Codec/Codec_SISO_SIHO.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Codec_polar_name;
extern const std::string Codec_polar_prefix;
struct Codec_polar : public Codec_SISO_SIHO
{
	class parameters : public Codec_SISO_SIHO::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// depending parameters
		tools::auto_cloned_unique_ptr<Frozenbits_generator::parameters> fbg;

		// ---------------------------------------------------------------------------------------------------- METHODS
		explicit parameters(const std::string &p = Codec_polar_prefix);
		virtual ~parameters() = default;
		Codec_polar::parameters* clone() const;
		void enable_puncturer();

		virtual std::vector<std::string> get_names      () const;
		virtual std::vector<std::string> get_short_names() const;
		virtual std::vector<std::string> get_prefixes   () const;

		// parameters construction
		void get_description(tools::Argument_map_info &args) const;
		void store          (const tools::Argument_map_value &vals);
		void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		template <typename B = int, typename Q = float>
		module::Codec_polar<B,Q>* build(module::CRC<B>* crc = nullptr) const;
	};

	template <typename B = int, typename Q = float>
	static module::Codec_polar<B,Q>* build(const parameters &params, module::CRC<B>* crc = nullptr);
};
}
}

#endif /* FACTORY_CODEC_POLAR_HPP */
