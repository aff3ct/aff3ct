/*!
 * \file
 * \brief Class module::Encoder_RSC_DB.
 */
#ifndef ENCODER_RSC_DB_HPP_
#define ENCODER_RSC_DB_HPP_

#include <string>
#include <vector>

#include "Module/Encoder/Encoder.hpp"
#include "Tools/Interface/Interface_get_trellis.hpp"

namespace aff3ct
{
namespace module
{

template <typename B>
class Encoder_turbo_DB;

template <typename B = int>
class Encoder_RSC_DB: public Encoder<B>, public tools::Interface_get_trellis
{
#ifndef DOXYGEN_SHOULD_SKIP_THIS
	friend Encoder_turbo_DB<B>;
#endif

private:
	const int  n_ff;               // number of D flip-flop
	const int  n_states;           // number of states in the trellis
	const bool buffered_encoding;

	std::vector<std::vector<int>> next_state;
	std::vector<std::vector<int>> out_parity;

	std::vector<std::vector<int>> poly;
	std::vector<std::vector<int>> circ_states;

public:
	Encoder_RSC_DB(const int& K, const int& N,
	               const std::string standard = "DVB-RCS1",
	               const bool buffered_encoding = true);
	virtual ~Encoder_RSC_DB() = default;

	virtual Encoder_RSC_DB<B>* clone() const;

	int tail_length() const {return 0;}
	std::vector<std::vector<int>> get_trellis();

	bool is_codeword(const B *X_N);
	bool is_buffered() const;

protected:
	void _encode(const B *U_K, B *X_N, const size_t frame_id);

	void __encode_from_state(const B* U_K, B* X_N, const bool only_parity, const int init_state, int& end_state);
	void __pre_encode(const B* U_K, int& end_state);

private:
	int calc_state_trans(const int in_state, const int in, int& par);
};
}
}

#endif /* ENCODER_RSC_DB_HPP_ */
