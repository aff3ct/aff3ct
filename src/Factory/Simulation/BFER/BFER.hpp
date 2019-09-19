#ifndef FACTORY_SIMULATION_BFER_HPP_
#define FACTORY_SIMULATION_BFER_HPP_

#include <chrono>
#include <vector>
#include <string>
#include <map>

#include "Tools/Arguments/Argument_tools.hpp"
#include "Tools/auto_cloned_unique_ptr.hpp"
#include "Factory/Module/Channel/Channel.hpp"
#include "Factory/Module/CRC/CRC.hpp"
#include "Factory/Module/Codec/Codec.hpp"
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
struct BFER : Simulation
{
	class parameters : public Simulation::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// optional parameters
		std::string err_track_path      = "error_tracker";
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
		tools::auto_cloned_unique_ptr<Source       ::parameters> src;
		tools::auto_cloned_unique_ptr<CRC          ::parameters> crc;
		tools::auto_cloned_unique_ptr<Codec        ::parameters> cdc;
		tools::auto_cloned_unique_ptr<Modem        ::parameters> mdm;
		tools::auto_cloned_unique_ptr<Channel      ::parameters> chn;
		tools::auto_cloned_unique_ptr<Quantizer    ::parameters> qnt;
		tools::auto_cloned_unique_ptr<Monitor_BFER ::parameters> mnt_er;
		tools::auto_cloned_unique_ptr<Monitor_MI   ::parameters> mnt_mi;
		tools::auto_cloned_unique_ptr<Terminal     ::parameters> ter;

		// ---------------------------------------------------------------------------------------------------- METHODS
		virtual ~parameters() = default;
		virtual BFER::parameters* clone() const;

		virtual std::vector<std::string> get_names      () const;
		virtual std::vector<std::string> get_short_names() const;
		virtual std::vector<std::string> get_prefixes   () const;

		// setters
		void set_src   (Source      ::parameters *src);
		void set_crc   (CRC         ::parameters *crc);
		void set_cdc   (Codec       ::parameters *cdc);
		void set_mdm   (Modem       ::parameters *mdm);
		void set_chn   (Channel     ::parameters *chn);
		void set_qnt   (Quantizer   ::parameters *qnt);
		void set_mnt_mi(Monitor_MI  ::parameters *mnt);
		void set_mnt_er(Monitor_BFER::parameters *mnt);
		void set_ter   (Terminal    ::parameters *ter);

		const Codec::parameters* get_cdc() const;

		// parameters construction
		void get_description(tools::Argument_map_info &args) const;
		void store          (const tools::Argument_map_value &vals);
		void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

	protected:
		parameters(const std::string &n = BFER_name, const std::string &p = BFER_prefix);
	};
};
}
}

#endif /* FACTORY_SIMULATION_BFER_HPP_ */
