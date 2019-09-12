#ifndef FACTORY_DECODER_LDPC_HPP
#define FACTORY_DECODER_LDPC_HPP

#include <vector>
#include <string>
#include <memory>
#include <map>

#include "Tools/Arguments/Argument_tools.hpp"
#include "Tools/Algo/Matrix/Sparse_matrix/Sparse_matrix.hpp"
#include "Module/Encoder/Encoder.hpp"
#include "Module/Decoder/Decoder_SIHO.hpp"
#include "Module/Decoder/Decoder_SISO_SIHO.hpp"
#include "Factory/Module/Decoder/Decoder.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Decoder_LDPC_name;
extern const std::string Decoder_LDPC_prefix;
struct Decoder_LDPC : public Decoder
{
	class parameters : public Decoder::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// required parameters
		std::string H_path;

		// optional parameters
		std::string H_reorder       = "NONE";
		std::string min             = "MINL";
		std::string simd_strategy   = "";
		float       norm_factor     = 1.f;
		float       offset          = 0.f;
		float       mwbf_factor     = 1.f;
		bool        enable_syndrome = true;
		int         syndrome_depth  = 1;
		int         n_ite           = 10;

		std::vector<float> ppbf_proba;

		// ---------------------------------------------------------------------------------------------------- METHODS
		explicit parameters(const std::string &p = Decoder_LDPC_prefix);
		virtual ~parameters() = default;
		Decoder_LDPC::parameters* clone() const;

		// parameters construction
		void get_description(tools::Argument_map_info &args) const;
		void store          (const tools::Argument_map_value &vals);
		void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		template <typename B = int, typename Q = float>
		module::Decoder_SIHO<B,Q>* build(const tools::Sparse_matrix &H,
		                                 const std::vector<unsigned> &info_bits_pos,
		                                 const std::unique_ptr<module::Encoder<B>>& encoder = nullptr) const;

		template <typename B = int, typename Q = float>
		module::Decoder_SISO_SIHO<B,Q>* build_siso(const tools::Sparse_matrix &H,
		                                           const std::vector<unsigned> &info_bits_pos,
		                                           const std::unique_ptr<module::Encoder<B>>& encoder = nullptr) const;
	};

	template <typename B = int, typename Q = float>
	static module::Decoder_SIHO<B,Q>* build(const parameters& params, const tools::Sparse_matrix &H,
	                                        const std::vector<unsigned> &info_bits_pos,
	                                        const std::unique_ptr<module::Encoder<B>>& encoder = nullptr);

	template <typename B = int, typename Q = float>
	static module::Decoder_SISO_SIHO<B,Q>* build_siso(const parameters& params,
	                                                  const tools::Sparse_matrix &H,
	                                                  const std::vector<unsigned> &info_bits_pos,
	                                                  const std::unique_ptr<module::Encoder<B>>& encoder = nullptr);
};
}
}

#endif /* FACTORY_DECODER_LDPC_HPP */
