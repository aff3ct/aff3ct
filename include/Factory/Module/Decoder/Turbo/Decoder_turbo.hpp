/*!
 * \file
 * \brief Class factory::Decoder_turbo.
 */
#ifndef FACTORY_DECODER_TURBO_HPP
#define FACTORY_DECODER_TURBO_HPP

#include <vector>
#include <string>
#include <map>
#include <cli.hpp>

#include "Tools/Factory/Header.hpp"
#include "Tools/auto_cloned_unique_ptr.hpp"
#include "Module/Encoder/Encoder.hpp"
#include "Module/Decoder/Decoder_SISO.hpp"
#include "Module/Decoder/Decoder_SIHO.hpp"
#include "Module/Decoder/Turbo/Decoder_turbo.hpp"
#include "Module/Interleaver/Interleaver.hpp"
#include "Factory/Module/Interleaver/Interleaver.hpp"
#include "Factory/Tools/Code/Turbo/Flip_and_check.hpp"
#include "Factory/Tools/Code/Turbo/Scaling_factor.hpp"
#include "Factory/Module/Decoder/RSC/Decoder_RSC.hpp"
#include "Factory/Module/Decoder/Decoder.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Decoder_turbo_name;
extern const std::string Decoder_turbo_prefix;
template <class D1 = Decoder_RSC, class D2 = D1>
class Decoder_turbo : public Decoder
{
public:
	// ----------------------------------------------------------------------------------------------------- PARAMETERS
	// optional parameters
	bool self_corrected = false;
	bool enable_json    = false;
	int  n_ite          = 6;
	int  crc_start_ite  = 2;

	// depending parameters
	tools::auto_cloned_unique_ptr<D1            > sub1;
	tools::auto_cloned_unique_ptr<D2            > sub2;
	tools::auto_cloned_unique_ptr<Interleaver   > itl;
	tools::auto_cloned_unique_ptr<Scaling_factor> sf;
	tools::auto_cloned_unique_ptr<Flip_and_check> fnc;

	// -------------------------------------------------------------------------------------------------------- METHODS
	explicit Decoder_turbo(const std::string &p = Decoder_turbo_prefix);
	virtual ~Decoder_turbo() = default;
	Decoder_turbo<D1,D2>* clone() const;

	virtual std::vector<std::string> get_names      () const;
	virtual std::vector<std::string> get_short_names() const;
	virtual std::vector<std::string> get_prefixes   () const;

	// parameters construction
	void get_description(cli::Argument_map_info &args) const;
	void store          (const cli::Argument_map_value &vals);
	void get_headers    (std::map<std::string,tools::header_list>& headers, const bool full = true) const;

	// builder
	template <typename B = int, typename Q = float>
	module::Decoder_turbo<B,Q>* build(const module::Decoder_SISO<B,Q> &siso_n,
	                                  const module::Decoder_SISO<B,Q> &siso_i,
	                                  const module::Interleaver<Q>    &itl,
	                                        module::Encoder<B>        *encoder = nullptr) const;

	template <typename B = int, typename Q = float>
	module::Decoder_SIHO<B,Q>* build(module::Encoder<B> *encoder = nullptr) const;
};

#ifndef DOXYGEN_SHOULD_SKIP_THIS
struct Decoder_turbo_common
{
	static void add_args_and_options(cli::Argument_map_info &args, const std::string &p, const std::string &class_name);
};
#endif
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Factory/Module/Decoder/Turbo/Decoder_turbo.hxx"
#endif

#endif /* FACTORY_DECODER_TURBO_HPP */
