#ifndef SIMULATION_LDPC_HPP_
#define SIMULATION_LDPC_HPP_

#include "../Simulation_BFER.hpp"

template <typename B, typename R, typename Q>
class Simulation_BFER_LDPC : public Simulation_BFER<B,R,Q>
{
protected:
	// LDPC bi-partite graph representation
	std::vector<unsigned char> n_variables_per_parity;
	std::vector<unsigned char> n_parities_per_variable;
	std::vector<unsigned int > transpose;

public:
	Simulation_BFER_LDPC(const parameters& params);

	virtual ~Simulation_BFER_LDPC();

protected:
	void          launch_precompute();
	void          snr_precompute   ();
	Encoder<B>*   build_encoder    (const int tid = 0);
	Decoder<B,Q>* build_decoder    (const int tid = 0);
};

#endif /* SIMULATION_LDPC_HPP_ */
