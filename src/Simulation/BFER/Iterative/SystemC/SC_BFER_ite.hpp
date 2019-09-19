#ifdef AFF3CT_SYSTEMC_SIMU

#ifndef SC_SIMULATION_BFER_ITE_HPP_
#define SC_SIMULATION_BFER_ITE_HPP_

#include <vector>
#include <memory>

#include "Tools/SystemC/SC_Router.hpp"
#include "Tools/SystemC/SC_Funnel.hpp"
#include "Tools/SystemC/SC_Predicate.hpp"
#include "Tools/SystemC/SC_Duplicator.hpp"
#include "Tools/Display/Terminal/Terminal.hpp"
#include "Factory/Simulation/BFER/BFER_ite.hpp"
#include "Module/Coset/Coset.hpp"
#include "Simulation/BFER/Iterative/BFER_ite.hpp"

namespace aff3ct
{
namespace simulation
{
template <typename B = int, typename R = float, typename Q = R>
class SC_BFER_ite : public BFER_ite<B,R,Q>
{
protected:
	std::vector<std::unique_ptr<tools::SC_Duplicator>> duplicator;
	            std::unique_ptr<module::Coset<B,Q>>    coset_real_i;
	            std::unique_ptr<tools::SC_Router>      router;
	            std::unique_ptr<tools::SC_Funnel>      funnel;
	            std::unique_ptr<tools::SC_Predicate>   predicate;

public:
	explicit SC_BFER_ite(const factory::BFER_ite::parameters &params_BFER_ite);
	virtual ~SC_BFER_ite() = default;

protected:
	void create_sc_modules();

	virtual void __build_communication_chain(const int tid = 0);
	virtual void _launch();

	virtual std::unique_ptr<module::Coset<B,Q>> build_coset_real(const int tid = 0);

private:
	void bind_sockets      ();
	void bind_sockets_debug();
};
}
}

#endif /* SC_SIMULATION_BFER_ITE_HPP_ */

#endif /* AFF3CT_SYSTEMC_SIMU */
