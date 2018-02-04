#ifndef FACTORY_ENCODER_TURBO_HPP
#define FACTORY_ENCODER_TURBO_HPP

#include <string>

#include "Module/Interleaver/Interleaver.hpp"
#include "Module/Encoder/Turbo/Encoder_turbo.hpp"

#include "Factory/Module/Interleaver/Interleaver.hpp"
#include "Factory/Module/Encoder/RSC/Encoder_RSC.hpp"

#include "../Encoder.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Encoder_turbo_name;
extern const std::string Encoder_turbo_prefix;
struct Encoder_turbo : public Encoder
{
	template <class E1 = Encoder_RSC, class E2 = E1>
	class parameters : public Encoder::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// optional
		std::string json_path = "";

		// depending parameters
		Interleaver::parameters *itl;
		typename E1::parameters *sub1;
		typename E2::parameters *sub2;

		// ---------------------------------------------------------------------------------------------------- METHODS
		explicit parameters(const std::string &p = Encoder_turbo_prefix);
		virtual ~parameters();
		Encoder_turbo::parameters<E1,E2>* clone() const;

		virtual std::vector<std::string> get_names      () const;
		virtual std::vector<std::string> get_short_names() const;
		virtual std::vector<std::string> get_prefixes   () const;

		// parameters construction
		void get_description(tools::Argument_map_info &req_args, tools::Argument_map_info &opt_args) const;
		void store          (const tools::Argument_map_value &vals                                 );
		void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		template <typename B = int>
		module::Encoder_turbo<B>* build(const module::Interleaver<B> &itl,
		                                      module::Encoder    <B> *enc_n,
		                                      module::Encoder    <B> *enc_i = nullptr) const;
	};

	template <typename B = int, class E1 = Encoder_RSC, class E2 = E1>
	static module::Encoder_turbo<B>* build(const parameters<E1,E2>      &params,
	                                       const module::Interleaver<B> &itl,
	                                             module::Encoder    <B> *enc_n,
	                                             module::Encoder    <B> *enc_i = nullptr);
};
}
}

#include "Encoder_turbo.hxx"

#endif /* FACTORY_ENCODER_TURBO_HPP */
