#ifndef FACTORY_SIMULATION_BFER_ITE_HPP_
#define FACTORY_SIMULATION_BFER_ITE_HPP_

#include <vector>
#include <string>
#include <map>

#include "Tools/Arguments/Argument_tools.hpp"
#include "Tools/auto_cloned_unique_ptr.hpp"
#include "Factory/Module/Interleaver/Interleaver.hpp"
#include "Factory/Module/Codec/Codec_SISO_SIHO.hpp"
#include "Factory/Simulation/BFER/BFER.hpp"

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
extern const std::string BFER_ite_name;
extern const std::string BFER_ite_prefix;
struct BFER_ite : BFER
{
	class parameters : public BFER::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// optional parameters
		int n_ite     = 15;
		int crc_start = 2;

		// module parameters
		tools::auto_cloned_unique_ptr<Interleaver::parameters> itl;

		// ---------------------------------------------------------------------------------------------------- METHODS
		explicit parameters(const std::string &p = BFER_ite_prefix);
		virtual ~parameters() = default;
		BFER_ite::parameters* clone() const;

		virtual std::vector<std::string> get_names      () const;
		virtual std::vector<std::string> get_short_names() const;
		virtual std::vector<std::string> get_prefixes   () const;

		// setters
		// void set_cdc(Codec_SISO_SIHO::parameters *cdc) { this->cdc = cdc; BFER::parameters::set_cdc(cdc); }
		void set_itl(Interleaver::parameters *itl) { this->itl.reset(itl); }

		const Codec_SISO_SIHO::parameters* get_cdc() const;


		// parameters construction
		void get_description(tools::Argument_map_info &args) const;
		void store          (const tools::Argument_map_value &vals);
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
