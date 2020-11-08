/*!
 * \file
 * \brief Class factory::Decoder_turbo_product.
 */
#ifndef FACTORY_DECODER_TURBO_PRODUCT_HPP
#define FACTORY_DECODER_TURBO_PRODUCT_HPP

#include <vector>
#include <string>
#include <memory>
#include <map>
#include <cli.hpp>

#include "Tools/Factory/Header.hpp"
#include "Tools/auto_cloned_unique_ptr.hpp"
#include "Module/Encoder/Encoder.hpp"
#include "Module/Decoder/Decoder_SIHO.hpp"
#include "Module/Decoder/Decoder_SISO.hpp"
#include "Module/Decoder/Turbo_product/Chase_pyndiah/Decoder_chase_pyndiah.hpp"
#include "Module/Interleaver/Interleaver.hpp"
#include "Factory/Module/Interleaver/Interleaver.hpp"
#include "Factory/Module/Decoder/BCH/Decoder_BCH.hpp"
#include "Factory/Module/Decoder/Decoder.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Decoder_turbo_product_name;
extern const std::string Decoder_turbo_product_prefix;
class Decoder_turbo_product : public Decoder
{
public:
	// ----------------------------------------------------------------------------------------------------- PARAMETERS
	// optional parameters
	int                n_ite                      = 4;
	int                n_least_reliable_positions = 2;
	int                n_test_vectors             = 0;
	int                n_competitors              = 0;
	int                parity_extended            = false;
	std::vector<float> alpha;
	std::vector<float> beta;
	std::vector<float> cp_coef;

	// depending parameters
	tools::auto_cloned_unique_ptr<Decoder_BCH> sub;
	tools::auto_cloned_unique_ptr<Interleaver> itl;

	// -------------------------------------------------------------------------------------------------------- METHODS
	explicit Decoder_turbo_product(const std::string &p = Decoder_turbo_product_prefix);
	virtual ~Decoder_turbo_product() = default;
	Decoder_turbo_product* clone() const;

	virtual std::vector<std::string> get_names      () const;
	virtual std::vector<std::string> get_short_names() const;
	virtual std::vector<std::string> get_prefixes   () const;

	// parameters construction
	void get_description(cli::Argument_map_info &args) const;
	void store          (const cli::Argument_map_value &vals);
	void get_headers    (std::map<std::string,tools::header_list>& headers, const bool full = true) const;

	// builder
	template <typename B = int, typename Q = float>
	module::Decoder_SIHO<B,Q>* build(const module::Decoder_chase_pyndiah<B,Q> &cp_r,
	                                 const module::Decoder_chase_pyndiah<B,Q> &cp_c,
	                                 const module::Interleaver<Q>             &itl,
	                                       module::Encoder<B>                 *encoder = nullptr) const;

	template <typename B = int, typename Q = float>
	module::Decoder_SISO<B,Q>* build_siso(const module::Decoder_chase_pyndiah<B,Q> &cp_r,
	                                      const module::Decoder_chase_pyndiah<B,Q> &cp_c,
	                                      const module::Interleaver<Q>             &itl,
	                                            module::Encoder<B>                 *encoder = nullptr) const;

};
}
}

#endif /* FACTORY_DECODER_TURBO_PRODUCT_HPP */
