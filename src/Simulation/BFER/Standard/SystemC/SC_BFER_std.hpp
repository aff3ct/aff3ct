#ifdef AFF3CT_SYSTEMC_SIMU

#ifndef SC_SIMULATION_BFER_STD_HPP_
#define SC_SIMULATION_BFER_STD_HPP_

#include <vector>
#include <memory>

#include "Tools/SystemC/SC_Duplicator.hpp"
#include "Factory/Simulation/BFER/BFER_std.hpp"
#include "Simulation/BFER/Standard/BFER_std.hpp"

namespace aff3ct
{
namespace simulation
{
template <typename B = int, typename R = float, typename Q = R>
class SC_BFER_std : public BFER_std<B,R,Q>
{
protected:
	std::vector<std::unique_ptr<tools::SC_Duplicator>> duplicator;

public:
	explicit SC_BFER_std(const factory::BFER_std::parameters &params_BFER_std);
	virtual ~SC_BFER_std() = default;

protected:
	void create_sc_modules();

	virtual void __build_communication_chain(const int tid = 0);
	virtual void _launch();

private:
	void bind_sockets();
};
}
}

#endif /* SC_SIMULATION_BFER_STD_HPP_ */

#endif /* AFF3CT_SYSTEMC_SIMU */
