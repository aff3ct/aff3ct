/*!
 * \file
 * \brief Class module::Encoder_RSC_sys.
 */
#ifndef ENCODER_RSC_SYS_HPP_
#define ENCODER_RSC_SYS_HPP_

#include <vector>

#include "Module/Encoder/Encoder.hpp"
#include "Tools/Interface/Interface_get_trellis.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int>
class Encoder_RSC_sys : public Encoder<B>, public tools::Interface_get_trellis
{
protected:
	const int n_ff;     // number of D flip-flop
	const int n_states; // number of states in the trellis

	const bool buffered_encoding;

public:
	Encoder_RSC_sys(const int& K, const int& N, const int n_ff, const bool buffered_encoding);
	virtual ~Encoder_RSC_sys() = default;

	int get_n_ff   ();
	int tail_length() const;

	virtual std::vector<std::vector<int>> get_trellis();

	bool is_codeword(const B *X_N);

protected:
	void _encode(const B *U_K, B *X_N, const size_t frame_id);

	virtual int inner_encode(const int bit_sys, int &state) = 0;
	virtual int tail_bit_sys(const int &state             ) = 0;

private:
	void __encode(const B* U_K, B* sys, B* tail_sys, B* par, B* tail_par, const int stride = 1, const int stride_tail = 1);
	bool _is_codeword(const B* sys, const B* tail_sys, const B* par, const B* tail_par, const int stride = 1,
	                  const int stride_tail = 1);
};
}
}

#endif /* ENCODER_RSC_SYS_HPP_ */
