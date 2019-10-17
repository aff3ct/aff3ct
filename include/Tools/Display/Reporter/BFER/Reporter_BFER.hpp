/*!
 * \file
 * \brief Class tools::Reporter_BFER.
 */
#ifndef Reporter_BFER_HPP_
#define Reporter_BFER_HPP_

#include "Module/Monitor/BFER/Monitor_BFER.hpp"
#include "Tools/Display/Reporter/Reporter_monitor.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int32_t>
class Reporter_BFER : public Reporter_monitor<module::Monitor_BFER<B>>
{
public:
	using Rm = Reporter_monitor<module::Monitor_BFER<B>>;
	using typename Rm::M;
	using typename Rm::report_t;

	explicit Reporter_BFER(const M &monitor);

	virtual ~Reporter_BFER() = default;

	report_t report(bool final = false);

private:
	void create_groups();
};
}
}

#endif /* Reporter_BFER_HPP_ */
