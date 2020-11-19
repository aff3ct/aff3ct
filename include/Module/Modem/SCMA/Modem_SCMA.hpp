/*!
 * \file
 * \brief Class module::Modem_SCMA.
 */
#ifndef MODEM_SCMA_HPP_
#define MODEM_SCMA_HPP_

#include <memory>
#include <string>

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
	const tools::Codebook<R> CB;

	tools::Vector_4D<Q> arr_phi;
	tools::Vector_3D<Q> msg_user_to_resources;
	tools::Vector_3D<Q> msg_resource_to_users;
	tools::Vector_2D<Q> guess;
	const bool          disable_sig2;
	      R             n0; // 1 / n0 = 179.856115108
	const int           n_ite;

public:
	Modem_SCMA(const int N, const std::string &codebook_path, const bool disable_sig2 = false,
	           const int n_ite = 1);
	virtual ~Modem_SCMA() = default;

	virtual Modem_SCMA<B,R,Q,PSI>* clone() const;

	static bool is_complex_mod();
	static bool is_complex_fil();
	static int size_mod(const int N, const int bps);
	static int size_fil(const int N, const int bps);
	static int size_mod(const int N, const std::string &codebook_path);
	static int size_fil(const int N, const std::string &codebook_path);

	virtual void set_n_frames(const size_t n_frames);

protected:
	virtual void _modulate     (                               const B* X_N1, R *X_N2, const size_t frame_id);
	virtual void _demodulate   (const float *CP,               const Q *Y_N1, Q *Y_N2, const size_t frame_id);
	virtual void _demodulate_wg(const float *CP, const R *H_N, const Q *Y_N1, Q *Y_N2, const size_t frame_id);
	virtual void _filter       (const float *CP,               const R *Y_N1, R *Y_N2, const size_t frame_id);

private:
	Q phi(const Q* Y_N1, int i, int j, int k, int re, int batch);
	Q phi(const Q* Y_N1, int i, int j, int k, int re, int batch, const R* H_N);
	void demodulate_batch(Q* Y_N2, int batch);
	Q normalize_prob_msg_resource_to_users(int user, int resource_ind, int codeword);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Modem/SCMA/Modem_SCMA.hxx"
#endif

#endif /* MODEM_SCMA_HPP_ */
