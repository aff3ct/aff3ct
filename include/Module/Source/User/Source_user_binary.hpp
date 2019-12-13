#ifndef SOURCE_USER_BINARY_HPP_
#define SOURCE_USER_BINARY_HPP_

#include <string>
#include <vector>
#include <fstream>
#include <thread>
#include <memory>

#include "Module/Source/Source.hpp"

namespace aff3ct
{
namespace module
{
template <typename B>
class Source_user_binary : public Source<B>
{
private:
	std::shared_ptr<std::ifstream> source_file;
	static std::thread::id master_thread_id;

public:
	Source_user_binary(const int K, std::string filename, const int n_frames = 1);
	virtual ~Source_user_binary() = default;

	virtual Source_user_binary<B>* clone() const;

protected:
	void _generate(B *U_K, const int frame_id);
};
}
}

#endif /* SOURCE_USER_BINARY_HPP_ */
