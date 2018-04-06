#if !defined(PREC_8_BIT) && !defined(PREC_16_BIT)

#ifndef FACTORY_SIMULATION_EXIT_HPP_
#define FACTORY_SIMULATION_EXIT_HPP_

#include <string>
#include <thread>

#include "Factory/Module/Source/Source.hpp"
#include "Factory/Module/Modem/Modem.hpp"
#include "Factory/Module/Codec/Codec.hpp"
#include "Factory/Module/Channel/Channel.hpp"
#include "Factory/Module/Quantizer/Quantizer.hpp"
#include "Factory/Module/Monitor/EXIT/Monitor_EXIT.hpp"
#include "Factory/Module/Codec/Codec_SISO.hpp"
#include "Factory/Tools/Display/Terminal/EXIT/Terminal_EXIT.hpp"

#include "../Simulation.hpp"

namespace aff3ct
{
namespace simulation
{
template <typename B, typename R>
class EXIT;
}
}

namespace aff3ct
{
namespace factory
{
extern const std::string EXIT_name;
extern const std::string EXIT_prefix;
struct EXIT : Simulation
{
	class parameters : public Simulation::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// required parameters
		// float       sig_a_min  = 0.0f;
		// float       sig_a_max  = 5.0f;
		std::vector<float> sig_a_range;

		// optional parameters
		std::string snr_type   = "ES";
		// float       sig_a_step = 0.5f;

		// module parameters
		Source       ::parameters *src = nullptr;
		Codec_SISO   ::parameters *cdc = nullptr;
		Modem        ::parameters *mdm = nullptr;
		Channel      ::parameters *chn = nullptr;
		Quantizer    ::parameters *qnt = nullptr;
		Monitor_EXIT ::parameters *mnt = nullptr;
		Terminal_EXIT::parameters *ter = nullptr;

		// ---------------------------------------------------------------------------------------------------- METHODS
		explicit parameters(const std::string &p = EXIT_prefix);
		virtual ~parameters();
		virtual EXIT::parameters* clone() const;

		virtual std::vector<std::string> get_names      () const;
		virtual std::vector<std::string> get_short_names() const;
		virtual std::vector<std::string> get_prefixes   () const;

		// setters
		void set_src(Source       ::parameters *src) { this->src = src; }
		void set_cdc(Codec_SISO   ::parameters *cdc) { this->cdc = cdc; }
		void set_mdm(Modem        ::parameters *mdm) { this->mdm = mdm; }
		void set_chn(Channel      ::parameters *chn) { this->chn = chn; }
		void set_qnt(Quantizer    ::parameters *qnt) { this->qnt = qnt; }
		void set_mnt(Monitor_EXIT ::parameters *mnt) { this->mnt = mnt; }
		void set_ter(Terminal_EXIT::parameters *ter) { this->ter = ter; }

		// parameters construction
		void get_description(tools::Argument_map_info &args) const;
		void store          (const tools::Argument_map_value &vals);
		void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		template <typename B = int, typename R = float>
		simulation::EXIT<B,R>* build() const;
	};

	template <typename B = int, typename R = float>
	static simulation::EXIT<B,R>* build(const parameters &params);
};
}
}

#endif /* FACTORY_SIMULATION_EXIT_HPP_ */

#endif