#ifndef FACTORY_SIMULATION_BFER_STD_HPP_
#define FACTORY_SIMULATION_BFER_STD_HPP_

#include <string>

#include "Factory_simulation_BFER.hpp"
#include "Tools/Arguments_reader.hpp"

#include "Tools/Factory/Factory_CRC.hpp"
#include "Tools/Factory/Factory_quantizer.hpp"
#include "Tools/Factory/Factory_puncturer.hpp"

namespace aff3ct
{
namespace tools
{

struct Factory_simulation_BFER_std : Factory_simulation_BFER
{
	struct parameters : Factory_simulation_BFER::parameters
	{
		virtual ~parameters() {}

		// ---- simulation
		bool debug_fe = false;
	};

	struct chain_parameters : Factory_simulation_BFER::chain_parameters
	{
		chain_parameters()
		{
			if(this->sim == nullptr)
				this->sim = new parameters();
		}

		virtual ~chain_parameters()
		{
			if(this->sim != nullptr)
				delete this->sim;

			this->sim = nullptr;
		}

		Factory_CRC      ::parameters  crc;
		Factory_quantizer::parameters  qnt;
		Factory_puncturer::parameters *pct = nullptr;
	};

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, parameters &params);
	static void group_args(Arguments_reader::arg_grp& ar);

	static void header(Header::params_list& head_sim, Header::params_list& head_cde, const parameters& params);
};

}
}

#endif /* FACTORY_SIMULATION_BFER_STD_HPP_ */
