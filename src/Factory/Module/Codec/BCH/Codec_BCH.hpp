#ifndef FACTORY_CODEC_BCH_HPP
#define FACTORY_CODEC_BCH_HPP

#include <string>
#include <map>

#include "Tools/Arguments/Argument_tools.hpp"
#include "Module/CRC/CRC.hpp"
#include "Module/Codec/BCH/Codec_BCH.hpp"
#include "Factory/Module/Codec/Codec_SIHO_HIHO.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Codec_BCH_name;
extern const std::string Codec_BCH_prefix;
struct Codec_BCH : public Codec_SIHO_HIHO
{
	class parameters : public Codec_SIHO_HIHO::parameters
	{
	public:
		explicit parameters(const std::string &p = Codec_BCH_prefix);
		virtual ~parameters() = default;
		Codec_BCH::parameters* clone() const;

		// parameters construction
		void get_description(tools::Argument_map_info &args) const;
		void store          (const tools::Argument_map_value &vals);
		void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		template <typename B = int, typename Q = float>
		module::Codec_BCH<B,Q>* build(module::CRC<B> *crc = nullptr) const;
	};

	template <typename B = int, typename Q = float>
	static module::Codec_BCH<B,Q>* build(const parameters &params, module::CRC<B> *crc = nullptr);
};
}
}

#endif /* FACTORY_CODEC_BCH_HPP */
