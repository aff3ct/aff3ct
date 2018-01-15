#ifndef FACTORY_DECODER_RSC_DB_HPP
#define FACTORY_DECODER_RSC_DB_HPP

#include <string>
#include <vector>
#include <iostream>
#include <sstream>

#include "Tools/Math/max.h"

#include "Module/Decoder/RSC_DB/BCJR/Decoder_RSC_DB_BCJR.hpp"

#include "../Decoder.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Decoder_RSC_DB_name;
extern const std::string Decoder_RSC_DB_prefix;
struct Decoder_RSC_DB : public Decoder
{
	class parameters : public Decoder::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// optional parameters
		std::string max      = "MAX";
		bool        buffered = true;

		// ---------------------------------------------------------------------------------------------------- METHODS
		explicit parameters(const std::string &p = Decoder_RSC_DB_prefix);
		virtual ~parameters();
		Decoder_RSC_DB::parameters* clone() const;

		// parameters construction
		void get_description(tools::Argument_map_info &req_args, tools::Argument_map_info &opt_args) const;
		void store          (const tools::Argument_map_value &vals                                 );
		void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		template <typename B = int, typename Q = float>
		module::Decoder_RSC_DB_BCJR<B,Q>* build(const std::vector<std::vector<int>> &trellis) const;

	private:
		template <typename B = int, typename Q = float, tools::proto_max<Q> MAX>
		module::Decoder_RSC_DB_BCJR<B,Q>* _build(const std::vector<std::vector<int>> &trellis) const;
	};

	template <typename B = int, typename Q = float>
	static module::Decoder_RSC_DB_BCJR<B,Q>* build(const parameters &params, const std::vector<std::vector<int>> &trellis);
};
}
}

#endif /* FACTORY_DECODER_RSC_DB_HPP */
