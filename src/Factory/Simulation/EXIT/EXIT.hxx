#include "Simulation/EXIT/EXIT.hpp"

#include "EXIT.hpp"

namespace aff3ct
{
namespace factory
{
template <class C>
template <typename B, typename R>
simulation::EXIT<C,B,R>* EXIT::parameters<C>
::build() const
{
	return new simulation::EXIT<C,B,R>(*this);
}

template <class C, typename B, typename R>
simulation::EXIT<C,B,R>* EXIT
::build(const parameters<C> &params)
{
	return params.template build<B,R>();
}

template <class C>
void EXIT::store_args(const arg_val_map &vals, parameters<C> &params, const std::string p)
{
	Simulation::store_args(vals, params, p);

	if(exist(vals, {p+"-siga-min", "a"})) params.sig_a_min  = std::stof(vals.at({p+"-siga-min", "a"}));
	if(exist(vals, {p+"-siga-max", "A"})) params.sig_a_max  = std::stof(vals.at({p+"-siga-max", "A"}));
	if(exist(vals, {p+"-siga-step"    })) params.sig_a_step = std::stof(vals.at({p+"-siga-step"    }));
}

template <class C>
void EXIT::make_header(params_list& head_sim, const parameters<C> &params, const bool full)
{
	Simulation::make_header(head_sim, params, full);

	head_sim.push_back(std::make_pair("Sigma-a min (a)",  std::to_string(params.sig_a_min )));
	head_sim.push_back(std::make_pair("Sigma-a max (A)",  std::to_string(params.sig_a_max )));
	head_sim.push_back(std::make_pair("Sigma-a step",     std::to_string(params.sig_a_step)));

	const auto bit_rate = (float)params.src.K / (float)params.cdc.enc.N_cw;
	head_sim.push_back(std::make_pair("Bit rate", std::to_string(bit_rate)));
	head_sim.push_back(std::make_pair("Inter frame level", std::to_string(params.src.n_frames)));
}
}
}
