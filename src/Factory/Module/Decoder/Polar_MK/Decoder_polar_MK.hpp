#ifndef FACTORY_DECODER_POLAR_MK_HPP
#define FACTORY_DECODER_POLAR_MK_HPP

#include <string>
#include <vector>
#include <cmath>

#include "Module/CRC/CRC.hpp"
#include "Module/Decoder/Decoder_SIHO.hpp"
#include "Module/Encoder/Encoder.hpp"

#include "Tools/Code/Polar/Polar_code.hpp"

#include "../Decoder.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Decoder_polar_MK_name;
extern const std::string Decoder_polar_MK_prefix;
struct Decoder_polar_MK : public Decoder
{
	class parameters : public Decoder::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// optional parameters
		int L = 8;

		// ---------------------------------------------------------------------------------------------------- METHODS
		explicit parameters(const std::string &p = Decoder_polar_MK_prefix);
		virtual ~parameters() = default;
		Decoder_polar_MK::parameters* clone() const;

		// parameters construction
		virtual void get_description(tools::Argument_map_info &args) const;
		virtual void store          (const tools::Argument_map_value &vals);
		virtual void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		template <typename B = int, typename Q = float>
		module::Decoder_SIHO<B,Q>* build(const tools::Polar_code &code, const std::vector<bool> &frozen_bits,
		                                 module::CRC<B> *crc = nullptr,
		                                 const std::unique_ptr<module::Encoder<B>>& encoder = nullptr) const;

	protected:
		parameters(const std::string &n, const std::string &p);
	};

	template <typename B = int, typename Q = float>
	static module::Decoder_SIHO<B,Q>* build(const parameters& params,
	                                        const tools::Polar_code &code,
	                                        const std::vector<bool> &frozen_bits,
	                                        module::CRC<B> *crc = nullptr,
	                                        const std::unique_ptr<module::Encoder<B>>& encoder = nullptr);
};
}
}

#endif /* FACTORY_DECODER_POLAR_MK_HPP */
