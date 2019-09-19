#ifndef FACTORY_DECODER_POLAR_HPP
#define FACTORY_DECODER_POLAR_HPP

#include <vector>
#include <string>
#include <memory>
#include <map>

#include "Tools/Arguments/Argument_tools.hpp"
#include "Module/CRC/CRC.hpp"
#include "Module/Encoder/Encoder.hpp"
#include "Module/Decoder/Decoder_SIHO.hpp"
#include "Module/Decoder/Decoder_SISO_SIHO.hpp"
#include "Factory/Module/Decoder/Decoder.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Decoder_polar_name;
extern const std::string Decoder_polar_prefix;
struct Decoder_polar : public Decoder
{
	class parameters : public Decoder::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// optional parameters
		std::string simd_strategy = "";
		std::string polar_nodes   = "{R0,R0L,R1,REP,REPL,SPC}";
		bool        full_adaptive = true;
		int         n_ite         = 1;
		int         L             = 8;
		int         T             = 8;

		// ---------------------------------------------------------------------------------------------------- METHODS
		explicit parameters(const std::string &p = Decoder_polar_prefix);
		virtual ~parameters() = default;
		Decoder_polar::parameters* clone() const;

		// parameters construction
		virtual void get_description(tools::Argument_map_info &args) const;
		virtual void store          (const tools::Argument_map_value &vals);
		virtual void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		template <typename B = int, typename Q = float>
		module::Decoder_SISO_SIHO<B,Q>* build_siso(const std::vector<bool> &frozen_bits,
		                                           const std::unique_ptr<module::Encoder<B>>& encoder = nullptr) const;

		template <typename B = int, typename Q = float>
		module::Decoder_SIHO<B,Q>* build(const std::vector<bool> &frozen_bits, module::CRC<B> *crc = nullptr,
		                                 const std::unique_ptr<module::Encoder<B>>& encoder = nullptr) const;

		template <typename B = int, typename Q = float>
		module::Decoder_SIHO<B,Q>* build_gen(module::CRC<B> *crc = nullptr,
		                                     const std::unique_ptr<module::Encoder<B>>& encoder = nullptr) const;

	private:
		template <typename B = int, typename Q = float, class API_polar>
		module::Decoder_SIHO<B,Q>* _build(const std::vector<bool> &frozen_bits, module::CRC<B> *crc = nullptr,
		                                  const std::unique_ptr<module::Encoder<B>>& encoder = nullptr) const;

		template <typename B = int, typename Q = float, class API_polar>
		module::Decoder_SIHO<B,Q>* _build_scl_fast(const std::vector<bool> &frozen_bits,
		                                           module::CRC<B> *crc = nullptr,
		                                           const std::unique_ptr<module::Encoder<B>>& encoder = nullptr) const;

		template <typename B = int, typename Q = float, class API_polar>
		module::Decoder_SIHO<B,Q>* _build_gen(module::CRC<B> *crc = nullptr,
		                                      const std::unique_ptr<module::Encoder<B>>& encoder = nullptr) const;

	protected:
		parameters(const std::string &n, const std::string &p);
	};

	template <typename B = int, typename Q = float>
	static module::Decoder_SISO_SIHO<B,Q>* build_siso(const parameters& params, const std::vector<bool> &frozen_bits,
	                                                  const std::unique_ptr<module::Encoder<B>>& encoder = nullptr);

	template <typename B = int, typename Q = float>
	static module::Decoder_SIHO<B,Q>* build(const parameters& params, const std::vector<bool> &frozen_bits,
	                                        module::CRC<B> *crc = nullptr, const std::unique_ptr<module::Encoder<B>>& encoder = nullptr);

	template <typename B = int, typename Q = float>
	static module::Decoder_SIHO<B,Q>* build_gen(const parameters &params, module::CRC<B> *crc = nullptr,
	                                            const std::unique_ptr<module::Encoder<B>>& encoder = nullptr);

	static const std::vector<bool>& get_frozen_bits(const std::string &implem);
};
}
}

#endif /* FACTORY_DECODER_POLAR_HPP */
