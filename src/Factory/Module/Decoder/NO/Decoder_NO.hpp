#ifndef FACTORY_DECODER_NO_HPP
#define FACTORY_DECODER_NO_HPP

#include <string>
#include <memory>
#include <map>

#include "Tools/Arguments/Argument_tools.hpp"
#include "Module/Encoder/Encoder.hpp"
#include "Module/Decoder/Decoder_SIHO.hpp"
#include "Module/Decoder/Decoder_SISO_SIHO.hpp"
#include "Factory/Module/Decoder/Decoder.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Decoder_NO_name;
extern const std::string Decoder_NO_prefix;
struct Decoder_NO : public Decoder
{
	class parameters : public Decoder::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// empty

		// ---------------------------------------------------------------------------------------------------- METHODS
		explicit parameters(const std::string &p = Decoder_NO_prefix);
		virtual ~parameters() = default;
		Decoder_NO::parameters* clone() const;

		// parameters construction
		void get_description(tools::Argument_map_info &args) const;
		void store          (const tools::Argument_map_value &vals);
		void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		template <typename B = int, typename Q = float>
		module::Decoder_SISO_SIHO<B,Q>* build_siso(const std::unique_ptr<module::Encoder<B>>& encoder = nullptr) const;

		template <typename B = int, typename Q = float>
		module::Decoder_SIHO<B,Q>* build(const std::unique_ptr<module::Encoder<B>>& encoder = nullptr) const;
	};

	template <typename B = int, typename Q = float>
	static module::Decoder_SISO_SIHO<B,Q>* build_siso(const parameters &params, const std::unique_ptr<module::Encoder<B>>& encoder = nullptr);

	template <typename B = int, typename Q = float>
	static module::Decoder_SIHO<B,Q>* build(const parameters &params, const std::unique_ptr<module::Encoder<B>>& encoder = nullptr);
};
}
}

#endif /* FACTORY_DECODER_NO_HPP */
