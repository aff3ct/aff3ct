#ifndef FACTORY_DECODER_POLAR_HPP
#define FACTORY_DECODER_POLAR_HPP

#include <string>
#include <vector>
#include <cmath>

#include "Module/CRC/CRC.hpp"
#include "Module/Decoder/Decoder_SISO_SIHO.hpp"
#include "Module/Decoder/Decoder_SIHO.hpp"

#include "../Decoder.hpp"

namespace aff3ct
{
namespace factory
{
struct Decoder_polar : public Decoder
{
	static const std::string name;
	static const std::string prefix;

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

		// ---------------------------------------------------------------------------------------------------- METHODS
		parameters(const std::string &p = Decoder_polar::prefix);
		virtual ~parameters();
		Decoder_polar::parameters* clone() const;

		// parameters construction
		virtual void get_description(arg_map &req_args, arg_map &opt_args                              ) const;
		virtual void store          (const arg_val_map &vals                                           );
		virtual void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		template <typename B = int, typename Q = float>
		module::Decoder_SISO_SIHO<B,Q>* build_siso(const std::vector<bool> &frozen_bits) const;

		template <typename B = int, typename Q = float>
		module::Decoder_SIHO<B,Q>* build(const std::vector<bool> &frozen_bits, module::CRC<B> *crc = nullptr) const;

		template <typename B = int, typename Q = float>
		module::Decoder_SIHO<B,Q>* build_gen(module::CRC<B> *crc = nullptr) const;

	private:
		template <typename B = int, typename Q = float, class API_polar>
		module::Decoder_SIHO<B,Q>* _build(const std::vector<bool> &frozen_bits, module::CRC<B> *crc = nullptr) const;

		template <typename B = int, typename Q = float, class API_polar>
		module::Decoder_SIHO<B,Q>* _build_scl_fast(const std::vector<bool> &frozen_bits,
		                                           module::CRC<B> *crc = nullptr) const;

		template <typename B = int, typename Q = float, class API_polar>
		module::Decoder_SIHO<B,Q>* _build_gen(module::CRC<B> *crc = nullptr) const;

	protected:
		parameters(const std::string &n, const std::string &p);
	};

	template <typename B = int, typename Q = float>
	static module::Decoder_SISO_SIHO<B,Q>* build_siso(const parameters& params, const std::vector<bool> &frozen_bits);

	template <typename B = int, typename Q = float>
	static module::Decoder_SIHO<B,Q>* build(const parameters& params, const std::vector<bool> &frozen_bits,
	                                        module::CRC<B> *crc = nullptr);

	template <typename B = int, typename Q = float>
	static module::Decoder_SIHO<B,Q>* build_gen(const parameters &params, module::CRC<B> *crc = nullptr);

	static const std::vector<bool>& get_frozen_bits(const std::string &implem);
};
}
}

#endif /* FACTORY_DECODER_POLAR_HPP */
