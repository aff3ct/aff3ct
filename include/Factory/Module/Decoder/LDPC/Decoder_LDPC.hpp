/*!
 * \file
 * \brief Class factory::Decoder_LDPC.
 */
#ifndef FACTORY_DECODER_LDPC_HPP
#define FACTORY_DECODER_LDPC_HPP

#include <vector>
#include <string>
#include <map>
#include <cli.hpp>

#include "Tools/Factory/Header.hpp"
#include "Tools/Algo/Matrix/Sparse_matrix/Sparse_matrix.hpp"
#include "Module/Encoder/Encoder.hpp"
#include "Module/Decoder/Decoder_SIHO.hpp"
#include "Module/Decoder/Decoder_SISO.hpp"
#include "Factory/Module/Decoder/Decoder.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Decoder_LDPC_name;
extern const std::string Decoder_LDPC_prefix;
class Decoder_LDPC : public Decoder
{
public:
	// ----------------------------------------------------------------------------------------------------- PARAMETERS
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

	// -------------------------------------------------------------------------------------------------------- METHODS
	explicit Decoder_LDPC(const std::string &p = Decoder_LDPC_prefix);
	virtual ~Decoder_LDPC() = default;
	Decoder_LDPC* clone() const;

	// parameters construction
	void get_description(cli::Argument_map_info &args) const;
	void store          (const cli::Argument_map_value &vals);
	void get_headers    (std::map<std::string,tools::header_list>& headers, const bool full = true) const;

	// builder
	template <typename B = int, typename Q = float>
	module::Decoder_SIHO<B,Q>* build(const tools::Sparse_matrix &H,
	                                 const std::vector<unsigned> &info_bits_pos,
	                                 module::Encoder<B> *encoder = nullptr) const;

	template <typename B = int, typename Q = float>
	module::Decoder_SISO<B,Q>* build_siso(const tools::Sparse_matrix &H,
	                                      const std::vector<unsigned> &info_bits_pos,
	                                      module::Encoder<B> *encoder = nullptr) const;
};
}
}

#endif /* FACTORY_DECODER_LDPC_HPP */
