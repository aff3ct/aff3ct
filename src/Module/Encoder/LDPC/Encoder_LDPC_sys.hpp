#ifndef ENCODER_LDPC_SYS_HPP_
#define ENCODER_LDPC_SYS_HPP_

#include "../Encoder_sys.hpp"

template <typename B>
class Encoder_LDPC_sys : public Encoder_sys<B>
{
public:
	Encoder_LDPC_sys(const int K, const int N, const int n_frames = 1, const std::string name = "Encoder_LDPC_sys");
	virtual ~Encoder_LDPC_sys();

	virtual void encode    (const mipp::vector<B>& U_K, mipp::vector<B>& X_N) = 0;
	virtual void encode_sys(const mipp::vector<B>& U_K, mipp::vector<B>& par) = 0;

	virtual std::vector<unsigned char> get_n_variables_per_parity () = 0;
	virtual std::vector<unsigned char> get_n_parities_per_variable() = 0;
	virtual std::vector<unsigned int > get_transpose              () = 0;
};

#endif /* ENCODER_LDPC_SYS_HPP_ */
