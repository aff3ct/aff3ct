#ifndef SOURCE_USER_HPP_
#define SOURCE_USER_HPP_

#include <string>
#include <random>
#include <vector>

#include "../Source.hpp"

namespace aff3ct
{
namespace module
{
template <typename B>
class Source_user : public Source<B>
{
private:
	std::vector<std::vector<B>> source;
	int src_counter;

public:
	Source_user(const int K, std::string filename, const int n_frames = 1, const int start_idx = 0);
	virtual ~Source_user() = default;

protected:
	void _generate(B *U_K, const int frame_id);
};
}
}

#endif /* SOURCE_USER_HPP_ */
