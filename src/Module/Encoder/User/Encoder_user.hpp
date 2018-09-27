#ifndef ENCODER_USER_HPP_
#define ENCODER_USER_HPP_

#include <string>

#include "../Encoder.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int>
class Encoder_user : public Encoder<B>
{
private:
	std::vector<std::vector<B>> codewords;
	int cw_counter;

public:
	Encoder_user(const int K, const int N, const std::string &filename, const int n_frames = 1, const int start_idx = 0);
	virtual ~Encoder_user() = default;

	bool is_sys() const;

protected:
	void _encode(const B *U_K, B *X_N, const int frame_id);
};
}
}

#endif /* ENCODER_USER_HPP_ */
