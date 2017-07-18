#ifndef FACTORY_SIMULATION_BFER_HPP_
#define FACTORY_SIMULATION_BFER_HPP_

#include <string>
#include <thread>

#include "Tools/Arguments_reader.hpp"

#include "Tools/Factory/Module/Factory_CRC.hpp"
#include "Tools/Factory/Module/Factory_source.hpp"
#include "Tools/Factory/Module/Factory_modem.hpp"
#include "Tools/Factory/Module/Factory_channel.hpp"
#include "Tools/Factory/Module/Factory_quantizer.hpp"
#include "Tools/Factory/Module/Factory_monitor.hpp"
#include "Tools/Factory/Module/Code/Factory_puncturer.hpp"
#include "Tools/Factory/Module/Code/Factory_encoder.hpp"
#include "Tools/Factory/Module/Code/Factory_decoder.hpp"
#include "Tools/Factory/Module/Code/Factory_puncturer.hpp"
#include "Tools/Factory/Simulation/BFER/Factory_terminal_BFER.hpp"

#include "../Factory_simulation.hpp"

namespace aff3ct
{
namespace tools
{

struct Factory_simulation_BFER : Factory_simulation
{
	struct parameters : Factory_simulation::parameters
	{
		parameters()
		: Factory_simulation::parameters(),
		  src(new Factory_source       ::parameters()),
		  crc(new Factory_CRC          ::parameters()),
		  enc(new Factory_encoder      ::parameters()),
		  pct(new Factory_puncturer    ::parameters()),
		  mdm(new Factory_modem        ::parameters()),
		  chn(new Factory_channel      ::parameters()),
		  qnt(new Factory_quantizer    ::parameters()),
		  dec(new Factory_decoder      ::parameters()),
		  mnt(new Factory_monitor      ::parameters()),
		  ter(new Factory_terminal_BFER::parameters())
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

		std::string snr_type        = "EB";
		bool        time_report     = false;
		bool        debug           = false;
		int         debug_limit     = 0;
		int         debug_precision = 5;
		int         benchs          = 0;

		Factory_source       ::parameters *src;
		Factory_CRC          ::parameters *crc;
		Factory_encoder      ::parameters *enc;
		Factory_puncturer    ::parameters *pct;
		Factory_modem        ::parameters *mdm;
		Factory_channel      ::parameters *chn;
		Factory_quantizer    ::parameters *qnt;
		Factory_decoder      ::parameters *dec;
		Factory_monitor      ::parameters *mnt;
		Factory_terminal_BFER::parameters *ter;
	};

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, parameters &params);
	static void group_args(Arguments_reader::arg_grp& ar);

	static void header(params_list& head_sim, const parameters& params);
};

}
}

#endif /* FACTORY_SIMULATION_BFER_HPP_ */
