#ifndef FACTORY_SIMULATION_BFER_HPP_
#define FACTORY_SIMULATION_BFER_HPP_

#include <string>

#include "Factory/Module/Channel.hpp"
#include "Factory/Module/Code/Decoder.hpp"
#include "Factory/Module/Code/Encoder.hpp"
#include "Factory/Module/Code/Puncturer.hpp"
#include "Factory/Module/Code/Puncturer.hpp"
#include "Factory/Module/CRC.hpp"
#include "Factory/Module/Modem.hpp"
#include "Factory/Module/Monitor.hpp"
#include "Factory/Module/Quantizer.hpp"
#include "Factory/Module/Source.hpp"
#include "Factory/Tools/Display/Terminal/BFER/Terminal_BFER.hpp"

#include "../Simulation.hpp"

namespace aff3ct
{
namespace factory
{
struct BFER : Simulation
{
	static const std::string name;
	static const std::string prefix;

	struct parameters : Simulation::parameters
	{
		parameters()
		: Simulation::parameters(),
		  src(new Source       ::parameters()),
		  crc(new CRC          ::parameters()),
		  enc(new Encoder      ::parameters()),
		  pct(new Puncturer    ::parameters()),
		  mdm(new Modem        ::parameters()),
		  chn(new Channel      ::parameters()),
		  qnt(new Quantizer    ::parameters()),
		  dec(new Decoder      ::parameters()),
		  mnt(new Monitor      ::parameters()),
		  ter(new Terminal_BFER::parameters())
		{
		}

		virtual ~parameters()
		{
			if (this->src != nullptr) { delete this->src; this->src = nullptr; }
			if (this->crc != nullptr) { delete this->crc; this->crc = nullptr; }
			if (this->enc != nullptr) { delete this->enc; this->enc = nullptr; }
			if (this->pct != nullptr) { delete this->pct; this->pct = nullptr; }
			if (this->mdm != nullptr) { delete this->mdm; this->mdm = nullptr; }
			if (this->chn != nullptr) { delete this->chn; this->chn = nullptr; }
			if (this->qnt != nullptr) { delete this->qnt; this->qnt = nullptr; }
			if (this->dec != nullptr) { delete this->dec; this->dec = nullptr; }
			if (this->mnt != nullptr) { delete this->mnt; this->mnt = nullptr; }
			if (this->ter != nullptr) { delete this->ter; this->ter = nullptr; }
		}

		std::string snr_type         = "EB";
		std::string err_track_path   = "error_tracker";
		bool        err_track_revert = false;
		bool        err_track_enable = false;
		bool        time_report      = false;
		bool        debug            = false;
		bool        coset            = false;
		int         debug_limit      = 0;
		int         debug_precision  = 5;
		int         benchs           = 0;

		Source       ::parameters *src;
		CRC          ::parameters *crc;
		Encoder      ::parameters *enc;
		Puncturer    ::parameters *pct;
		Modem        ::parameters *mdm;
		Channel      ::parameters *chn;
		Quantizer    ::parameters *qnt;
		Decoder      ::parameters *dec;
		Monitor      ::parameters *mnt;
		Terminal_BFER::parameters *ter;
	};

	static void build_args(arg_map &req_args, arg_map &opt_args, const std::string p = prefix);
	static void store_args(const arg_val_map &vals, parameters &params, const std::string p = prefix);
	static void make_header(params_list& head_sim, const parameters& params);
};
}
}

#endif /* FACTORY_SIMULATION_BFER_HPP_ */
