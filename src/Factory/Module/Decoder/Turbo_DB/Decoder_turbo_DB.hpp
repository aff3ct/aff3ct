#ifndef FACTORY_DECODER_TURBO_DB_HPP
#define FACTORY_DECODER_TURBO_DB_HPP

#include <vector>
#include <string>
#include <memory>
#include <map>

#include "Tools/Arguments/Argument_tools.hpp"
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
struct Decoder_turbo_DB : public Decoder
{
	class parameters : public Decoder::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// optional parameters
		int n_ite         = 6;
		int crc_start_ite = 2;

		// depending parameters
		tools::auto_cloned_unique_ptr<Decoder_RSC_DB   ::parameters> sub;
		tools::auto_cloned_unique_ptr<Interleaver      ::parameters> itl;
		tools::auto_cloned_unique_ptr<Scaling_factor   ::parameters> sf;
		tools::auto_cloned_unique_ptr<Flip_and_check_DB::parameters> fnc;

		// ---------------------------------------------------------------------------------------------------- METHODS
		explicit parameters(const std::string &p = Decoder_turbo_DB_prefix);
		virtual ~parameters() = default;
		Decoder_turbo_DB::parameters* clone() const;

		virtual std::vector<std::string> get_names      () const;
		virtual std::vector<std::string> get_short_names() const;
		virtual std::vector<std::string> get_prefixes   () const;

		// parameters construction
		void get_description(tools::Argument_map_info &args) const;
		void store          (const tools::Argument_map_value &vals);
		void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		template <typename B = int, typename Q = float>
		module::Decoder_turbo_DB<B,Q>* build(const module::Interleaver<Q>              &itl,
		                                           module::Decoder_RSC_DB_BCJR<B,Q>    &siso_n,
		                                           module::Decoder_RSC_DB_BCJR<B,Q>    &siso_i,
		                                     const std::unique_ptr<module::Encoder<B>> &encoder = nullptr) const;

		template <typename B = int, typename Q = float>
		module::Decoder_SIHO<B,Q>* build(const std::unique_ptr<module::Encoder<B>> &encoder = nullptr) const;
	};

	template <typename B = int, typename Q = float>
	static module::Decoder_turbo_DB<B,Q>* build(const parameters                          &params,
	                                            const module::Interleaver<Q>              &itl,
	                                                  module::Decoder_RSC_DB_BCJR<B,Q>    &siso_n,
	                                                  module::Decoder_RSC_DB_BCJR<B,Q>    &siso_i,
	                                            const std::unique_ptr<module::Encoder<B>> &encoder = nullptr);

	template <typename B = int, typename Q = float>
	static module::Decoder_SIHO<B,Q>* build(const parameters                          &params,
	                                        const std::unique_ptr<module::Encoder<B>> &encoder = nullptr);
};
}
}

#endif /* FACTORY_DECODER_TURBO_DB_HPP */
