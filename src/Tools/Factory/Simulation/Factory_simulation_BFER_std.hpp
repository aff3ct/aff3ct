#ifndef FACTORY_SIMULATION_BFER_STD_HPP_
#define FACTORY_SIMULATION_BFER_STD_HPP_

#include <string>

#include "Factory_simulation_BFER.hpp"
#include "Tools/Arguments_reader.hpp"

#include "Tools/Factory/Factory_source.hpp"
#include "Tools/Factory/Factory_modem.hpp"
#include "Tools/Factory/Factory_channel.hpp"
#include "Tools/Factory/Factory_quantizer.hpp"
#include "Tools/Factory/Factory_encoder_common.hpp"
#include "Tools/Factory/Factory_decoder_common.hpp"

namespace aff3ct
{
namespace tools
{

struct Factory_simulation_BFER_std : Factory_simulation_BFER
{
	struct simu_parameters_BFER_std : simu_parameters_BFER
	{
		bool debug_fe = false;
	};

	template <typename B = int, typename R = float, typename Q = R>
	struct chain_parameters_BFER_std : chain_parameters
	{
		virtual ~chain_parameters_BFER_std() {}

		Factory_simulation_BFER_std           ::simu_parameters_BFER_std sim;
		typename Factory_source        <B    >::source_parameters        src;
		typename Factory_modem         <B,R,Q>::modem_parameters         modem;
		typename Factory_channel       <  R  >::channel_parameters       chn;
		typename Factory_quantizer     <  R,Q>::quantizer_parameters     qua;
		typename Factory_encoder_common<B    >::encoder_parameters      *enc;
		typename Factory_decoder_common       ::decoder_parameters      *dec;
	};

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, simu_parameters_BFER_std &params);
	static void group_args(Arguments_reader::arg_grp& ar);
};

}
}

#endif /* FACTORY_SIMULATION_BFER_STD_HPP_ */
