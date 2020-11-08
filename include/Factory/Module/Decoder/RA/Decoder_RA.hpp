/*!
 * \file
 * \brief Class factory::Decoder_RA.
 */
#ifndef FACTORY_DECODER_RA_HPP
#define FACTORY_DECODER_RA_HPP

#include <vector>
#include <string>
#include <map>
#include <cli.hpp>

#include "Tools/Factory/Header.hpp"
#include "Tools/auto_cloned_unique_ptr.hpp"
#include "Module/Encoder/Encoder.hpp"
#include "Module/Decoder/Decoder_SIHO.hpp"
#include "Factory/Module/Interleaver/Interleaver.hpp"
#include "Factory/Module/Decoder/Decoder.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Decoder_RA_name;
extern const std::string Decoder_RA_prefix;
class Decoder_RA : public Decoder
{
public:
	// ----------------------------------------------------------------------------------------------------- PARAMETERS
	// optional parameters
	int n_ite = 10;

	// depending parameters
	tools::auto_cloned_unique_ptr<Interleaver> itl;

	// -------------------------------------------------------------------------------------------------------- METHODS
	explicit Decoder_RA(const std::string &p = Decoder_RA_prefix);
	virtual ~Decoder_RA() = default;
	Decoder_RA* clone() const;

	virtual std::vector<std::string> get_names      () const;
	virtual std::vector<std::string> get_short_names() const;
	virtual std::vector<std::string> get_prefixes   () const;

	// parameters construction
	void get_description(cli::Argument_map_info &args) const;
	void store          (const cli::Argument_map_value &vals);
	void get_headers    (std::map<std::string,tools::header_list>& headers, const bool full = true) const;

	// builder
	template <typename B = int, typename Q = float>
	module::Decoder_SIHO<B,Q>* build(const module::Interleaver<Q> &itl, module::Encoder<B> *encoder = nullptr) const;
};
}
}

#endif /* FACTORY_DECODER_RA_HPP */
