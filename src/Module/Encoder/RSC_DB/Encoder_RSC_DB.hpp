#ifndef ENCODER_RSC_DB_HPP_
#define ENCODER_RSC_DB_HPP_

#include <vector>

#include "../Encoder.hpp"

namespace aff3ct
{
namespace module
{

template <typename B>
class Encoder_turbo_DB;

template <typename B = int>
class Encoder_RSC_DB: public Encoder<B>
{
	friend Encoder_turbo_DB<B>;

private:
	static const std::string default_std;

	const int  n_ff;               // number of D flip-flop
	const int  n_states;           // number of states in the trellis
	const bool buffered_encoding;

	std::vector<std::vector<int>> next_state;
	std::vector<std::vector<int>> out_parity;

	std::vector<std::vector<int>> poly;
	std::vector<std::vector<int>> circ_states;

public:
	Encoder_RSC_DB(const int& K, const int& N,
	               const std::string &standard = default_std,
	               const bool buffered_encoding = true,
	               const int n_frames = 1);
	virtual ~Encoder_RSC_DB() = default;

	int tail_length() const {return 0;}
	std::vector<std::vector<int>> get_trellis();

	bool is_codeword(const B *X_N);
	bool is_buffered() const;

protected:
	void _encode(const B *U_K, B *X_N, const int frame_id);

	void __encode_from_state(const B* U_K, B* X_N, const bool only_parity, const int init_state, int& end_state);
	void __pre_encode(const B* U_K, int& end_state);

private:
	int calc_state_trans(const int in_state, const int in, int& par);
};
}
}

#endif /* ENCODER_RSC_DB_HPP_ */
