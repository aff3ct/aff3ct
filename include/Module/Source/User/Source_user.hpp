/*!
 * \file
 * \brief Class module::Source_user.
 */
#ifndef SOURCE_USER_HPP_
#define SOURCE_USER_HPP_

#include <string>
#include <vector>

#include "Module/Source/Source.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int>
class Source_user : public Source<B>
{
private:
	std::vector<std::vector<B>> source;
	int src_counter;

public:
	Source_user(const int K, const std::string &filename, const int start_idx = 0);
	virtual ~Source_user() = default;

	virtual Source_user<B>* clone() const;

protected:
	void _generate(B *U_K, const size_t frame_id);
};
}
}

#endif /* SOURCE_USER_HPP_ */
