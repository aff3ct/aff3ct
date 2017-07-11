#ifndef FACTORY_SIMULATION_BFER_ITE_HPP_
#define FACTORY_SIMULATION_BFER_ITE_HPP_

#include <string>

#include "Factory_simulation_BFER.hpp"
#include "Tools/Arguments_reader.hpp"

#include "Tools/Factory/Factory_CRC.hpp"
#include "Tools/Factory/Factory_quantizer.hpp"
#include "Tools/Factory/Factory_interleaver.hpp"
#include "Tools/Factory/Factory_puncturer.hpp"

namespace aff3ct
{
namespace tools
{

struct Factory_simulation_BFER_ite : Factory_simulation_BFER
{
	struct simu_parameters_BFER_ite : simu_parameters_BFER
	{
		virtual ~simu_parameters_BFER_ite() {}

		// ---- simulation
		int n_ite = 15;
	};

	template <typename B = int, typename R = float, typename Q = R>
	struct chain_parameters_BFER_ite : chain_parameters_BFER<B,R,Q>
	{
		chain_parameters_BFER_ite()
		{
			if(this->sim == nullptr)
				this->sim = new simu_parameters_BFER_ite();
		}

		virtual ~chain_parameters_BFER_ite()
		{
			if(this->sim != nullptr)
				delete this->sim;

			this->sim = nullptr;
		}

		typename Factory_CRC           <B    >::CRC_parameters          crc;
		typename Factory_quantizer     <  R,Q>::quantizer_parameters    qua;
		typename Factory_interleaver   <int  >::interleaver_parameters  itl;
		typename Factory_puncturer     <B,  Q>::puncturer_parameters   *pct = nullptr;
	};

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, simu_parameters_BFER_ite &params);
	static void group_args(Arguments_reader::arg_grp& ar);

	static void header(Header::params_list& head_sim, Header::params_list& head_cde, const simu_parameters_BFER_ite& params);
};

}
}

#endif /* FACTORY_SIMULATION_BFER_ITE_HPP_ */
