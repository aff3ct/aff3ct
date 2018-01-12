#ifndef FACTORY_CODEC_HPP_
#define FACTORY_CODEC_HPP_

#include <string>

#include "Factory/Module/Encoder/Encoder.hpp"
#include "Factory/Module/Decoder/Decoder.hpp"
#include "Factory/Module/Interleaver/Interleaver.hpp"

#include "../../Factory.hpp"

namespace aff3ct
{
namespace factory
{
struct Codec : Factory
{
	static const std::string name;
	static const std::string prefix;

	class parameters : public Factory::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// depending parameters
		Encoder::parameters     *enc = nullptr;
		Decoder::parameters     *dec = nullptr;
		Interleaver::parameters *itl = nullptr;

		// deduced parameters
		int K           = 0;
		int N           = 0;
		int N_cw        = 0;
		int tail_length = 0;

		// ---------------------------------------------------------------------------------------------------- METHODS
		parameters(const std::string &p = Codec::prefix);
		virtual ~parameters();
		virtual Codec::parameters* clone() const;
		virtual void enable_puncturer();

		virtual std::vector<std::string> get_names      () const;
		virtual std::vector<std::string> get_short_names() const;
		virtual std::vector<std::string> get_prefixes   () const;

		// parameters construction
		virtual void get_description(arg_map &req_args, arg_map &opt_args                              ) const;
		virtual void store          (const arg_val_map &vals                                           );
		virtual void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

	protected:
		parameters(const std::string &n = Codec::name, const std::string &p = Codec::prefix);

		void set_enc(Encoder    ::parameters *enc);
		void set_dec(Decoder    ::parameters *dec);
		void set_itl(Interleaver::parameters *itl);
	};
};
}
}

#endif /* FACTORY_CODEC_HPP_ */
