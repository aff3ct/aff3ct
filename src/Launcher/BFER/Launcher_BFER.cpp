#include <thread>
#include <string>
#include <iostream>

#include "Launcher_BFER.hpp"

using namespace aff3ct::tools;
using namespace aff3ct::launcher;

template <typename B, typename R, typename Q>
Launcher_BFER<B,R,Q>
::Launcher_BFER(const int argc, const char **argv, std::ostream &stream)
: Launcher<B,R,Q>(argc, argv, stream), codec(nullptr), m_chain_params(new Factory_simulation_BFER_std::chain_parameters_BFER_std<B,R,Q>())
{
	this->chain_params =  m_chain_params;
	this->simu_params  = &m_chain_params->sim;
}

template <typename B, typename R, typename Q>
Launcher_BFER<B,R,Q>
::~Launcher_BFER()
{
	if (codec              != nullptr)
		delete codec;

	if (m_chain_params != nullptr)
		delete m_chain_params;
}

template <typename B, typename R, typename Q>
void Launcher_BFER<B,R,Q>
::build_args()
{
	Launcher<B,R,Q>::build_args();

	Factory_simulation_BFER_std::build_args(this->req_args, this->opt_args);

	Factory_source<B>      ::build_args(this->req_args, this->opt_args);

	Factory_modem<B,R,Q>   ::build_args(this->req_args, this->opt_args);

	Factory_channel<R>     ::build_args(this->req_args, this->opt_args);

	Factory_quantizer<R,Q> ::build_args(this->req_args, this->opt_args);
}

template <typename B, typename R, typename Q>
void Launcher_BFER<B,R,Q>
::store_args()
{
	Launcher<B,R,Q>::store_args();

	Factory_simulation_BFER_std::store_args(this->ar, m_chain_params->sim);

	Factory_source<B>      ::store_args(this->ar, m_chain_params->src);

	Factory_modem<B,R,Q>   ::store_args(this->ar, m_chain_params->modem, m_chain_params->sim.N);

	Factory_channel<R>     ::store_args(this->ar, m_chain_params->chn);

	Factory_quantizer<R,Q> ::store_args(this->ar, m_chain_params->qua);
}

template <typename B, typename R, typename Q>
void Launcher_BFER<B,R,Q>
::group_args()
{
	Launcher<B,R,Q>::group_args();

	Factory_simulation_BFER_std::group_args(this->arg_group);

	Factory_source<B>      ::group_args(this->arg_group);

	Factory_modem<B,R,Q>   ::group_args(this->arg_group);

	Factory_channel<R>     ::group_args(this->arg_group);

	Factory_quantizer<R,Q> ::group_args(this->arg_group);
}

template <typename B, typename R, typename Q>
std::vector<std::pair<std::string,std::string>> Launcher_BFER<B,R,Q>
::header_simulation()
{
	std::string threads = "unused";
	if (this->params.simulation.n_threads)
		threads = std::to_string(this->params.simulation.n_threads) + " thread(s)";

	auto p = Launcher<B,R,Q>::header_simulation();

#ifdef STARPU
	p.push_back(std::make_pair("Task concurrency level (t)", std::to_string(this->params.simulation.n_threads)));
#else
	p.push_back(std::make_pair("Multi-threading (t)", threads));
#endif

	return p;
}

template <typename B, typename R, typename Q>
std::vector<std::pair<std::string,std::string>> Launcher_BFER<B,R,Q>
::header_code()
{
	std::string coset = this->params.code.coset ? "on" : "off";

	auto p = Launcher<B,R,Q>::header_code();

	p.push_back(std::make_pair("Coset approach (c)", coset));

	return p;
}

template <typename B, typename R, typename Q>
std::vector<std::pair<std::string,std::string>> Launcher_BFER<B,R,Q>
::header_encoder()
{
	std::string syst_enc = ((this->params.encoder.systematic) ? "on" : "off");

	auto p = Launcher<B,R,Q>::header_encoder();

	p.push_back(std::make_pair("Type", this->params.encoder.type));

	if (this->params.encoder.type == "USER")
		p.push_back(std::make_pair("Path", this->params.encoder.path));

	p.push_back(std::make_pair("Systematic encoding", syst_enc));

	return p;
}

template <typename B, typename R, typename Q>
std::vector<std::pair<std::string,std::string>> Launcher_BFER<B,R,Q>
::header_decoder()
{
	auto p = Launcher<B,R,Q>::header_decoder();

	p.push_back(std::make_pair("Type (D)",       this->params.decoder.type  ));
	p.push_back(std::make_pair("Implementation", this->params.decoder.implem));

	return p;
}

template <typename B, typename R, typename Q>
std::vector<std::pair<std::string,std::string>> Launcher_BFER<B,R,Q>
::header_monitor()
{
	auto p = Launcher<B,R,Q>::header_monitor();

	p.push_back(std::make_pair("Frame error count (e)", std::to_string(this->params.monitor.n_frame_errors)));

	std::string enable_track = (this->params.monitor.err_track_enable) ? "on" : "off";
	p.push_back(std::make_pair("Bad frames tracking", enable_track));

	std::string enable_rev_track = (this->params.monitor.err_track_revert) ? "on" : "off";
	p.push_back(std::make_pair("Bad frames replay", enable_rev_track));

	if (this->params.monitor.err_track_enable || this->params.monitor.err_track_revert)
	{
		std::string path = this->params.monitor.err_track_path + std::string("_$snr.[src,enc,chn]");
		p.push_back(std::make_pair("Bad frames base path", path));
	}

	return p;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::launcher::Launcher_BFER<B_8,R_8,Q_8>;
template class aff3ct::launcher::Launcher_BFER<B_16,R_16,Q_16>;
template class aff3ct::launcher::Launcher_BFER<B_32,R_32,Q_32>;
template class aff3ct::launcher::Launcher_BFER<B_64,R_64,Q_64>;
#else
template class aff3ct::launcher::Launcher_BFER<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation
