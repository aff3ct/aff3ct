#ifndef FACTORY_MODEM_HPP
#define FACTORY_MODEM_HPP

#include <string>

#include "Tools/Math/max.h"

#include "Module/Modem/Modem.hpp"

#include "Factory.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename R = float, typename Q = R>
struct Factory_modem : public Factory
{
	static module::Modem<B,R,Q>* build(const std::string type,
	                                   const int         N,
	                                   const float       sigma,
	                                   const std::string max_type = "MAX",
	                                   const std::string psi_type = "PSI0",
	                                   const int         bps      = 1,
	                                   const std::string path     = "",
	                                   const int         upf      = 5,
	                                   const int         cpm_L    = 3,
	                                   const int         cpm_k    = 1,
	                                   const int         cpm_p    = 2,
	                                   const std::string mapping  = "NATURAL",
	                                   const std::string wave     = "GMSK",
	                                   const bool        no_sig2  = false,
	                                   const int         n_ite    = 1,
	                                   const int         n_frames = 1);

	static int get_buffer_size_after_modulation(const std::string type,
	                                            const int         N,
	                                            const int         bps   = 1,
	                                            const int         upf   = 5,
	                                            const int         cpm_L = 3);

	static int get_buffer_size_after_filtering(const std::string type,
	                                           const int         N,
	                                           const int         bps   = 1,
	                                           const int         cpm_L = 3,
	                                           const int         cpm_p = 2);

private:
	template <proto_max<Q> MAX>
	static module::Modem<B,R,Q>* _build(const std::string type,
	                                    const int         N,
	                                    const float       sigma,
	                                    const int         bps      = 1,
	                                    const std::string path     = "",
	                                    const int         upf      = 5,
	                                    const int         cpm_L    = 3,
	                                    const int         cpm_k    = 1,
	                                    const int         cpm_p    = 2,
	                                    const std::string mapping  = "NATURAL",
	                                    const std::string wave     = "GMSK",
	                                    const bool        no_sig2  = false,
	                                    const int         n_frames = 1);

	static module::Modem<B,R,Q>* _build_scma(const std::string type,
	                                         const int         N,
	                                         const float       sigma,
	                                         const int         bps      = 3,
	                                         const std::string psi_type = "PSI0",
	                                         const bool        no_sig2  = false,
	                                         const int         n_ite    = 1,
	                                         const int         n_frames = 6);
};
}
}

#endif /* FACTORY_MODEM_HPP */
