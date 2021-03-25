#ifndef FACTORY_SIMULATION_BFER_HPP_
#define FACTORY_SIMULATION_BFER_HPP_

#include <chrono>
#include <vector>
#include <string>
#include <map>
#include <cli.hpp>

#include "Tools/auto_cloned_unique_ptr.hpp"
#include "Factory/Module/Channel/Channel.hpp"
#include "Factory/Module/CRC/CRC.hpp"
#include "Factory/Tools/Codec/Codec.hpp"
#include "Factory/Module/Modem/Modem.hpp"
#include "Factory/Module/Quantizer/Quantizer.hpp"
#include "Factory/Module/Source/Source.hpp"
#include "Factory/Module/Monitor/MI/Monitor_MI.hpp"
#include "Factory/Module/Monitor/BFER/Monitor_BFER.hpp"
#include "Factory/Tools/Display/Terminal/Terminal.hpp"
#include "Factory/Simulation/Simulation.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string BFER_name;
extern const std::string BFER_prefix;
class BFER : public Simulation
{
public:
	// ----------------------------------------------------------------------------------------------------- PARAMETERS
	// optional parameters
	std::string err_track_path      = "error_tracker";
	std::string sequence_path       = "";
	int         err_track_threshold = 0;
	bool        err_track_revert    = false;
	bool        err_track_enable    = false;
	bool        coset               = false;
	bool        coded_monitoring    = false;
	bool        ter_sigma           = false;
	bool        mnt_mutinfo         = false;

#ifdef AFF3CT_MPI
	std::chrono::milliseconds mnt_mpi_comm_freq = std::chrono::milliseconds(1000);
#else
	std::chrono::milliseconds mnt_red_lazy_freq = std::chrono::milliseconds(0);
	bool                      mnt_red_lazy      = false;
#endif

	// module parameters
	tools::auto_cloned_unique_ptr<Source      > src;
	tools::auto_cloned_unique_ptr<CRC         > crc;
	tools::auto_cloned_unique_ptr<Codec       > cdc;
	tools::auto_cloned_unique_ptr<Modem       > mdm;
	tools::auto_cloned_unique_ptr<Channel     > chn;
	tools::auto_cloned_unique_ptr<Quantizer   > qnt;
	tools::auto_cloned_unique_ptr<Monitor_BFER> mnt_er;
	tools::auto_cloned_unique_ptr<Monitor_MI  > mnt_mi;
	tools::auto_cloned_unique_ptr<Terminal    > ter;

	// -------------------------------------------------------------------------------------------------------- METHODS
	virtual ~BFER() = default;
	virtual BFER* clone() const;

	virtual std::vector<std::string> get_names      () const;
	virtual std::vector<std::string> get_short_names() const;
	virtual std::vector<std::string> get_prefixes   () const;

	// setters
	void set_src   (Source       *src);
	void set_crc   (CRC          *crc);
	void set_cdc   (Codec        *cdc);
	void set_mdm   (Modem        *mdm);
	void set_chn   (Channel      *chn);
	void set_qnt   (Quantizer    *qnt);
	void set_mnt_mi(Monitor_MI   *mnt);
	void set_mnt_er(Monitor_BFER *mnt);
	void set_ter   (Terminal     *ter);

	const Codec* get_cdc() const;

	// parameters construction
	void get_description(cli::Argument_map_info &args) const;
	void store          (const cli::Argument_map_value &vals);
	void get_headers    (std::map<std::string,tools::header_list>& headers, const bool full = true) const;

protected:
	BFER(const std::string &n = BFER_name, const std::string &p = BFER_prefix);
};
}
}

#endif /* FACTORY_SIMULATION_BFER_HPP_ */
