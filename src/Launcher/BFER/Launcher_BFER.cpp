#include <thread>
#include <string>
#include <iostream>

#include "Launcher_BFER.hpp"

using namespace aff3ct::tools;
using namespace aff3ct::launcher;
using namespace aff3ct::simulation;

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
	if (codec != nullptr)
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

	Factory_monitor<B>     ::build_args(this->req_args, this->opt_args);

	Factory_terminal_BFER  ::build_args(this->req_args, this->opt_args);
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

	Factory_monitor<B>     ::store_args(this->ar, m_chain_params->mon);

	Factory_terminal_BFER  ::store_args(this->ar, m_chain_params->ter);
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

	Factory_monitor<B>     ::group_args(this->arg_group);

	Factory_terminal_BFER  ::group_args(this->arg_group);
}

template <typename B, typename R, typename Q>
void Launcher_BFER<B,R,Q>
::print_header()
{
	Launcher<B,R,Q>::print_header();

	Factory_simulation_BFER_std::header(sim, cde, m_chain_params->sim);

	Factory_source<B>      ::header(src, m_chain_params->src);

	Factory_modem<B,R,Q>   ::header(mod, demod, m_chain_params->modem);

	Factory_channel<R>     ::header(chn, m_chain_params->chn);

	Factory_quantizer<R,Q> ::header(qua, m_chain_params->qua);

	Factory_monitor<B>     ::header(mon, m_chain_params->mon);

	Factory_terminal_BFER  ::header(ter, m_chain_params->ter);

	int max_n_chars = 0;
	Header::compute_max_n_chars(sim, max_n_chars);
	Header::compute_max_n_chars(cde, max_n_chars);
	Header::compute_max_n_chars(src, max_n_chars);
//	Header::compute_max_n_chars(crc, max_n_chars);
	Header::compute_max_n_chars(enc, max_n_chars);
//	Header::compute_max_n_chars(pct, max_n_chars);
//	Header::compute_max_n_chars(itl, max_n_chars);
	Header::compute_max_n_chars(mod, max_n_chars);
	Header::compute_max_n_chars(chn, max_n_chars);
	Header::compute_max_n_chars(demod, max_n_chars);
//	Header::compute_max_n_chars(depct, max_n_chars);
	Header::compute_max_n_chars(qua, max_n_chars);
	Header::compute_max_n_chars(dec, max_n_chars);
	Header::compute_max_n_chars(mon, max_n_chars);
	Header::compute_max_n_chars(ter, max_n_chars);

	if (sim.size()) Header::print_parameters("Simulation",  sim, max_n_chars, this->stream);
	if (cde.size()) Header::print_parameters("Code",        cde, max_n_chars, this->stream);
	if (src.size()) Header::print_parameters("Source",      src, max_n_chars, this->stream);
//	if (crc.size()) Header::print_parameters("CRC",         crc, max_n_chars, this->stream);
	if (enc.size()) Header::print_parameters("Encoder",     enc, max_n_chars, this->stream);
//	if (pct.size()) Header::print_parameters("Puncturer",   pct, max_n_chars, this->stream);
//	if (itl.size()) Header::print_parameters("Interleaver", itl, max_n_chars, this->stream);
	if (mod.size()) Header::print_parameters("Modulator",   mod, max_n_chars, this->stream);
	if (chn.size()) Header::print_parameters("Channel",     chn, max_n_chars, this->stream);
	if (demod.size()) Header::print_parameters("Demodulator", demod, max_n_chars, this->stream);
//	if (depct.size()) Header::print_parameters("Depuncturer", depct, max_n_chars, this->stream);
	if (qua.size()) Header::print_parameters("Quantizer",   qua, max_n_chars, this->stream);
	if (dec.size()) Header::print_parameters("Decoder",     dec, max_n_chars, this->stream);
	if (mon.size()) Header::print_parameters("Monitor",     mon, max_n_chars, this->stream);
	if (ter.size()) Header::print_parameters("Terminal",    ter, max_n_chars, this->stream);
	this->stream << "#" << std::endl;
}

template <typename B, typename R, typename Q>
Simulation* Launcher_BFER<B,R,Q>
::build_simu()
{
	this->build_codec();

#if defined(SYSTEMC)
	return new SC_Simulation_BFER_std     <B,R,Q>(this->params, *this->codec);
#elif defined(STARPU)
	return new SPU_Simulation_BFER_std    <B,R,Q>(this->params, *this->codec);
#else
	return new Simulation_BFER_std_threads<B,R,Q>(this->params, *this->codec);
#endif
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
