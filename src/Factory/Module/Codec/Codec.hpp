#ifndef FACTORY_CODEC_HPP_
#define FACTORY_CODEC_HPP_

#include <vector>
#include <string>
#include <map>

#include "Tools/Arguments/Argument_tools.hpp"
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
struct Codec : Factory
{
	class parameters : public Factory::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// depending parameters
		tools::auto_cloned_unique_ptr<Encoder    ::parameters> enc;
		tools::auto_cloned_unique_ptr<Decoder    ::parameters> dec;
		tools::auto_cloned_unique_ptr<Puncturer  ::parameters> pct;
		tools::auto_cloned_unique_ptr<Interleaver::parameters> itl;

		// deduced parameters
		int K           = 0;
		int N           = 0;
		int N_cw        = 0;
		int tail_length = 0;

		// ---------------------------------------------------------------------------------------------------- METHODS
		explicit parameters(const std::string &p = Codec_prefix);
		virtual ~parameters() = default;
		virtual Codec::parameters* clone() const;
		virtual void enable_puncturer();

		virtual std::vector<std::string> get_names      () const;
		virtual std::vector<std::string> get_short_names() const;
		virtual std::vector<std::string> get_prefixes   () const;

		// parameters construction
		virtual void get_description(tools::Argument_map_info &args) const;
		virtual void store          (const tools::Argument_map_value &vals);
		virtual void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

	protected:
		parameters(const std::string &n = Codec_name, const std::string &p = Codec_prefix);

		void set_enc(Encoder    ::parameters *enc);
		void set_dec(Decoder    ::parameters *dec);
		void set_pct(Puncturer  ::parameters *pct);
		void set_itl(Interleaver::parameters *itl);

		void set_enc(tools::auto_cloned_unique_ptr<Encoder    ::parameters>&& enc);
		void set_dec(tools::auto_cloned_unique_ptr<Decoder    ::parameters>&& dec);
		void set_pct(tools::auto_cloned_unique_ptr<Puncturer  ::parameters>&& pct);
		void set_itl(tools::auto_cloned_unique_ptr<Interleaver::parameters>&& itl);
	};
};
}
}

#endif /* FACTORY_CODEC_HPP_ */
