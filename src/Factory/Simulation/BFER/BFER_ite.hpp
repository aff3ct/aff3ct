#ifndef FACTORY_SIMULATION_BFER_ITE_HPP_
#define FACTORY_SIMULATION_BFER_ITE_HPP_

#include <string>

#include "Factory/Module/Interleaver/Interleaver.hpp"
#include "Factory/Module/Codec/Codec_SISO_SIHO.hpp"

#include "BFER.hpp"

namespace aff3ct
{
namespace simulation
{
template <typename B, typename R, typename Q>
class BFER_ite;
}
}

namespace aff3ct
{
namespace factory
{
struct BFER_ite : BFER
{
	static const std::string name;
	static const std::string prefix;

	class parameters : public BFER::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// optional parameters
		int n_ite     = 15;
		int crc_start = 2;

		// module parameters
		Interleaver    ::parameters *itl = nullptr;
		Codec_SISO_SIHO::parameters *cdc = nullptr;

		// ---------------------------------------------------------------------------------------------------- METHODS
		parameters(const std::string p = BFER_ite::prefix);
		virtual ~parameters();
		BFER_ite::parameters* clone() const;

		virtual std::vector<std::string> get_names      () const;
		virtual std::vector<std::string> get_short_names() const;
		virtual std::vector<std::string> get_prefixes   () const;

		// setters
		void set_cdc(Codec_SISO_SIHO::parameters *cdc) { this->cdc = cdc; BFER::parameters::set_cdc(cdc); }
		void set_itl(Interleaver    ::parameters *itl) { this->itl = itl;                                 }

		// parameters construction
		void get_description(tools::Argument_map_info &req_args, tools::Argument_map_info &opt_args) const;
		void store          (const tools::Argument_map_value &vals                                 );
		void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		template <typename B = int, typename R = float, typename Q = R>
		simulation::BFER_ite<B,R,Q>* build() const;
	};

	template <typename B = int, typename R = float, typename Q = R>
	static simulation::BFER_ite<B,R,Q>* build(const parameters &params);
};
}
}

#endif /* FACTORY_SIMULATION_BFER_ITE_HPP_ */
