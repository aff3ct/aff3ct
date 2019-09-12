#ifndef FACTORY_ENCODER_RA_HPP
#define FACTORY_ENCODER_RA_HPP

#include <vector>
#include <string>
#include <map>

#include "Tools/Arguments/Argument_tools.hpp"
#include "Tools/auto_cloned_unique_ptr.hpp"
#include "Module/Encoder/RA/Encoder_RA.hpp"
#include "Module/Interleaver/Interleaver.hpp"
#include "Factory/Module/Interleaver/Interleaver.hpp"
#include "Factory/Module/Encoder/Encoder.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Encoder_RA_name;
extern const std::string Encoder_RA_prefix;
struct Encoder_RA : public Encoder
{
	class parameters : public Encoder::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// depending parameters
		tools::auto_cloned_unique_ptr<Interleaver::parameters> itl;

		// ---------------------------------------------------------------------------------------------------- METHODS
		explicit parameters(const std::string &p = Encoder_RA_prefix);
		virtual ~parameters() = default;
		Encoder_RA::parameters* clone() const;

		virtual std::vector<std::string> get_names      () const;
		virtual std::vector<std::string> get_short_names() const;
		virtual std::vector<std::string> get_prefixes   () const;

		// parameters construction
		void get_description(tools::Argument_map_info &args) const;
		void store          (const tools::Argument_map_value &vals);
		void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		template <typename B = int>
		module::Encoder_RA<B>* build(const module::Interleaver<B> &itl) const;
	};

	template <typename B = int>
	static module::Encoder_RA<B>* build(const parameters& params, const module::Interleaver<B> &itl);
};
}
}

#endif /* FACTORY_ENCODER_RA_HPP */
