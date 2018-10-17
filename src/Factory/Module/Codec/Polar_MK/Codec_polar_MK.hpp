#ifndef FACTORY_CODEC_POLAR_MK_HPP
#define FACTORY_CODEC_POLAR_MK_HPP

#include <string>
#include <cmath>

#include "Factory/Tools/Code/Polar_MK/Frozenbits_generator_MK.hpp"

#include "Module/CRC/CRC.hpp"
#include "Module/Codec/Polar_MK/Codec_polar_MK.hpp"

#include "../Codec_SIHO.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Codec_polar_MK_name;
extern const std::string Codec_polar_MK_prefix;
struct Codec_polar_MK : public Codec_SIHO
{
	class parameters : public Codec_SIHO::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// depending parameters
		tools::auto_cloned_unique_ptr<Frozenbits_generator_MK::parameters> fbg;

		// ---------------------------------------------------------------------------------------------------- METHODS
		explicit parameters(const std::string &p = Codec_polar_MK_prefix);
		virtual ~parameters() = default;
		Codec_polar_MK::parameters* clone() const;

		virtual std::vector<std::string> get_names      () const;
		virtual std::vector<std::string> get_short_names() const;
		virtual std::vector<std::string> get_prefixes   () const;

		// parameters construction
		void get_description(tools::Argument_map_info &args) const;
		void store          (const tools::Argument_map_value &vals);
		void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		template <typename B = int, typename Q = float>
		module::Codec_polar_MK<B,Q>* build(module::CRC<B>* crc = nullptr) const;
	};

	template <typename B = int, typename Q = float>
	static module::Codec_polar_MK<B,Q>* build(const parameters &params, module::CRC<B>* crc = nullptr);
};
}
}

#endif /* FACTORY_CODEC_POLAR_MK_HPP */
