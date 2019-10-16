#ifndef Reporter_EXIT_HPP_
#define Reporter_EXIT_HPP_

#include "Module/Monitor/EXIT/Monitor_EXIT.hpp"
#include "Tools/Noise/Noise.hpp"
#include "Tools/Display/Reporter/Reporter_monitor.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int32_t, typename R = float>
class Reporter_EXIT : public Reporter_monitor<module::Monitor_EXIT<B,R>>
{
public:
	using Rm = Reporter_monitor<module::Monitor_EXIT<B,R>>;
	using typename Rm::M;
	using typename Rm::report_t;

protected:
	const Noise<R>& noise_a;

public:
	Reporter_EXIT(const M &monitor, const Noise<R>& noise_a);

	virtual ~Reporter_EXIT() = default;

	report_t report(bool final = false);

private:
	void create_groups();
};
}
}

#endif /* Reporter_EXIT_HPP_ */
