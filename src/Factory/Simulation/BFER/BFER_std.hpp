#ifndef FACTORY_SIMULATION_BFER_STD_HPP_
#define FACTORY_SIMULATION_BFER_STD_HPP_

#include <string>
#include <map>

#include "Tools/Arguments/Argument_tools.hpp"
#include "Factory/Module/Codec/Codec_SIHO.hpp"
#include "Factory/Simulation/BFER/BFER.hpp"

namespace aff3ct
{
namespace simulation
{
template <typename B, typename R, typename Q>
class BFER_std;
}
}

namespace aff3ct
{
namespace factory
{
extern const std::string BFER_std_name;
extern const std::string BFER_std_prefix;
struct BFER_std : BFER
{
	class parameters : public BFER::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// module parameters
		// Codec_SIHO::parameters *cdc = nullptr;

		// ---------------------------------------------------------------------------------------------------- METHODS
		explicit parameters(const std::string &p = BFER_std_prefix);
		virtual ~parameters() = default;
		BFER_std::parameters* clone() const;

		// setters
		// void set_cdc(Codec_SIHO::parameters *cdc) { this->cdc = cdc; BFER::parameters::set_cdc(cdc); }
		const Codec_SIHO::parameters* get_cdc() const;

		// parameters construction
		void get_description(tools::Argument_map_info &args) const;
		void store          (const tools::Argument_map_value &vals);
		void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		template <typename B = int, typename R = float, typename Q = R>
		simulation::BFER_std<B,R,Q>* build() const;
	};

	template <typename B = int, typename R = float, typename Q = R>
	static simulation::BFER_std<B,R,Q>* build(const parameters &params);
};
}
}

#endif /* FACTORY_SIMULATION_BFER_STD_HPP_ */
