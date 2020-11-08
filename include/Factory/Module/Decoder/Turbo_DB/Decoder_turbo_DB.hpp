/*!
 * \file
 * \brief Class factory::Decoder_turbo_DB.
 */
#ifndef FACTORY_DECODER_TURBO_DB_HPP
#define FACTORY_DECODER_TURBO_DB_HPP

#include <vector>
#include <string>
#include <map>
#include <cli.hpp>

#include "Tools/Factory/Header.hpp"
#include "Tools/auto_cloned_unique_ptr.hpp"
#include "Module/Encoder/Encoder.hpp"
#include "Module/Decoder/Decoder_SIHO.hpp"
#include "Module/Decoder/RSC_DB/BCJR/Decoder_RSC_DB_BCJR.hpp"
#include "Module/Decoder/Turbo_DB/Decoder_turbo_DB.hpp"
#include "Module/Interleaver/Interleaver.hpp"
#include "Factory/Module/Interleaver/Interleaver.hpp"
#include "Factory/Tools/Code/Turbo/Flip_and_check_DB.hpp"
#include "Factory/Tools/Code/Turbo/Scaling_factor.hpp"
#include "Factory/Module/Decoder/RSC_DB/Decoder_RSC_DB.hpp"
#include "Factory/Module/Decoder/Decoder.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Decoder_turbo_DB_name;
extern const std::string Decoder_turbo_DB_prefix;
class Decoder_turbo_DB : public Decoder
{
public:
	// ----------------------------------------------------------------------------------------------------- PARAMETERS
	// optional parameters
	int n_ite         = 6;
	int crc_start_ite = 2;

	// depending parameters
	tools::auto_cloned_unique_ptr<Decoder_RSC_DB   > sub;
	tools::auto_cloned_unique_ptr<Interleaver      > itl;
	tools::auto_cloned_unique_ptr<Scaling_factor   > sf;
	tools::auto_cloned_unique_ptr<Flip_and_check_DB> fnc;

	// -------------------------------------------------------------------------------------------------------- METHODS
	explicit Decoder_turbo_DB(const std::string &p = Decoder_turbo_DB_prefix);
	virtual ~Decoder_turbo_DB() = default;
	Decoder_turbo_DB* clone() const;

	virtual std::vector<std::string> get_names      () const;
	virtual std::vector<std::string> get_short_names() const;
	virtual std::vector<std::string> get_prefixes   () const;

	// parameters construction
	void get_description(cli::Argument_map_info &args) const;
	void store          (const cli::Argument_map_value &vals);
	void get_headers    (std::map<std::string,tools::header_list>& headers, const bool full = true) const;

	// builder
	template <typename B = int, typename Q = float>
	module::Decoder_turbo_DB<B,Q>* build(const module::Decoder_RSC_DB_BCJR<B,Q> &siso_n,
	                                     const module::Decoder_RSC_DB_BCJR<B,Q> &siso_i,
	                                     const module::Interleaver<Q>           &itl,
	                                           module::Encoder<B>               *encoder = nullptr) const;

	template <typename B = int, typename Q = float>
	module::Decoder_SIHO<B,Q>* build(module::Encoder<B> *encoder = nullptr) const;
};
}
}

#endif /* FACTORY_DECODER_TURBO_DB_HPP */
