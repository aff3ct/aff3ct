#if !defined(AFF3CT_8BIT_PREC) && !defined(AFF3CT_16BIT_PREC)

#ifndef FACTORY_SIMULATION_EXIT_HPP_
#define FACTORY_SIMULATION_EXIT_HPP_

#include <vector>
#include <string>
#include <map>
#include <cli.hpp>

#include "Tools/auto_cloned_unique_ptr.hpp"
#include "Factory/Module/Source/Source.hpp"
#include "Factory/Module/Modem/Modem.hpp"
#include "Factory/Module/Channel/Channel.hpp"
#include "Factory/Module/Quantizer/Quantizer.hpp"
#include "Factory/Module/Monitor/EXIT/Monitor_EXIT.hpp"
#include "Factory/Tools/Codec/Codec_SISO.hpp"
#include "Factory/Tools/Display/Terminal/Terminal.hpp"
#include "Factory/Simulation/Simulation.hpp"

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
extern const std::string EXIT_name;
extern const std::string EXIT_prefix;
class EXIT : public Simulation
{
public:
	// ----------------------------------------------------------------------------------------------------- PARAMETERS
	// required parameters
	std::vector<float> sig_a_range;

	// optional parameters
	std::string snr_type = "ES";

	// module parameters
	tools::auto_cloned_unique_ptr<Source      > src;
	tools::auto_cloned_unique_ptr<Codec_SISO  > cdc;
	tools::auto_cloned_unique_ptr<Modem       > mdm;
	tools::auto_cloned_unique_ptr<Channel     > chn;
	tools::auto_cloned_unique_ptr<Quantizer   > qnt;
	tools::auto_cloned_unique_ptr<Monitor_EXIT> mnt;
	tools::auto_cloned_unique_ptr<Terminal    > ter;

	// -------------------------------------------------------------------------------------------------------- METHODS
	explicit EXIT(const std::string &p = EXIT_prefix);
	virtual ~EXIT() = default;
	virtual EXIT* clone() const;

	virtual std::vector<std::string> get_names      () const;
	virtual std::vector<std::string> get_short_names() const;
	virtual std::vector<std::string> get_prefixes   () const;

	// setters
	void set_src(Source       *src);
	void set_cdc(Codec_SISO   *cdc);
	void set_mdm(Modem        *mdm);
	void set_chn(Channel      *chn);
	void set_qnt(Quantizer    *qnt);
	void set_mnt(Monitor_EXIT *mnt);
	void set_ter(Terminal     *ter);

	// parameters construction
	void get_description(cli::Argument_map_info &args) const;
	void store          (const cli::Argument_map_value &vals);
	void get_headers    (std::map<std::string,tools::header_list>& headers, const bool full = true) const;

	// builder
	template <typename B = int, typename R = float>
	simulation::Simulation* build() const;
};
}
}

#endif /* FACTORY_SIMULATION_EXIT_HPP_ */

#endif