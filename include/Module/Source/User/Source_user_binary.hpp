#ifndef SOURCE_USER_BINARY_HPP_
#define SOURCE_USER_BINARY_HPP_

#include <string>
#include <vector>
#include <fstream>
#include <memory>

#include "Module/Source/Source.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int32_t>
class Source_user_binary : public Source<B>
{
private:
	std::ifstream source_file;
	const bool auto_reset;
	const bool fifo_mode;
	bool done;
	size_t n_left;
	std::vector<char> memblk;
	std::vector<B> left_bits; // to store bits that are left by last call (n_left & n_completing)

public:
	Source_user_binary(const int K,
	                   const std::string &filename,
	                   const bool auto_reset = true,
	                   const bool fifo_mode = false);
	virtual ~Source_user_binary() = default;

	virtual bool is_done() const;

	virtual void reset();

protected:
	void _generate(B *U_K, const size_t frame_id);
};
}
}

#endif /* SOURCE_USER_BINARY_HPP_ */
