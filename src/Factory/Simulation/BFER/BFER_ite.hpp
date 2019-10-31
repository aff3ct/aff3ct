#ifndef FACTORY_SIMULATION_BFER_ITE_HPP_
#define FACTORY_SIMULATION_BFER_ITE_HPP_

#include <vector>
#include <string>
#include <map>
#include <cli.hpp>

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
class BFER_ite : public BFER
{
public:
	// ----------------------------------------------------------------------------------------------------- PARAMETERS
	// optional parameters
	int n_ite     = 15;
	int crc_start = 2;

	// module parameters
	tools::auto_cloned_unique_ptr<Interleaver> itl;

	// -------------------------------------------------------------------------------------------------------- METHODS
	explicit BFER_ite(const std::string &p = BFER_ite_prefix);
	virtual ~BFER_ite() = default;
	BFER_ite* clone() const;

	virtual std::vector<std::string> get_names      () const;
	virtual std::vector<std::string> get_short_names() const;
	virtual std::vector<std::string> get_prefixes   () const;

	// setters
	// void set_cdc(Codec_SISO_SIHO *cdc) { this->cdc = cdc; BFER::set_cdc(cdc); }
	void set_itl(Interleaver *itl) { this->itl.reset(itl); }

	const Codec_SISO_SIHO* get_cdc() const;


	// parameters construction
	void get_description(cli::Argument_map_info &args) const;
	void store          (const cli::Argument_map_value &vals);
	void get_headers    (std::map<std::string,tools::header_list>& headers, const bool full = true) const;

	// builder
	template <typename B = int, typename R = float, typename Q = R>
	simulation::BFER_ite<B,R,Q>* build() const;
};
}
}

#endif /* FACTORY_SIMULATION_BFER_ITE_HPP_ */
