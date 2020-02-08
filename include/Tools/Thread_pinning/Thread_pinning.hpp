#ifndef THREAD_PINNING_HPP
#define THREAD_PINNING_HPP

#include <string>

namespace aff3ct
{
namespace tools
{
class Thread_pinning
{
public:
	static void init();
	static void destroy();
	static void pin(const size_t puid);
	static void unpin();

	static std::string get_cur_cpuset_str();

	static void set_logs(const bool enable_logs);
	static bool is_logs();
};
}
}

#endif /* THREAD_PINNING_HPP */
