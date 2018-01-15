#ifndef FACTORY_SIMULATION_BFER_HPP_
#define FACTORY_SIMULATION_BFER_HPP_

#include <string>

#include "Factory/Module/Channel/Channel.hpp"
#include "Factory/Module/CRC/CRC.hpp"
#include "Factory/Module/Codec/Codec.hpp"
#include "Factory/Module/Modem/Modem.hpp"
#include "Factory/Module/Quantizer/Quantizer.hpp"
#include "Factory/Module/Source/Source.hpp"
#include "Factory/Module/Monitor/BFER/Monitor_BFER.hpp"
#include "Factory/Tools/Display/Terminal/BFER/Terminal_BFER.hpp"

#include "../Simulation.hpp"

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
		std::string snr_type            = "EB";
		std::string err_track_path      = "error_tracker";
		int         err_track_threshold = 0;
		bool        err_track_revert    = false;
		bool        err_track_enable    = false;
		bool        coset               = false;
		bool        coded_monitoring    = false;

		// module parameters
		Source       ::parameters *src = nullptr;
		CRC          ::parameters *crc = nullptr;
		Codec        ::parameters *cdc = nullptr;
		Modem        ::parameters *mdm = nullptr;
		Channel      ::parameters *chn = nullptr;
		Quantizer    ::parameters *qnt = nullptr;
		Monitor_BFER ::parameters *mnt = nullptr;
		Terminal_BFER::parameters *ter = nullptr;

		// ---------------------------------------------------------------------------------------------------- METHODS
		virtual ~parameters();
		virtual BFER::parameters* clone() const;

		virtual std::vector<std::string> get_names      () const;
		virtual std::vector<std::string> get_short_names() const;
		virtual std::vector<std::string> get_prefixes   () const;

		// setters
		        void set_src(Source       ::parameters *src) { this->src = src; }
		        void set_crc(CRC          ::parameters *crc) { this->crc = crc; }
		virtual void set_cdc(Codec        ::parameters *cdc) { this->cdc = cdc; }
		        void set_mdm(Modem        ::parameters *mdm) { this->mdm = mdm; }
		        void set_chn(Channel      ::parameters *chn) { this->chn = chn; }
		        void set_qnt(Quantizer    ::parameters *qnt) { this->qnt = qnt; }
		        void set_mnt(Monitor_BFER ::parameters *mnt) { this->mnt = mnt; }
		        void set_ter(Terminal_BFER::parameters *ter) { this->ter = ter; }

		// parameters construction
		void get_description(tools::Argument_map_info &req_args, tools::Argument_map_info &opt_args) const;
		void store          (const tools::Argument_map_value &vals                                 );
		void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

	protected:
		parameters(const std::string &n = BFER_name, const std::string &p = BFER_prefix);
	};
};
}
}

#endif /* FACTORY_SIMULATION_BFER_HPP_ */
