#ifndef FACTORY_CODEC_REPETITION_HPP
#define FACTORY_CODEC_REPETITION_HPP

#include <string>
#include <map>

#include "Tools/Arguments/Argument_tools.hpp"
#include "Module/CRC/CRC.hpp"
#include "Module/Codec/Repetition/Codec_repetition.hpp"
#include "Factory/Module/Codec/Codec_SIHO.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Codec_repetition_name;
extern const std::string Codec_repetition_prefix;
struct Codec_repetition : public Codec_SIHO
{
	class parameters : public Codec_SIHO::parameters
	{
	public:
		explicit parameters(const std::string &p = Codec_repetition_prefix);
		virtual ~parameters() = default;
		Codec_repetition::parameters* clone() const;

		// parameters construction
		void get_description(tools::Argument_map_info &args) const;
		void store          (const tools::Argument_map_value &vals);
		void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		template <typename B = int, typename Q = float>
		module::Codec_repetition<B,Q>* build(module::CRC<B> *crc = nullptr) const;
	};

	template <typename B = int, typename Q = float>
	static module::Codec_repetition<B,Q>* build(const parameters &params, module::CRC<B> *crc = nullptr);
};
}
}

#endif /* FACTORY_CODEC_REPETITION_HPP */
