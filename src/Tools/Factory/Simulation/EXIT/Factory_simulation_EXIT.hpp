#ifndef FACTORY_SIMULATION_EXIT_HPP_
#define FACTORY_SIMULATION_EXIT_HPP_

#include <string>
#include <thread>

#include "../Factory_simulation.hpp"
#include "Tools/Arguments_reader.hpp"

#include "Tools/Factory/Factory_source.hpp"
#include "Tools/Factory/Factory_modem.hpp"
#include "Tools/Factory/Factory_channel.hpp"
#include "Tools/Factory/Factory_quantizer.hpp"
#include "Tools/Factory/Factory_monitor.hpp"
#include "Tools/Factory/Factory_terminal.hpp"
#include "Tools/Factory/Factory_puncturer.hpp"
#include "Tools/Factory/Factory_encoder_common.hpp"
#include "Tools/Factory/Factory_decoder_common.hpp"

namespace aff3ct
{
namespace tools
{

struct Factory_simulation_EXIT : Factory_simulation
{
	struct simu_parameters_EXIT : simu_parameters
	{
		virtual ~simu_parameters_EXIT() {}

		// ---- simulation
		std::string snr_type   = "ES";
		float       sig_a_min  = 0.0f;
		float       sig_a_max  = 5.0f;
		float       sig_a_step = 0.5f;
		bool        debug      = false;
	};

	template <typename B = int, typename R = float, typename Q = R>
	struct chain_parameters_EXIT : chain_parameters
	{
		chain_parameters_EXIT()
		{
			if(this->sim == nullptr)
				this->sim = new simu_parameters_EXIT();
		}

		virtual ~chain_parameters_EXIT()
		{
			if(this->sim != nullptr)
				delete this->sim;

			this->sim = nullptr;
		}

		Factory_simulation_EXIT               ::simu_parameters_EXIT    *sim = nullptr;
		typename Factory_source        <B    >::source_parameters        src;
		typename Factory_modem         <B,R,Q>::modem_parameters         modem;
		typename Factory_channel       <  R  >::channel_parameters       chn;
		typename Factory_puncturer     <B,  Q>::puncturer_parameters    *pct = nullptr;
		typename Factory_encoder_common<B    >::encoder_parameters      *enc = nullptr;
		typename Factory_decoder_common       ::decoder_parameters      *dec = nullptr;
		typename Factory_terminal_EXIT        ::terminal_parameters_EXIT ter;
	};

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, simu_parameters_EXIT &params);
	static void group_args(Arguments_reader::arg_grp& ar);

	static void header(Header::params_list& head_sim, Header::params_list& head_cde, const simu_parameters_EXIT& params);
};

}
}

#endif /* FACTORY_SIMULATION_EXIT_HPP_ */
