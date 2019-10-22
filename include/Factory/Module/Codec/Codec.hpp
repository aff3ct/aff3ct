/*!
 * \file
 * \brief Class factory::Codec.
 */
#ifndef FACTORY_CODEC_HPP_
#define FACTORY_CODEC_HPP_

#include <vector>
#include <string>
#include <map>
#include <cli.hpp>

#include "Tools/Factory/Header.hpp"
#include "Tools/auto_cloned_unique_ptr.hpp"
#include "Factory/Module/Encoder/Encoder.hpp"
#include "Factory/Module/Decoder/Decoder.hpp"
#include "Factory/Module/Puncturer/Puncturer.hpp"
#include "Factory/Module/Interleaver/Interleaver.hpp"
#include "Factory/Factory.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Codec_name;
extern const std::string Codec_prefix;
class Codec : public Factory
{
public:
	// ----------------------------------------------------------------------------------------------------- PARAMETERS
	// depending parameters
	tools::auto_cloned_unique_ptr<Encoder    > enc;
	tools::auto_cloned_unique_ptr<Decoder    > dec;
	tools::auto_cloned_unique_ptr<Puncturer  > pct;
	tools::auto_cloned_unique_ptr<Interleaver> itl;

	// deduced parameters
	int K           = 0;
	int N           = 0;
	int N_cw        = 0;
	int tail_length = 0;

	// -------------------------------------------------------------------------------------------------------- METHODS
	explicit Codec(const std::string &p = Codec_prefix);
	virtual ~Codec() = default;
	virtual Codec* clone() const;
	virtual void enable_puncturer();

	virtual std::vector<std::string> get_names      () const;
	virtual std::vector<std::string> get_short_names() const;
	virtual std::vector<std::string> get_prefixes   () const;

	// parameters construction
	virtual void get_description(cli::Argument_map_info &args) const;
	virtual void store          (const cli::Argument_map_value &vals);
	virtual void get_headers    (std::map<std::string,tools::header_list>& headers, const bool full = true) const;

protected:
	Codec(const std::string &n = Codec_name, const std::string &p = Codec_prefix);

	void set_enc(Encoder     *enc);
	void set_dec(Decoder     *dec);
	void set_pct(Puncturer   *pct);
	void set_itl(Interleaver *itl);

	void set_enc(tools::auto_cloned_unique_ptr<Encoder    >&& enc);
	void set_dec(tools::auto_cloned_unique_ptr<Decoder    >&& dec);
	void set_pct(tools::auto_cloned_unique_ptr<Puncturer  >&& pct);
	void set_itl(tools::auto_cloned_unique_ptr<Interleaver>&& itl);
};
}
}

#endif /* FACTORY_CODEC_HPP_ */
