#ifndef FACTORY_SIMULATION_BFER_STD_HPP_
#define FACTORY_SIMULATION_BFER_STD_HPP_

#include <string>

#include "Factory/Module/Codec/Codec_SIHO.hpp"

#include "BFER.hpp"

namespace aff3ct
{
namespace simulation
{
template <typename B, typename R, typename Q>
class BFER_std;
}
}

namespace aff3ct
{
namespace factory
{
struct BFER_std : BFER
{
	static const std::string name;
	static const std::string prefix;

	class parameters : public BFER::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// module parameters
		Codec_SIHO::parameters *cdc = nullptr;

		// ---------------------------------------------------------------------------------------------------- METHODS
		parameters(const std::string &p = BFER_std::prefix);
		virtual ~parameters();
		BFER_std::parameters* clone() const;

		// setters
		void set_cdc(Codec_SIHO::parameters *cdc) { this->cdc = cdc; BFER::parameters::set_cdc(cdc); }

		// parameters construction
		void get_description(arg_map &req_args, arg_map &opt_args                              ) const;
		void store          (const arg_val_map &vals                                           );
		void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		template <typename B = int, typename R = float, typename Q = R>
		simulation::BFER_std<B,R,Q>* build() const;
	};

	template <typename B = int, typename R = float, typename Q = R>
	static simulation::BFER_std<B,R,Q>* build(const parameters &params);
};
}
}

#endif /* FACTORY_SIMULATION_BFER_STD_HPP_ */
