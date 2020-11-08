/*!
 * \file
 * \brief Class factory::Encoder_turbo_DB.
 */
#ifndef FACTORY_ENCODER_TURBO_DB_HPP
#define FACTORY_ENCODER_TURBO_DB_HPP

#include <vector>
#include <string>
#include <map>
#include <cli.hpp>

#include "Tools/Factory/Header.hpp"
#include "Tools/auto_cloned_unique_ptr.hpp"
#include "Module/Interleaver/Interleaver.hpp"
#include "Module/Encoder/RSC_DB/Encoder_RSC_DB.hpp"
#include "Module/Encoder/Turbo_DB/Encoder_turbo_DB.hpp"
#include "Factory/Module/Interleaver/Interleaver.hpp"
#include "Factory/Module/Encoder/RSC_DB/Encoder_RSC_DB.hpp"
#include "Factory/Module/Encoder/Encoder.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Encoder_turbo_DB_name;
extern const std::string Encoder_turbo_DB_prefix;
class Encoder_turbo_DB : public Encoder
{
public:
	// ----------------------------------------------------------------------------------------------------- PARAMETERS
	// depending parameters
	tools::auto_cloned_unique_ptr<Interleaver   > itl;
	tools::auto_cloned_unique_ptr<Encoder_RSC_DB> sub;

	// -------------------------------------------------------------------------------------------------------- METHODS
	explicit Encoder_turbo_DB(const std::string &p = Encoder_turbo_DB_prefix);
	virtual ~Encoder_turbo_DB() = default;
	Encoder_turbo_DB* clone() const;

	virtual std::vector<std::string> get_names      () const;
	virtual std::vector<std::string> get_short_names() const;
	virtual std::vector<std::string> get_prefixes   () const;

	// parameters construction
	void get_description(cli::Argument_map_info &args) const;
	void store          (const cli::Argument_map_value &vals);
	void get_headers    (std::map<std::string,tools::header_list>& headers, const bool full = true) const;

	// builder
	template <typename B = int>
	module::Encoder_turbo_DB<B>* build(const module::Encoder_RSC_DB<B> &sub_enc,
	                                   const module::Interleaver<B> &itl) const;
};
}
}

#endif /* FACTORY_ENCODER_TURBO_DB_HPP */
