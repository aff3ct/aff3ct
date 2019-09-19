#if !defined(AFF3CT_8BIT_PREC) && !defined(AFF3CT_16BIT_PREC)

#ifndef FACTORY_SIMULATION_EXIT_HPP_
#define FACTORY_SIMULATION_EXIT_HPP_

#include <vector>
#include <string>
#include <map>

#include "Tools/Arguments/Argument_tools.hpp"
#include "Tools/auto_cloned_unique_ptr.hpp"
#include "Factory/Module/Source/Source.hpp"
#include "Factory/Module/Modem/Modem.hpp"
#include "Factory/Module/Channel/Channel.hpp"
#include "Factory/Module/Quantizer/Quantizer.hpp"
#include "Factory/Module/Monitor/EXIT/Monitor_EXIT.hpp"
#include "Factory/Module/Codec/Codec_SISO.hpp"
#include "Factory/Tools/Display/Terminal/Terminal.hpp"
#include "Factory/Simulation/Simulation.hpp"

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
		std::vector<float> sig_a_range;

		// optional parameters
		std::string snr_type = "ES";

		// module parameters
		tools::auto_cloned_unique_ptr<Source      ::parameters> src;
		tools::auto_cloned_unique_ptr<Codec_SISO  ::parameters> cdc;
		tools::auto_cloned_unique_ptr<Modem       ::parameters> mdm;
		tools::auto_cloned_unique_ptr<Channel     ::parameters> chn;
		tools::auto_cloned_unique_ptr<Quantizer   ::parameters> qnt;
		tools::auto_cloned_unique_ptr<Monitor_EXIT::parameters> mnt;
		tools::auto_cloned_unique_ptr<Terminal    ::parameters> ter;

		// ---------------------------------------------------------------------------------------------------- METHODS
		explicit parameters(const std::string &p = EXIT_prefix);
		virtual ~parameters() = default;
		virtual EXIT::parameters* clone() const;

		virtual std::vector<std::string> get_names      () const;
		virtual std::vector<std::string> get_short_names() const;
		virtual std::vector<std::string> get_prefixes   () const;

		// setters
		void set_src(Source      ::parameters *src);
		void set_cdc(Codec_SISO  ::parameters *cdc);
		void set_mdm(Modem       ::parameters *mdm);
		void set_chn(Channel     ::parameters *chn);
		void set_qnt(Quantizer   ::parameters *qnt);
		void set_mnt(Monitor_EXIT::parameters *mnt);
		void set_ter(Terminal    ::parameters *ter);

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