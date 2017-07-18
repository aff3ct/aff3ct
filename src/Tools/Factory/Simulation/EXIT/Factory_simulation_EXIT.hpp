#ifndef FACTORY_SIMULATION_EXIT_HPP_
#define FACTORY_SIMULATION_EXIT_HPP_

#include <string>
#include <thread>

#include "Tools/Arguments_reader.hpp"
#include "Tools/Factory/Module/Factory_source.hpp"
#include "Tools/Factory/Module/Factory_modem.hpp"
#include "Tools/Factory/Module/Factory_channel.hpp"
#include "Tools/Factory/Module/Factory_quantizer.hpp"
#include "Tools/Factory/Module/Factory_monitor.hpp"
#include "Tools/Factory/Module/Code/Factory_puncturer.hpp"
#include "Tools/Factory/Module/Code/Factory_encoder.hpp"
#include "Tools/Factory/Module/Code/Factory_decoder.hpp"
#include "Tools/Factory/Simulation/EXIT/Factory_terminal_EXIT.hpp"

#include "../Factory_simulation.hpp"

namespace aff3ct
{
namespace tools
{

struct Factory_simulation_EXIT : Factory_simulation
{
	struct parameters : Factory_simulation::parameters
	{
		parameters()
		: Factory_simulation::parameters(),
		  src(new Factory_source       ::parameters()),
		  enc(new Factory_encoder      ::parameters()),
		  pct(new Factory_puncturer    ::parameters()),
		  mdm(new Factory_modem        ::parameters()),
		  chn(new Factory_channel      ::parameters()),
		  dec(new Factory_decoder      ::parameters()),
		  ter(new Factory_terminal_EXIT::parameters())
		{
		}

		virtual ~parameters()
		{
			if (this->src != nullptr) { delete this->src; this->src = nullptr; }
			if (this->enc != nullptr) { delete this->enc; this->enc = nullptr; }
			if (this->pct != nullptr) { delete this->pct; this->pct = nullptr; }
			if (this->mdm != nullptr) { delete this->mdm; this->mdm = nullptr; }
			if (this->chn != nullptr) { delete this->chn; this->chn = nullptr; }
			if (this->dec != nullptr) { delete this->dec; this->dec = nullptr; }
			if (this->ter != nullptr) { delete this->ter; this->ter = nullptr; }
		}

		std::string snr_type   = "ES";
		float       sig_a_min  = 0.0f;
		float       sig_a_max  = 5.0f;
		float       sig_a_step = 0.5f;
		bool        debug      = false;

		Factory_source       ::parameters *src = nullptr;
		Factory_encoder      ::parameters *enc = nullptr;
		Factory_puncturer    ::parameters *pct = nullptr;
		Factory_modem        ::parameters *mdm = nullptr;
		Factory_channel      ::parameters *chn = nullptr;
		Factory_decoder      ::parameters *dec = nullptr;
		Factory_terminal_EXIT::parameters *ter = nullptr;
	};

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, parameters &params);
	static void group_args(Arguments_reader::arg_grp& ar);

	static void header(params_list& head_sim, const parameters& params);
};

}
}

#endif /* FACTORY_SIMULATION_EXIT_HPP_ */
