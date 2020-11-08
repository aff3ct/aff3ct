/*!
 * \file
 * \brief Class factory::Encoder_turbo.
 */
#ifndef FACTORY_ENCODER_TURBO_HPP
#define FACTORY_ENCODER_TURBO_HPP

#include <vector>
#include <string>
#include <memory>
#include <map>
#include <cli.hpp>

#include "Tools/Factory/Header.hpp"
#include "Tools/auto_cloned_unique_ptr.hpp"
#include "Module/Interleaver/Interleaver.hpp"
#include "Module/Encoder/Turbo/Encoder_turbo.hpp"
#include "Factory/Module/Interleaver/Interleaver.hpp"
#include "Factory/Module/Encoder/RSC/Encoder_RSC.hpp"
#include "Factory/Module/Encoder/Encoder.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Encoder_turbo_name;
extern const std::string Encoder_turbo_prefix;
template <class E1 = Encoder_RSC, class E2 = E1>
class Encoder_turbo : public Encoder
{
public:
	// ----------------------------------------------------------------------------------------------------- PARAMETERS
	// optional
	std::string json_path = "";

	// depending parameters
	tools::auto_cloned_unique_ptr<Interleaver> itl;
	tools::auto_cloned_unique_ptr<E1> sub1;
	tools::auto_cloned_unique_ptr<E2> sub2;

	// -------------------------------------------------------------------------------------------------------- METHODS
	explicit Encoder_turbo(const std::string &p = Encoder_turbo_prefix);
	virtual ~Encoder_turbo() = default;
	Encoder_turbo<E1,E2>* clone() const;

	virtual std::vector<std::string> get_names      () const;
	virtual std::vector<std::string> get_short_names() const;
	virtual std::vector<std::string> get_prefixes   () const;

	// parameters construction
	void get_description(cli::Argument_map_info &args) const;
	void store          (const cli::Argument_map_value &vals);
	void get_headers    (std::map<std::string,tools::header_list>& headers, const bool full = true) const;

	// builder
	template <typename B = int>
	module::Encoder_turbo<B>* build(const module::Encoder<B>     &enc_n,
	                                const module::Encoder<B>     &enc_i,
	                                const module::Interleaver<B> &itl) const;
};

#ifndef DOXYGEN_SHOULD_SKIP_THIS
struct Encoder_turbo_common
{
	static void add_args_and_options(cli::Argument_map_info &args, const std::string &p, const std::string &class_name);
};
#endif
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Factory/Module/Encoder/Turbo/Encoder_turbo.hxx"
#endif

#endif /* FACTORY_ENCODER_TURBO_HPP */
