#ifndef MODEM_SCMA_HPP_
#define MODEM_SCMA_HPP_

#include <memory>

#include "Tools/Noise/Noise.hpp"
#include "Tools/Noise/Sigma.hpp"
#include "Tools/Code/SCMA/Codebook.hpp"
#include "Tools/Code/SCMA/modem_SCMA_functions.hpp"
#include "Tools/Algo/Multidimensional_vector/Vector_2D.hpp"
#include "Tools/Algo/Multidimensional_vector/Vector_3D.hpp"
#include "Tools/Algo/Multidimensional_vector/Vector_4D.hpp"
#include "Module/Modem/Modem.hpp"

namespace aff3ct
{
namespace module
{

template <typename B = int, typename R = float, typename Q = R, tools::proto_psi<Q> PSI = tools::psi_0>
class Modem_SCMA : public Modem<B,R,Q>
{
private:
	std::unique_ptr<const tools::Codebook<R>> CB_ptr;
	const tools::Codebook<R>& CB;

	tools::Vector_4D<Q> arr_phi;
	tools::Vector_3D<Q> msg_user_to_resources;
	tools::Vector_3D<Q> msg_resource_to_users;
	tools::Vector_2D<Q> guess;
	const bool          disable_sig2;
	      R             n0; // 1 / n0 = 179.856115108
	const int           n_ite;

public:
	Modem_SCMA(const int N, std::unique_ptr<const tools::Codebook<R>>&& CB, const tools::Noise<R>& noise = tools::Sigma<R>(),
	           const bool disable_sig2 = false, const int n_ite = 1, const int n_frames = 6);
	virtual ~Modem_SCMA() = default;

	virtual void set_noise(const tools::Noise<R>& noise);

	virtual void modulate     (              const B* X_N1, R *X_N2, const int frame_id = -1); using Modem<B,R,Q>::modulate;
	virtual void demodulate   (              const Q *Y_N1, Q *Y_N2, const int frame_id = -1); using Modem<B,R,Q>::demodulate;
	virtual void demodulate_wg(const R *H_N, const Q *Y_N1, Q *Y_N2, const int frame_id = -1); using Modem<B,R,Q>::demodulate_wg;
	virtual void filter       (              const R *Y_N1, R *Y_N2, const int frame_id = -1); using Modem<B,R,Q>::filter;

	static bool is_complex_mod();
	static bool is_complex_fil();
	static int size_mod(const int N, const int bps);
	static int size_fil(const int N, const int bps);

private:
	Q phi(const Q* Y_N1, int i, int j, int k, int re, int batch);
	Q phi(const Q* Y_N1, int i, int j, int k, int re, int batch, const R* H_N);
	void demodulate_batch(Q* Y_N2, int batch);
	Q normalize_prob_msg_resource_to_users(int user, int resource_ind, int codeword);
};
}
}

#include "Module/Modem/SCMA/Modem_SCMA.hxx"

#endif /* MODEM_SCMA_HPP_ */
