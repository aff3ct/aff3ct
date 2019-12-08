#ifndef FACTORY_SIMULATION_BFER_STD_HPP_
#define FACTORY_SIMULATION_BFER_STD_HPP_

#include <string>
#include <map>
#include <cli.hpp>

#include "Factory/Tools/Codec/Codec_SIHO.hpp"
#include "Factory/Simulation/BFER/BFER.hpp"

namespace aff3ct
{
namespace simulation
{
class Simulation;
}
}

namespace aff3ct
{
namespace factory
{
extern const std::string BFER_std_name;
extern const std::string BFER_std_prefix;
class BFER_std : public BFER
{
public:
	// ----------------------------------------------------------------------------------------------------- PARAMETERS
	// module parameters
	// Codec_SIHO *cdc = nullptr;

	// -------------------------------------------------------------------------------------------------------- METHODS
	explicit BFER_std(const std::string &p = BFER_std_prefix);
	virtual ~BFER_std() = default;
	BFER_std* clone() const;

	// setters
	// void set_cdc(Codec_SIHO *cdc) { this->cdc = cdc; BFER::set_cdc(cdc); }
	const Codec_SIHO* get_cdc() const;

	// parameters construction
	void get_description(cli::Argument_map_info &args) const;
	void store          (const cli::Argument_map_value &vals);
	void get_headers    (std::map<std::string,tools::header_list>& headers, const bool full = true) const;

	// builder
	template <typename B = int, typename R = float, typename Q = R>
	simulation::Simulation* build() const;
};
}
}

#endif /* FACTORY_SIMULATION_BFER_STD_HPP_ */
