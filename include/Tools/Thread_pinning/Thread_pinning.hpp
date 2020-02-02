#ifndef THREAD_PINNING_HPP
#define THREAD_PINNING_HPP

namespace aff3ct
{
namespace tools
{
class Thread_pinning
{
public:
	static void init();
	static void destroy();
	static void pin();
	static void unpin();

	static void set_logs(const bool enable_logs);
	static bool is_logs();

	static void example1();
	static void example2();
	static void example3();
	static void example4();
	static void example5();
	static void example6();
};
}
}

#endif /* THREAD_PINNING_HPP */
