/*!
 * \file
 * \brief Class factory::Encoder_turbo_product.
 */
#ifndef FACTORY_ENCODER_TURBO_PRODUCT_HPP
#define FACTORY_ENCODER_TURBO_PRODUCT_HPP

#include <vector>
#include <string>
#include <map>
#include <cli.hpp>

#include "Tools/Factory/Header.hpp"
#include "Tools/auto_cloned_unique_ptr.hpp"
#include "Module/Encoder/BCH/Encoder_BCH.hpp"
#include "Module/Interleaver/Interleaver.hpp"
#include "Module/Encoder/Turbo_product/Encoder_turbo_product.hpp"
#include "Factory/Module/Interleaver/Interleaver.hpp"
#include "Factory/Module/Encoder/BCH/Encoder_BCH.hpp"
#include "Factory/Module/Encoder/Encoder.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Encoder_turbo_product_name;
extern const std::string Encoder_turbo_product_prefix;
class Encoder_turbo_product : public Encoder
{
public:
	// ----------------------------------------------------------------------------------------------------- PARAMETERS
	// optional parameters
	int parity_extended = false;

	// depending parameters
	tools::auto_cloned_unique_ptr<Encoder_BCH> sub;
	tools::auto_cloned_unique_ptr<Interleaver> itl;

	// -------------------------------------------------------------------------------------------------------- METHODS
	explicit Encoder_turbo_product(const std::string &p = Encoder_turbo_product_prefix);
	virtual ~Encoder_turbo_product() = default;
	Encoder_turbo_product* clone() const;

	virtual std::vector<std::string> get_names      () const;
	virtual std::vector<std::string> get_short_names() const;
	virtual std::vector<std::string> get_prefixes   () const;

	// parameters construction
	void get_description(cli::Argument_map_info &args) const;
	void store          (const cli::Argument_map_value &vals);
	void get_headers    (std::map<std::string,tools::header_list>& headers, const bool full = true) const;

	// builder
	template <typename B = int>
	module::Encoder_turbo_product<B>* build(const module::Encoder_BCH<B> &enc_r,
	                                        const module::Encoder_BCH<B> &enc_c,
	                                        const module::Interleaver<B> &itl) const;
};
}
}

#endif /* FACTORY_ENCODER_TURBO_PRODUCT_HPP */
